#include <iostream>
#include <thread>
#include <chrono>
#include <stdexcept>

using namespace std;


class thread_guard
{
private:
    std::thread& t;

public:

    explicit thread_guard(std::thread& _t)
        : t(_t)
    {
    }

    ~thread_guard()
    {
        if (t.joinable())
        {
            t.join();
        }
    }

    thread_guard(thread_guard const&) = delete;

    thread_guard& operator=(thread_guard const&) = delete;
};


void foo()
{
    for (int i = 1; i <= 5; i++)
    {
        cout << "Worker thread: " << i << endl;

        this_thread::sleep_for(
            chrono::seconds(1)
        );
    }

    cout << "Worker finished!" << endl;
}


void other_operations()
{
    cout << "Main thread doing other operations..."
         << endl;

    this_thread::sleep_for(
        chrono::seconds(2)
    );

    cout << "Other operations completed."
         << endl;
}


void run()
{
    std::thread foo_thread(foo);
    thread_guard tg(foo_thread);

    // Do other work
    other_operations();

    // NO foo_thread.join() HERE!
    //
    // thread_guard's destructor will
    // automatically call join().
}


// --------------------------------------------------
// main()
// --------------------------------------------------

int main()
{
    cout << "Main started." << endl;

    run();

    cout << "Main finished." << endl;

    return 0;
}