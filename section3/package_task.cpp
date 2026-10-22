#include <iostream>
#include <thread>
#include <future>
#include <chrono>

int add(int x, int y)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "add function runs on thread: "
              << std::this_thread::get_id() << "\n";
    return x + y;
}

void task_thread()
{
    std::packaged_task<int(int, int)> task_1(add);
    std::future<int> future_1 = task_1.get_future();

    std::thread thread_1(std::move(task_1), 5, 6);
    thread_1.detach();

    std::cout << "task thread - " << future_1.get() << "\n";
}

void task_normal()
{
    std::packaged_task<int(int, int)> task_1(add);
    std::future<int> future_1 = task_1.get_future();

    task_1(7, 8);

    std::cout << "task normal - " << future_1.get() << "\n";
}

int main()
{
    task_thread();
    task_normal();

    return 0;
}

// Difference:
// - task_thread() executes add(5, 6) on a separate detached thread.
// - task_normal() executes add(7, 8) directly on the current thread.
// Both retrieve the result using future.get().