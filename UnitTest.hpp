//
//  UnitTest.hpp
//  RemoteDesktopProgram
//
//  Created by Zhanan Ao on 21/02/2021.
//

#ifndef UnitTest_hpp
#define UnitTest_hpp

#include <iostream>
#include "MotorDrive.hpp"
#include "Consumer.hpp"
#include "Producer.hpp"
#include "Buffer.hpp"
#include "Database.hpp"


void versionOne();

void testDriveClass();

void testReverse();

void debugConversion();

void testDatabase();

void fakeProducer(Buffer& b);

void fakeV1();

void fakeV2();

void versionTwo();

void testM();


#endif /* UnitTest_hpp */
