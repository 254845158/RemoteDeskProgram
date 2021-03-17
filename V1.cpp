//
//  main.cpp
//  v1
//
//  created by teppei yamada on 20/02/2021.
//

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
std::mutex mu;
//global variables

//std::unique_lock<std::mutex> locker;
const int buffern = 6; //number of data attributes+timestamp
const int datapoint = 500; //number of datapoints
const int batchN = 5;
bool userFlag = 0;

class buffer {
public:
    buffer() {}
    
    void put(float a[buffern]) {
        std::unique_lock<std::mutex> locker(bufmu);
        for (int i = 0; buffern > i; i++) {
            buffers[i].push_back(a[i]);
        }
        locker.unlock();
        cond.notify_all();
    }

    //void machineFlag(bool a) {
    //    flag = a;
    //}

    int time() {
        std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
        std::chrono::system_clock::duration dtn = tp.time_since_epoch();
        int seconds_epoch = dtn.count() * std::chrono::system_clock::period::num / std::chrono::system_clock::period::den;
        return seconds_epoch;
    }

    //bool getFlag() {
    //    return flag;
    //}
    bool getconsFlag() {
        return consFlag;
    }
    float* get() {
        std::unique_lock<std::mutex> locker(bufmu);
        for (int i = 0; buffern > i; i++) {
            while (buffers[i].size() == 0) {
                if (userFlag == 0) {
                    consFlag = 0;
                    break;
                }
                else {
                    std::unique_lock<std::mutex> glocker(mu);
                    cout << "buffer is empty..." << endl;
                    glocker.unlock();
                    cond.wait(locker);
                }
            }
            if (buffers[i].size() > 0) {
                back[i] = buffers[i].back();
                buffers[i].pop_back();
            }
        }
        locker.unlock();
        cond.notify_all();
        return back;
    }

private:
    std::mutex bufmu;   //instance of mutual exclusion lock
    std::condition_variable cond;  //instance of a condition variable for wait and notify_all
    std::vector<float> buffers [buffern];
    float back[buffern] = {0};
    bool flag = 1;
    int counter = 0;
    int consFlag = 1;
};

void sleep() {
    std::unique_lock<std::mutex> locker3(mu); //lock sleep with global lock
    std::mt19937 gen(time(0)); //A 'Mersenne_twister_engine' seeded by time(0).
    //time(0) gives the number of seconds elapsed since the start of the world according to Unix (00:00:00 on 1st January 1970)
    std::uniform_int_distribution<> dis(100, 500); //generate a random integer between 100-500
    std::this_thread::sleep_for(std::chrono::microseconds(dis(gen))); //sleep thread for random time between 0.1-0.5s
}

void prods(buffer& b,float n[][datapoint]) {
    b.machineFlag(1);
    float r[buffern];
    for (int s = 0; datapoint > s; s++) {
        for (int t = 0; buffern > t; t++) {
            r[t] = n[t][s];
            std::unique_lock<std::mutex> glocker1(mu);
            cout << "inserting"<< r[t] << endl;
            glocker1.unlock();
        }
            b.put(r);
            sleep();
    }
    b.machineFlag(0);
}

void cons(buffer& b) {
    int w = 0;
    int count = 0;
    float* ans; // pointer to array of buffer elements
    //float* ptr[batchN];
    while(b.getconsFlag()==1){
        //while (count > batchN) {
        //    ans = b.get();
        //    ptr[count] = ans;
        //    cout << "array " << *(ptr + count) << endl;
        //    count++;
        //}
            ans = b.get();
            for (int i = 0; buffern > i; i++) {
                std::unique_lock<std::mutex> glocker2(mu);
                cout << "datapoint " << w << ", attribute " << i << " = " << *(ans + i) << endl; // test for correct collection of data
                glocker2.unlock();
            }
        w++;
        /*insert conversion code here*/
        sleep();
    }
}

int main() {
    buffer buf;
    float d[buffern][datapoint];
    int k;
    for (float i = 0.0f; datapoint > i; i++) {
        k = i;
        for (int j = 0; buffern > j; j++) {
            d[j][k] = i;
            //cout << d[j][k] << endl;
        }
    }
    std::thread p(prods, std::ref(buf), d);
    std::thread c(cons, std::ref(buf));
    p.join();
    //cout << "producer terminated" << endl;
    c.join();
    std::cout << *(buf.get()) << endl;
    std::cout << "all threads terminated" << endl;
    return 0;
}

