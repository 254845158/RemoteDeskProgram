//
//  Consumer.cpp
//  RemoteDesktopProgram
//
//  Created by Zhanan Ao on 28/02/2021.
//

#include "Consumer.hpp"

void write2CSV(float data[buffern]) {
    std::ofstream myfile;
    myfile.open ("./data.csv", std::ios_base::app);
    for (int i = 0; i<buffern; i++) {
        if (i == buffern-1) {
            myfile << data[i] << ",\n";
        }
        else {
            myfile << data[i] << ",";
        }
        
    }
    std::cout << "writing data" << std::endl;
    myfile.close();
}

void cons(Buffer& b) {
    int w = 0;

    float* ans; // pointer to array of buffer elements
    
    while(1){
        if (b.readUserFlag() == false && b.readCount() == 0) {
            break;
        }
        ans = b.get();
        w++;
        
        /*insert conversion code here*/
        write2CSV(ans);
        //sleep();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));       //line just for debug
        
        //add in the deleted logic to exit loop
    }

    std::cout << "Consumer terminated..." << std::endl;
}
