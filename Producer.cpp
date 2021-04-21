//
//  Producer.cpp
//  RemoteDesktopProgram
//
//  Created by Zhanan Ao on 22/02/2021.
//

#include "Producer.hpp"


void producer(MotorDrive& motorDrive,Buffer& b) {
    float data[buffern];
    long long timestamp;
    int sleepTime = 20;
    //uint16_t output_voltage;
//    float output_frequency, torque_current;
//    int output_current;
    while (b.readUserFlag()) {
        try {
            //collect data from drive - only if motor is running i.e. there is output voltage
            data[0] = motorDrive.read16BitRegister(OUTPUT_VOLTAGE);
            std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
            data[1] = motorDrive.read32BitRegister<float>(OUTPUT_CURRENT, 3);
            std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
            std::cout << "output voltage = " << data[0] << std::endl;
            std::cout << "output current = " << data[1] << std::endl;
            if (data[0] > 0) {
                data[2] = motorDrive.read32BitRegister<float>(OUTPUT_FREQUENCY, 3);
                std::cout << "output frequency = " << data[2] << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
                data[3] = motorDrive.read32BitRegister<float>(TORQUE_CURRENT, 3);
                std::cout << "output torque current = " << data[3] << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
                
                //speed
                data[4] = 0;    //dummy data
                //timeStamp
                timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                //put data to buffer
                b.put(data, timestamp);   //line just for debug

            }
            else {
                //sleep
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }
        }
        catch (std::runtime_error& e) {
            std::cout << e.what() << std::endl;
        }
        
    }
    //call terminate function
    b.terminate();

    std::cout << "Producer terminated..." << std::endl;
}
