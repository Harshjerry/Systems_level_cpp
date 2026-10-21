
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

class bus_trip
{
private:
    bool arrived = false;
    int total_distance = 10;
    int distance_traveled = 0;

    mutex m;
    condition_variable cv;

public:
    void keep_driving()
    {
        while (true)
        {
            {
                lock_guard<mutex> lock(m);

                distance_traveled++;

                if (distance_traveled == total_distance)
                {
                    arrived = true;
                }
            } // Mutex is released here.

            if (arrived)
            {
                cv.notify_all();
                break;
            }

            this_thread::yield();
        }
    }

    void ask_driver_to_wake_up_at_right_time()
    {
        unique_lock<mutex> lock(m);

        cv.wait(lock, [this]()
                { return arrived; });

        cout << "Bus has arrived!" << endl;
    }
};

int main()
{
    bus_trip trip;

    thread driver_thread(&bus_trip::keep_driving, &trip);
    thread passenger_thread(
        &bus_trip::ask_driver_to_wake_up_at_right_time, &trip);

    driver_thread.join();
    passenger_thread.join();

    return 0;
}
