//
//  main.cpp
//  RemoteDesktopProgram
//
//  Created by Zhanan Ao on 19/02/2021.
//

#include <iostream>
#include "UnitTest.hpp"
#include "Producer.hpp"
#include "Consumer.hpp"
#include "Buffer.hpp"
#include "MotorDrive.hpp"

int main(int argc, const char * argv[]) {
    // insert code here...
    Buffer buf;
    MotorDrive motorDrive;
    motorDrive.establishConnection();
    
    std::thread p(producer, std::ref(motorDrive) ,std::ref(buf));
    std::thread c(cons, std::ref(buf));
    
    std::cout << "Thread running..." << std::endl;
    
    p.join();
    c.join();
    
    //read user flag in buffer
    
    std::cout << "all threads terminated" << endl;
    
    
    std::cout << "hello" << std::endl;
    return 0;
}
