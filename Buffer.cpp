//
//  Buffer.cpp
//  RemoteDesktopProgram
//
//  Created by Zhanan Ao on 01/03/2021.
//

#include "Buffer.hpp"

float* Buffer::get() {
    std::unique_lock<std::mutex> locker(bufmu);
    for (int i = 0; buffern > i; i++) {
        while (buffers[i].size() == 0 && this->terminated == false) {
            std::cout << "consumer locked" << std::endl;
            cond.wait(locker);
            std::cout << "consumer unlocked" << std::endl;
        }
        if (buffers[i].size() > 0) {
                //back[i] = buffers[i].back();        //always getting the newest data -- error
                //buffers[i].pop_back();
            back[i] = buffers[i][0];
            buffers[i].erase(buffers[i].begin());       //inefficient need changing
            this->count--;
        }
    }
    locker.unlock();
    cond.notify_all();      //dont need
    return back;
}

int Buffer::time()  {
    std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
    std::chrono::system_clock::duration dtn = tp.time_since_epoch();
    int seconds_epoch = (int) dtn.count() * std::chrono::system_clock::period::num / std::chrono::system_clock::period::den;
    return seconds_epoch;
}

void Buffer::put(float *a) {
    std::unique_lock<std::mutex> locker(bufmu);
    for (int i = 0; buffern > i; i++) {
        buffers[i].push_back(a[i]);             //push back put new data to back
        this->count++;
    }
    locker.unlock();
    cond.notify_all();
}

void Buffer::terminate() {
    std::unique_lock<std::mutex> locker(bufmu);
    this->terminated = true;
    this->cond.notify_all();
}
