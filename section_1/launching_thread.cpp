#include <iostream>
#include <thread>

using namespace std;

void foo()
{
    cout << "Hello from foo - Thread ID: "
         << this_thread::get_id() << endl;
}

class callable_class
{
public:
    void operator()()
    {
        cout << "Hello from class - Thread ID: "
             << this_thread::get_id() << endl;
    }
};

int main()
{
    thread thread1(foo);

    callable_class obj;
    thread thread2(obj);

    thread thread3([]()
    {
        cout << "Hello from lambda - Thread ID: "
             << this_thread::get_id() << endl;
    });

    thread1.join();
    thread2.join();
    thread3.join();

    cout << "Hello from main - Thread ID: "
         << this_thread::get_id() << endl;

    return 0;
}