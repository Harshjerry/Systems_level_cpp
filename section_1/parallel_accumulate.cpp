
#include <iostream>
#include <vector>
#include <thread>
#include <numeric>
#include <algorithm>
#include <functional>

using namespace std;

const int MIN_BLOCK_SIZE = 2;

// Normal function executed by each worker thread
template <typename Iterator, typename T>
void calculate_sum(Iterator start, Iterator end,
                   vector<T>& results, int i)
{
    results[i] = accumulate(start, end, T{});
}

template <typename Iterator, typename T>
T parallel_accumulate(Iterator start, Iterator end, T init)
{
    int size = distance(start, end);

    if (size == 0)
        return init;

    // Determine available hardware threads
    unsigned int hardware_threads = thread::hardware_concurrency();

    if (hardware_threads == 0)
        hardware_threads = 2;

    // Calculate the number of blocks dynamically
    int threads_by_elements =
        (size + MIN_BLOCK_SIZE - 1) / MIN_BLOCK_SIZE;

    int num_threads = min(
        static_cast<int>(hardware_threads),
        threads_by_elements
    );

    int block_size = size / num_threads;

    // One result slot per block
    vector<T> results(num_threads);

    // Store worker threads
    vector<thread> threads;

    Iterator block_start = start;

    // Create worker threads for all blocks except the last
    for (int i = 0; i < num_threads - 1; i++)
    {
        Iterator block_end = block_start;
        advance(block_end, block_size);

        // push_back() stores a newly constructed thread object
        threads.push_back(
            thread(calculate_sum<Iterator, T>,
                   block_start, block_end, ref(results), i)
        );

        block_start = block_end;
    }

    // Main thread calculates the last block
    results[num_threads - 1] =
        accumulate(block_start, end, T{});

    // Wait for all worker threads
    for (auto& t : threads)
        t.join();

    // Combine partial sums
    return accumulate(results.begin(), results.end(), init);
}

int main()
{
    vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8};

    int result = parallel_accumulate(
        nums.begin(), nums.end(), 0
    );

    cout << "Total sum = " << result << endl;

    return 0;
}
