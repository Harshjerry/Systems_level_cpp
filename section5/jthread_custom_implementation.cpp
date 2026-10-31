#include <bits/stdc++.h>
#include <thread>
#include <future>
#include <atomic>
using namespace std;


class interrupt_flag
{
    atomic<bool> flag;

public:

    interrupt_flag() : flag(false) {}

    void set()
    {
        flag.store(true);
    }

    bool is_set()
    {
        return flag.load();
    }
};


// Every thread has its own interrupt flag
thread_local interrupt_flag this_thread_flag;


class jthread_local
{
    thread _thread;
    interrupt_flag* flag;

public:

    template<typename FunctionType>
    jthread_local(FunctionType f)
    {
        promise<interrupt_flag*> p;

        _thread = thread([f, &p]() {

            p.set_value(&this_thread_flag);

            f();

        });

        flag = p.get_future().get();
    }


    void interrupt()
    {
        flag->set();
    }


    ~jthread_local()
    {
        if (_thread.joinable())
        {
            interrupt();
            _thread.join();
        }
    }

};


void task()
{
    int i = 0;

    while (!this_thread_flag.is_set())
    {
        cout << "Working: " << i++ << endl;
        this_thread::sleep_for(chrono::milliseconds(500));
    }

    cout << "Thread interrupted!" << endl;
}


int main()
{
    jthread_local t(task);

    this_thread::sleep_for(chrono::seconds(3));

    cout << "Sending interrupt signal..." << endl;

    t.interrupt();

    return 0;
}