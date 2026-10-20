#include<bits/stdc++.h>
#include <thread>
#include <chrono>
using namespace std;


void worker()
{
    for (int i = 1; i <= 5; i++)
    {
        cout << "Worker thread: " << i << endl;

        this_thread::sleep_for(chrono::seconds(1));
    }

    cout << "Worker thread finished!" << endl;
}

int main()
{
    cout << "Main thread started." << endl;

    // Create a new thread
    thread t(worker);

    // Detach the thread
    t.detach();

    cout << "Main thread continues immediately..." << endl;

    // Main does some other work
    for (int i = 1; i <= 3; i++)
    {
        cout << "Main thread: " << i << endl;

        this_thread::sleep_for(chrono::seconds(1));
    }

    cout << "Main thread finished." << endl;

    return 0;
}