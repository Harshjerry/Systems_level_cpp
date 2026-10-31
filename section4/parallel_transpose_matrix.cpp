#include <bits/stdc++.h>
#include <thread>
#include <future>
using namespace std;

void process_block(const vector<vector<int>>& A,
                   vector<vector<int>>& B,
                   int start, int end)
{
    int m = A[0].size();

    for (int idx = start; idx < end; idx++) {
        int i = idx / m;
        int j = idx % m;

        B[j][i] = A[i][j];
    }
}

void transpose_parallel(const vector<vector<int>>& A,
                        vector<vector<int>>& B)
{
    int n = A.size();
    if (n == 0) return;

    int m = A[0].size();
    if (m == 0) return;

    int total_cells = n * m;

    int num_threads = thread::hardware_concurrency();

    if (num_threads == 0)
        num_threads = 2;

    num_threads = min(num_threads, total_cells);

    int block_size = (total_cells + num_threads - 1) / num_threads;

    vector<thread> threads;

    for (int i = 0; i < num_threads; i++) {

        int start = i * block_size;
        int end = min(start + block_size, total_cells);

        if (start >= total_cells)
            break;

        threads.emplace_back(
            process_block,
            cref(A),
            ref(B),
            start,
            end
        );
    }

    // Wait for all threads to finish
    for (auto& t : threads) {
        t.join();
    }
}

int main()
{
    vector<vector<int>> A = {
        {1, 2, 3},
        {4, 5, 6}
    };

    int n = A.size();
    int m = A[0].size();

    vector<vector<int>> B(m, vector<int>(n));

    transpose_parallel(A, B);

    for (auto& row : B) {
        for (int x : row)
            cout << x << " ";
        cout << "\n";
    }
}