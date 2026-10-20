#include<bits/stdc++.h>
#include <thread>
using namespace std;

void downloadFile() {
    cout << "Downloading file...\n";
    // simulate downloading
    this_thread::sleep_for(chrono::seconds(5));

    cout << "Download complete!\n";
}

int main() {

    thread downloader(downloadFile);

    downloader.join();

    cout << "Now I can open the downloaded file.\n";
}

// So join() is extremely useful when:
// The parent/calling thread needs the worker's work to be completed.