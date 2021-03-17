//
//  Producer.hpp
//  RemoteDesktopProgram
//
//  Created by Zhanan Ao on 22/02/2021.
//

#ifndef Producer_hpp
#define Producer_hpp

#include <stdio.h>
#include "MotorDrive.hpp"
#include <thread>
#include <chrono>
#include "Buffer.hpp"

#define OUTPUT_VOLTAGE 501
#define OUTPUT_FREQUENCY 500
#define OUTPUT_CURRENT 400
#define TORQUE_CURRENT 401

void producer(MotorDrive& motorDrive,Buffer& b);

#endif /* Producer_hpp */
