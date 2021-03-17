//
//  MotorDrive.cpp
//  RemoteDesktopProgram
//
//  Created by Zhanan Ao on 19/02/2021.
//

#include "MotorDrive.hpp"

bool MotorDrive:: establishConnection(){
    char portName[255];
    int baudRate;
    int stopBit;

    while (1){
        std::cout << "Select the port to communicate with by entering the name i.e. COM2" << std::endl;

        std::cin >> portName;
        
        std::cout << "Select the baud rate to communicate with (9.6k,19.2k,38.4k,57.6k,115.2k)" << std::endl;

        std::cin >> baudRate;
        
        std::cout << "Select the number of stop bits (1 or 2)" << std::endl;

        std::cin >> stopBit;

        this -> mbConnection = modbus_new_rtu(portName, baudRate, 'N', 8, stopBit);
        
        if (modbus_connect(this->mbConnection) == 0) {
            std::cout << "Serial connection established" << std::endl;
            break;
        }
        else {
            std::cout << "Connection failed: " << modbus_strerror(errno) << std::endl;
            continue;
        }
    }
    
    while(1) {
        if(selectSlave() == 0) {
            break;
        }
        else {
            continue;
        }
    }

    return true;
}

int MotorDrive::selectSlave() {
    int id;
    std::cout << "Select the slave you wish to communicate with (1-247)" << std::endl;
    std::cin >> id;
    
    if (modbus_set_slave(this->mbConnection, id) == -1) {
        std::cout << "Slave connection failed: " << modbus_strerror(errno) << std::endl;
        return -1;
    }
    else {
        //****also check by reading a register
        try{
            std::cout << "Connected with baud mode : " << this->read16BitRegister(35) << std::endl;
        }
        catch (std::runtime_error& e) {
            std::cout << e.what() << std::endl;
            return -1;
        }
        std::cout << "Slave connection success" << std::endl;
        return 0;
    }
}

uint16_t MotorDrive::read16BitRegister(int reg) {
    uint16_t result[1];
    if (modbus_read_registers(this->mbConnection, reg, 1, result) == -1) {
        //error in reading
        const std::string error("error occured while reading 16 bit register");
        throw std::runtime_error(error);
    }
    else {
        return result[0];
    }
}

int MotorDrive::readConsecutive16BitRegisters(int reg, int N, uint16_t *result) {
    if (modbus_read_registers(this->mbConnection, reg, N, result) == -1) {
        //error in reading
        const std::string error("error occured while reading 16bit consec register");
        throw std::runtime_error(error);
    }
    else {
        return 0;
    }
}

template<class T>
T MotorDrive::read32BitRegister(int reg, int type) {
    int newReg;
    if (type == 3) {
        //float selected
        newReg = reg + 32768;
        uint16_t result[2];
        this->readConsecutive16BitRegisters(newReg, 2, result);
        std::cout << "32 bit upper = " << result[0] << std::endl;
        std::cout << "32 bit lower = " << result[1] << std::endl;
        reverse(result);
        std::cout << "reversed 32 bit upper = " << result[0] << std::endl;
        std::cout << "reversed 32 bit lower = " << result[1] << std::endl;
        T combinedResult;
        memcpy(&combinedResult, &result, sizeof(float));
        std::cout << "32 bit upper = " << result[0] << std::endl;
        std::cout << "combined result = " << combinedResult << std::endl;
        return combinedResult;
    }
    else if(type == 2) {
        //int selected
        newReg = reg + 16384;
        uint16_t result[2];
        this->readConsecutive16BitRegisters(newReg, 2, result);
        reverse(result);
        T combinedResult;
        memcpy(&combinedResult, &result, sizeof(int));
        return combinedResult;
    }
    else {
        return read16BitRegister(reg);
    }
}

int MotorDrive::write16BitRegister(int reg, uint16_t value) {
    if (modbus_write_register(this->mbConnection, reg, value) == 1) {
        return 0;
    }
    else {
        return -1;
    }
}

void reverse (uint16_t *p) {
    uint16_t temp = *p;
    *p = *(p+1);
    *(p+1) = temp;
}

void temp() {
    MotorDrive m;
    m.read32BitRegister<int>(0, 2);
    m.read32BitRegister<float>(0, 3);
}




