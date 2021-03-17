//
//  Buffer.hpp
//  RemoteDesktopProgram
//
//  Created by Zhanan Ao on 01/03/2021.
//

#ifndef Buffer_hpp
#define Buffer_hpp

#include <iostream>
#include <thread>
#include <mutex>
#include<deque>
#include <map>
#include <vector>
#include <condition_variable>
#include <chrono>
#include <random>

using namespace std;
//std::mutex mu;
//global variables

const int buffern = 6; //number of data attributes+timestamp
const int batchN = 5;

class Buffer {
public:
    Buffer() {}
    
    void put(float a[buffern]);

    int time();     //THIS WILL ALSO BE IN FLOAT -- may cause problem
    
    void toggleUserFlag() {
        this->userFlag = !this->userFlag;
    }
    
    bool readUserFlag() {
        return this->userFlag;
    }
    
    int readCount() {
        return this->count;
    }
    
    void terminate();

    float* get();

private:
    std::mutex bufmu;   //instance of mutual exclusion lock
    std::condition_variable cond;  //instance of a condition variable for wait and notify_all
    std::vector<float> buffers [buffern];
    float back[buffern] = {0};
    bool userFlag = true;
    int count = 0;
    bool terminated = false;
};

#endif /* Buffer_hpp */
