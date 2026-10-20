#include<bits/stdc++.h>
#include <thread>
#include <chrono>
using namespace std;
 void cleaning_crew(){
    cout<<"Cleaning crew is cleaning the ship"<<endl;
 }
  void full_speed(){
    cout<<"Captain is giving full speed to the ship"<<endl;
  }
  void stop_engine(){
    cout<<"Captain is stopping the engine of the ship"<<endl;
  }

int main(){
      while(true){
        int input;
        cin>>input;
        if(input!=100 && (input<1||input>3)){
            cout<<"Invalid input"<<endl;
            continue;
        }
        if(input==1){
            std::thread cleaning_thread(cleaning_crew);
            cleaning_thread.detach(); 
        }else if(input==2){
            std::thread full_speed_thread(full_speed);
            full_speed_thread.join();
        }else if(input==3){
            std::thread stop_engine_thread(stop_engine);
            stop_engine_thread.join();
        }else if(input==100){
            break;
        }
      }
      cout<<"Exiting the program"<<endl;
      return 0;
}



// g++ -std=c++20 captain.cpp -o captain && ./captain