
#include <iostream>
#include <mutex>
#include <queue>
#include <memory>
#include <condition_variable>
#include <thread>
#include <future>

using namespace std;

int find_ans()
{
    return 5000;
}

void other_oper()
{
    cout << " doing other task"<<endl;
}

int main()
{
    future<int> fut = async(find_ans);
    other_oper();
    cout << " the answer is " << fut.get() << endl;
    return 0;
}
