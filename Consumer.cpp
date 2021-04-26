//
//  Consumer.cpp
//  RemoteDesktopProgram
//
//  Created by Zhanan Ao on 28/02/2021.
//

#include "Consumer.hpp"

void write2CSV(float data[buffern], long long timestamp) {
    std::ofstream myfile;
    myfile.open ("./data.csv", std::ios_base::app);
    for (int i = 0; i<buffern; i++) {
        if (i == buffern-1) {
            myfile << data[i] << ",";
        }
        else {
            myfile << data[i] << ",";
        }
        
    }
    myfile << timestamp << ",\n";
    std::cout << "writing data" << std::endl;
    myfile.close();
}

void cons(Buffer& b) {
    long long* timestamp = new long long[1];
    float* ans; // pointer to array of buffer elements
    float data[buffern] = {};
    int count = 0;
    Database d;
    long long t1 = 0;
    long long t2 = 0;
    int averageTime = 1000; //ms
    if (d.establishConnection() == 0) {
        std::cout << "Database Connected" << std::endl;
    }
    while(1){
        if (b.readUserFlag() == false && b.readCount() == 0) {
            break;
        }
        ans = b.get(timestamp);

        if (t1 == 0) {
            t1 = *timestamp;
        }
        else {
            t2 = *timestamp;
        }

        if (t2 - t1 > averageTime) {
            //send data
            std::cout << "averaging - count = " << count << std::endl;

            for (int i = 0; i < buffern; i++) {
                data[i] += ans[i];
            }
            count++;
            for (int i = 0; i < buffern; i++) {
                data[i] = data[i] / (float)count;;

            }
            d.insertDataInstance(0, *timestamp);
            std::cout << d.lastID() << std::endl;
            d.insertMeasuredData(data, buffern);
            count = 0;
            t1 = 0;
            t2 = 0;
            std::fill(std::begin(data), std::end(data), 0);
        }
        else {
            //accumalate average
            std::cout << "accumalating" << std::endl;
            for (int i = 0; i < buffern; i++) {
                data[i] += ans[i];
            }
            count++;
        }
        
        /*insert conversion code here*/
        //write2CSV(ans,*timestamp);

        //sleep();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));  //line just for debug
        
        //add in the deleted logic to exit loop
    }
    d.closeConnection();
    std::cout << "Consumer terminated..." << std::endl;
}
