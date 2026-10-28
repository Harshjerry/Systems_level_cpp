#include <bits/stdc++.h>
#include <future>
#include <thread>
#include <atomic>

using namespace std;

void find_element_sequential(
    const vector<int>& arr, int start, int end,
    int target, atomic<bool>& found, promise<int>& prom)
{
    for (int i = start; i < end; i++)
    {
        if (found.load())
            return;

        if (arr[i] == target)
        {
            found = true;
            prom.set_value(i);
            return;
        }
    }
}

int find_element_parallel(const vector<int>& arr, int target)
{
    int n = arr.size();

    if (n == 0)
        return -1;

    int num_threads = thread::hardware_concurrency();
    if (num_threads == 0) num_threads = 2;

    num_threads = min(num_threads, n);
    int block_size = (n + num_threads - 1) / num_threads;

    atomic<bool> found(false);
    promise<int> prom;
    future<int> result = prom.get_future();

    vector<thread> threads;

    for (int i = 0; i < num_threads; i++)
    {
        int start = i * block_size;
        int end = min(start + block_size, n);

        if (start >= n) break;

        threads.emplace_back(
            find_element_sequential,
            cref(arr), start, end, target,
            ref(found), ref(prom)
        );
    }

    for (auto& t : threads)
        t.join();

    if (found.load())
        return result.get();

    return -1;
}

int main()
{
    vector<int> arr = {10, 20, 30, 40, 50, 60, 70, 80};

    int index = find_element_parallel(arr, 60);

    cout << "Index: " << index << '\n';
}