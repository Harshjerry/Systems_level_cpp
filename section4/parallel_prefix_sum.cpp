
#include <bits/stdc++.h>
#include <thread>
#include <future>

using namespace std;

// Stage 1: Calculate the local prefix sum of one block
void process_block(
    vector<long long>& arr,
    int start,
    int end,
    promise<long long>& prom)
{
    for (int i = start + 1; i < end; i++)
    {
        arr[i] += arr[i - 1];
    }

    // Return the total sum of this block
    prom.set_value(arr[end - 1]);
}

// Stage 2: Add the offset to every element in a block
void add_offset(
    vector<long long>& arr,
    int start,
    int end,
    long long offset)
{
    for (int i = start; i < end; i++)
    {
        arr[i] += offset;
    }
}

void parallel_prefix_sum(vector<long long>& arr)
{
    int n = arr.size();

    if (n == 0)
        return;

    int num_threads = thread::hardware_concurrency();

    if (num_threads == 0)
        num_threads = 2;

    num_threads = min(num_threads, n);

    int block_size = (n + num_threads - 1) / num_threads;
    int num_blocks = (n + block_size - 1) / block_size;

    vector<promise<long long>> promises(num_blocks);
    vector<future<long long>> futures;
    vector<thread> threads;

    // Obtain futures before launching the threads
    for (int i = 0; i < num_blocks; i++)
    {
        futures.push_back(promises[i].get_future());
    }

    // STAGE 1: Calculate local prefix sums in parallel
    for (int i = 0; i < num_blocks; i++)
    {
        int start = i * block_size;
        int end = min(start + block_size, n);

        threads.emplace_back(
            process_block,
            ref(arr),
            start,
            end,
            ref(promises[i])
        );
    }

    for (auto& t : threads)
        t.join();

    // Collect the total of every block
    vector<long long> block_totals(num_blocks);

    for (int i = 0; i < num_blocks; i++)
    {
        block_totals[i] = futures[i].get();
    }

    // Calculate the offset for each block
    vector<long long> offsets(num_blocks, 0);

    for (int i = 1; i < num_blocks; i++)
    {
        offsets[i] = offsets[i - 1] + block_totals[i - 1];
    }

    // STAGE 2: Add offsets in parallel
    threads.clear();

    for (int i = 0; i < num_blocks; i++)
    {
        int start = i * block_size;
        int end = min(start + block_size, n);

        if (offsets[i] == 0)
            continue;

        threads.emplace_back(
            add_offset,
            ref(arr),
            start,
            end,
            offsets[i]
        );
    }

    for (auto& t : threads)
        t.join();
}

int main()
{
    vector<long long> arr = {1, 2, 3, 4, 5, 6, 7, 8};

    parallel_prefix_sum(arr);

    cout << "Prefix sum: ";

    for (long long x : arr)
        cout << x << " ";

    cout << '\n';

    return 0;
}
