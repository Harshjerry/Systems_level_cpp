
#include <bits/stdc++.h>
#include <thread>

using namespace std;

int mul(vector<vector<int>>& A, vector<vector<int>>& B, int r, int c)
{
    int ans = 0;
    int k = A[0].size();

    for (int i = 0; i < k; i++)
        ans += A[r][i] * B[i][c];

    return ans;
}

// Each thread calculates a range of output cells
void multiply_block(
    vector<vector<int>>& A,
    vector<vector<int>>& B,
    vector<vector<int>>& ans,
    int start,
    int end,
    int bc)
{
    for (int idx = start; idx < end; idx++)
    {
        int r = idx / bc;
        int c = idx % bc;

        ans[r][c] = mul(A, B, r, c);
    }
}

vector<vector<int>> multiply_parallel(
    vector<vector<int>>& A,
    vector<vector<int>>& B)
{
    int ar = A.size();
    int bc = B[0].size();

    vector<vector<int>> ans(ar, vector<int>(bc, 0));

    int total_cells = ar * bc;

    int num_threads = thread::hardware_concurrency();

    if (num_threads == 0)
        num_threads = 2;

    num_threads = min(num_threads, total_cells);

    int block_size = (total_cells + num_threads - 1) / num_threads;

    vector<thread> threads;

    for (int i = 0; i < num_threads; i++)
    {
        int start = i * block_size;
        int end = min(start + block_size, total_cells);

        if (start >= total_cells)
            break;

        threads.emplace_back(
            multiply_block,
            ref(A),
            ref(B),
            ref(ans),
            start,
            end,
            bc
        );
    }

    for (auto& t : threads)
        t.join();

    return ans;
}

int main()
{
    vector<vector<int>> A = {
        {1, 2, 3},
        {4, 5, 6}
    };

    vector<vector<int>> B = {
        {7, 8},
        {9, 10},
        {11, 12}
    };

    vector<vector<int>> ans = multiply_parallel(A, B);

    for (auto& row : ans)
    {
        for (int x : row)
            cout << x << " ";

        cout << '\n';
    }

    return 0;
}
