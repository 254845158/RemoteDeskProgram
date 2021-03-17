//
//  MotorDrive.hpp
//  RemoteDesktopProgram
//
//  Created by Zhanan Ao on 19/02/2021.
//

#ifndef MotorDrive_hpp
#define MotorDrive_hpp

#include <iostream>
#include <modbus-rtu.h>
#include <stdexcept>
#include <string>

void reverse(uint16_t* p);

class MotorDrive {
private:
    modbus_t* mbConnection;
public:
    //function to establish modbus connection
    bool establishConnection();
    
    //function to select slave
    int selectSlave();
    
    //function to read a single register
    uint16_t read16BitRegister(int reg);
    
    //function to read consecutive registers
    int readConsecutive16BitRegisters(int reg,int N,uint16_t* result);
    
    //function to read a single 32 bit register
    template<class T>
    T read32BitRegister(int reg,int type);
    
    //function to read consecutive 32 bit register
    
    //function to write a single register
    int write16BitRegister(int reg, uint16_t value);
    
};  //end of MotorDrive class

#endif /* MotorDrive_hpp */


