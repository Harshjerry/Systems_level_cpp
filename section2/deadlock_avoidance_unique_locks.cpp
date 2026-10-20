#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <string>

using namespace std;

class bank_account
{
public:
    string name;
    double balance;
    mutex m;

    bank_account(string name, double balance)
        : name(name), balance(balance)
    {
    }

    // Withdraw money safely
    void withdraw(double amount)
    {
        lock_guard<mutex> lock(m);

        if (amount <= 0 || amount > balance)
        {
            cout << "Withdrawal failed for " << name << '\n';
            return;
        }

        balance -= amount;

        cout << name << " withdrew " << amount
             << ", balance = " << balance << '\n';
    }

    // Deposit money safely
    void deposite(double amount)
    {
        lock_guard<mutex> lock(m);

        if (amount <= 0)
        {
            cout << "Invalid deposit amount\n";
            return;
        }

        balance += amount;

        cout << name << " deposited " << amount
             << ", balance = " << balance << '\n';
    }

    // Transfer money safely between two accounts
    static void transfer(bank_account& from,
                         bank_account& to,
                         double amount)
    {
        if (&from == &to || amount <= 0)
        {
            cout << "Invalid transfer\n";
            return;
        }

        // Create lock objects WITHOUT locking the mutexes.
        // defer_lock means locking is postponed.
        unique_lock<mutex> ul_1(from.m, defer_lock);
        unique_lock<mutex> ul_2(to.m, defer_lock);

        // Acquire BOTH mutexes using a deadlock-avoidance algorithm.
        // Avoids deadlock even if another thread transfers in reverse order.
        lock(ul_1, ul_2);

        // Both accounts are now protected by their respective mutexes.
        if (amount > from.balance)
        {
            cout << "Transfer failed: insufficient balance in "
                 << from.name << '\n';
            return;
        }

        from.balance -= amount;
        to.balance += amount;

        cout << "Transferred " << amount
             << " from " << from.name
             << " to " << to.name << '\n';

        // No explicit unlock required.
        // Both unique_lock objects automatically unlock their mutexes
        // when they go out of scope.
    }

    // Display the balance safely
    void show_balance()
    {
        lock_guard<mutex> lock(m);

        cout << name << " balance = " << balance << '\n';
    }
};

int main()
{
    bank_account account_A("Alice", 1000);
    bank_account account_B("Bob", 1000);

    // Two threads transfer in opposite directions.
    // Without deadlock-safe locking, this pattern can cause deadlock.
    thread t1([&]()
    {
        for (int i = 0; i < 1000; i++)
            bank_account::transfer(account_A, account_B, 10);
    });

    thread t2([&]()
    {
        for (int i = 0; i < 1000; i++)
            bank_account::transfer(account_B, account_A, 10);
    });

    // Wait for both threads to finish.
    t1.join();
    t2.join();

    cout << "\nFinal balances:\n";
    account_A.show_balance();
    account_B.show_balance();

    return 0;
}