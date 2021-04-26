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
    float syncSpeed, syncSpeedrps;
    float slip;
    //uint16_t output_voltage;
//    float output_frequency, torque_current;
//    int output_current;
    while (b.readUserFlag()) {
        try {
            //collect data from drive - only if motor is running i.e. there is output voltage
            data[3] = motorDrive.read16BitRegister(OUTPUT_VOLTAGE);
            data[3] = data[3] / sqrt(3);        //convert the phase rms
            std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
            data[2] = motorDrive.read32BitRegister<float>(OUTPUT_CURRENT, 3);
            std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
            std::cout << "output voltage = " << data[3] << std::endl;
           // std::cout << "output current = " << data[1] << std::endl;
            if (data[3] > 0) {
                data[0] = motorDrive.read32BitRegister<float>(OUTPUT_FREQUENCY, 3);
                std::cout << "output frequency = " << data[0] << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
                data[5] = motorDrive.read32BitRegister<float>(TORQUE_CURRENT, 3);
                std::cout << "output torque current = " << data[5] << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
           
                //speed
                data[1] = motorDrive.read32BitRegister<float>(ROTATINAL_SPEED, 3);;    //dummy data

                //calculated torque
                syncSpeed = (data[0] * 60.0f) / 2.0f;
                slip = (syncSpeed - data[1]) / syncSpeed;
                syncSpeedrps = syncSpeed * (2.0f * 3.14159236 / 60.0f);
                data[4] = (3.0f / (slip * syncSpeedrps)) * (pow(data[3], 2) * 0.86) * (1 / (pow(0.83 + (0.86 / slip), 2) + pow(0.009593, 2)));

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
