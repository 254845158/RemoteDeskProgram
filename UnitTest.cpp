//
//  UnitTest.cpp
//  RemoteDesktopProgram
//
//  Created by Zhanan Ao on 21/02/2021.
//

#include "UnitTest.hpp"

void fakeProducer(Buffer& b) {
    long long timestamp;
    float data[buffern];
    int sleepTime = 10;
    float syncSpeed, syncSpeedrps;
    float slip;
    //uint16_t output_voltage;
//    float output_frequency, torque_current;
//    int output_current;
    while (b.readUserFlag()) {
        try {
            //collect data from drive - only if motor is running i.e. there is output voltage
            data[3] = 350;
            data[3] = data[3] / sqrt(3);
            std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
            data[2] = rand() % 100;
            std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));

            if (data[3] > 0) {
                data[0] = 50.0f;
                std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
                data[5] = 0.5f;
                
                std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
                data[1] = 1495.0f;    //dummy data
                //speed

                //calculated torque
                syncSpeed = (data[0] * 60.0f) / 2.0f;
                slip = (syncSpeed - data[1]) / syncSpeed;
                syncSpeedrps = syncSpeed * (2.0f * 3.14159236 / 60.0f);
                data[4] = (3.0f /(slip*syncSpeedrps)) *(pow(data[3], 2) * 0.86) * (1/(pow(0.83+(0.86/slip),2)+pow(0.009593,2)));

                //timeStamp
                timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                //std::cout << "time stamp = " << timestamp << std::endl;
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

/*void fakeV1() {
    Buffer buf;

    std::thread p(fakeProducer,std::ref(buf));
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
}*/

void fakeV2() {
    Buffer buf;
    //set the motor id and its params

    std::thread p(fakeProducer, std::ref(buf));
    std::thread c(cons, std::ref(buf));
    std::cout << "Thread running..." << std::endl;

    //read user flag in buffer
    char ca;
    std::cout << "Enter q to stop program" << std::endl;
 
    while (1) {
        std::cin >> ca;
        if (ca == 'q') {
            buf.toggleUserFlag();
            break;
        }
    }

    p.join();
    c.join();
    std::cout << "Database connection closed" << std::endl;

}

void versionTwo() {
    Buffer buf;
    MotorDrive motorDrive;
    motorDrive.establishConnection();

    std::thread p(producer, std::ref(motorDrive), std::ref(buf));
    std::thread c(cons, std::ref(buf));
    std::cout << "Thread running..." << std::endl;

    //read user flag in buffer
    char ca;

    while (1) {
        std::cout << "Enter q to stop program" << std::endl;
        std::cin >> ca;
        if (ca == 'q') {
            buf.toggleUserFlag();
            break;
        }
    }

    p.join();
    c.join();

}

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

void testDatabase() {
    std::cout << "hello mysql" << std::endl;
    Database d;
    int motorID = 0;
    if (d.establishConnection() == 0) {
        std::cout << "Database Connected" << std::endl;
    }
    
    float data[4] = { 1.0f,2.0f,3.0f,4.0f };
    if (d.insertMotor(motorID, data, 4)==0) {
        std::cout << "insert to motor table success" << std::endl;
    }
    if (d.insertDataInstance(motorID, 1234) == 0) {
        std::cout << "insert to dataInstance table success" << std::endl;
    }
    
    //problem cannot read last id properly, insertion is fine
    long long a = d.lastID();
    std::cout << "lastID = " << a << std::endl;
    float data2[6] = { 1.3,2.2,3.1,4.3,5,6 };
    float* f = data2;
    if (d.insertMeasuredData(f, 6) == 0) {
        std::cout << "insert to measuredData table success" << std::endl;
    }
    
    d.closeConnection();
    std::cout << "Database connection closed" << std::endl;
    std::cout << "Type anything and enter to quit" << std::endl;
    char q;
    std::cin >> q;
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

void testM() {
    
        float result = 4.0 / sqrt(3);
        std::cout << result << std::endl;
    
}
