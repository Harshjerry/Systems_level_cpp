
#include <bits/stdc++.h>
#include <future>
#include <chrono>

using namespace std;

int partition(vector<int>& input, int s, int e)
{
    int pivot = input[e];
    int idx = s - 1;

    for (int j = s; j < e; j++)
    {
        if (input[j] <= pivot)
        {
            idx++;
            swap(input[idx], input[j]);
        }
    }

    swap(input[idx + 1], input[e]);
    return idx + 1;
}

// Sequential Quick Sort
void quick_sort_sequential(vector<int>& input, int s, int e)
{
    if (s >= e)
        return;

    int pivot = partition(input, s, e);

    quick_sort_sequential(input, s, pivot - 1);
    quick_sort_sequential(input, pivot + 1, e);
}

// Parallel Quick Sort with limited task creation
void quick_sort(vector<int>& input, int s, int e, int depth = 0)
{
    if (s >= e)
        return;

    int pivot = partition(input, s, e);

    // Stop creating asynchronous tasks after 3 levels
    const int MAX_DEPTH = 3;

    // Use sequential Quick Sort for small partitions
    const int THRESHOLD = 1000;

    if (depth >= MAX_DEPTH || e - s < THRESHOLD)
    {
        quick_sort_sequential(input, s, pivot - 1);
        quick_sort_sequential(input, pivot + 1, e);
        return;
    }

    // Sort left partition asynchronously
    future<void> future1 = async(
        launch::async,
        [&input, s, pivot, depth]()
        {
            quick_sort(input, s, pivot - 1, depth + 1);
        }
    );

    // Sort right partition on the current thread
    quick_sort(input, pivot + 1, e, depth + 1);

    // Wait for the left partition to finish
    future1.get();
}

int main()
{
    const int N = 100000;

    srand(time(nullptr));

    vector<int> input(N);

    // Generate 100,000 random integers
    for (int i = 0; i < N; i++)
        input[i] = rand() % 1000000 + 1;

    vector<int> input2 = input;

    // Sequential Quick Sort
    auto start = chrono::steady_clock::now();

    quick_sort_sequential(input2, 0, (int)input2.size() - 1);

    auto end = chrono::steady_clock::now();

    double sequential_time =
        chrono::duration<double, milli>(end - start).count();

    // Parallel Quick Sort
    start = chrono::steady_clock::now();

    quick_sort(input, 0, (int)input.size() - 1);

    end = chrono::steady_clock::now();

    double parallel_time =
        chrono::duration<double, milli>(end - start).count();

    cout << fixed << setprecision(3);

    cout << "Number of elements: " << N << '\n';

    cout << "Sequential Quick Sort: "
         << sequential_time << " ms\n";

    cout << "Parallel Quick Sort: "
         << parallel_time << " ms\n";

    if (parallel_time > 0)
    {
        cout << "Speedup: "
             << sequential_time / parallel_time << "x\n";
    }

    cout << "Sequential sorted: "
         << boolalpha
         << is_sorted(input2.begin(), input2.end()) << '\n';

    cout << "Parallel sorted: "
         << is_sorted(input.begin(), input.end()) << '\n';

    return 0;
}
