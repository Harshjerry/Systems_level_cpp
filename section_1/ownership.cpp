
#include <iostream>
#include <thread>
#include <utility>

using namespace std;

void foo()
{
    cout << "foo() is running on thread: "
         << this_thread::get_id() << endl;
}

void bar()
{
    cout << "bar() is running on thread: "
         << this_thread::get_id() << endl;
}

void run()
{
    // STEP 1: Create thread_1.
    // thread_1 owns a thread executing foo().
    thread thread_1(foo);

    // STEP 2: Transfer ownership from thread_1 to thread_2.
    // thread_2 now owns the original foo() execution.
    // thread_1 becomes empty (non-joinable).
    thread thread_2 = move(thread_1);

    // STEP 3: Assign a new thread to thread_1.
    // This is safe because thread_1 is empty.
    // thread_1 now owns a thread executing bar().
    thread_1 = thread(bar);

    // STEP 4: Create thread_3.
    // thread_3 owns another thread executing foo().
    thread thread_3(foo);

    // STEP 5: Finish bar() before replacing thread_1's ownership.
    // If we move into thread_1 while it is still joinable,
    // the program calls std::terminate().
    thread_1.join();

    // STEP 6: Transfer ownership from thread_3 to thread_1.
    // thread_1 is non-joinable after join(), so this is safe.
    // thread_1 now owns thread_3's foo() execution.
    // thread_3 becomes empty.
    thread_1 = move(thread_3);

    // STEP 7: Wait for the foo() execution now owned by thread_1.
    thread_1.join();

    // STEP 8: Wait for the original foo() execution,
    // which has been owned by thread_2 since STEP 2.
    thread_2.join();

    // thread_3 is empty, so we do not need to join it.
}

int main()
{
    run();
    return 0;
}
