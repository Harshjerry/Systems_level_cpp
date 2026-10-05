#include<bits/stdc++.h>
#include<thread>
using namespace std;
  void test(){ 
        cout<< "Hello from test function";
  }

 void functionA() {
    thread t(test);
    cout << "Function A is called." << endl;
    t.join();
 }

 void functionB() {
    cout << "Function B is called." << endl;
 }


int main() {
     thread threadA(functionA);
     thread threadB(functionB);
     threadA.join();
     threadB.join();
     cout<< "Hello from main function" << endl;
    return 0;
}