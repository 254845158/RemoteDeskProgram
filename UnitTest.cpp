//
//  UnitTest.cpp
//  RemoteDesktopProgram
//
//  Created by Zhanan Ao on 21/02/2021.
//

#include "UnitTest.hpp"

void versionOne() {
    Buffer buf;
    MotorDrive motorDrive;
    motorDrive.establishConnection();

    std::thread p(producer, std::ref(motorDrive), std::ref(buf));
    std::thread c(cons, std::ref(buf));
    std::cout << "Thread running..." << std::endl;

    //read user flag in buffer
    char ca;
    std::cout << "Enter q to stop program" << std::endl;
    std::cin >> ca;
    while (1) {
        if (ca == 'q') {
            buf.toggleUserFlag();
            break;
        }
    }

    p.join();
    c.join();

}



void testReverse() {
    uint16_t test[2] = {1,2};
    std::cout << "test[0] = " << test[0] << "   test[1] = " << test[1] << std::endl;
    reverse(test);
    std::cout << "test[0] = " << test[0] << "   test[1] = " << test[1] << std::endl;
}

void debugConversion() {
    MotorDrive motorDrive;
    motorDrive.establishConnection();

    //read user flag in buffer
    char ca;
    int reg;
    while (1) {
        std::cout << "R to read or q to quit" << std::endl;
        std::cin >> ca;
        if (ca == 'q') {
            break;
        }
        std::cout << "Register to read" << std::endl;
        std::cin >> reg;
        std::cout << "Read value = " << motorDrive.read32BitRegister<float>(reg, 3) << std::endl;
    }
}

void testDriveClass() {
    MotorDrive motorDrive;
    motorDrive.establishConnection();
    
    try {
        std::cout << "16bit read reg 0 : " << motorDrive.read16BitRegister(0) << std::endl;
        
        uint16_t* result;
        result = new uint16_t[10];
        
        motorDrive.readConsecutive16BitRegisters(0, 10, result);
        for (int i = 0; i<10; i++) {
            std::cout<< "register " << i << " = " << result[i] << std::endl;
        }
        
        std::cout << "32bit read reg 10 : " << motorDrive.read32BitRegister<float>(10, 3) << std::endl;
        
    }
    catch (std::runtime_error& e) {
        std::cout << e.what() << std::endl;
    }
}
