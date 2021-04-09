//
//  Consumer.hpp
//  RemoteDesktopProgram
//
//  Created by Zhanan Ao on 28/02/2021.
//

#ifndef Consumer_hpp
#define Consumer_hpp

#include <iomanip>
#include <fstream>
#include "Buffer.hpp"
#include "Database.hpp"
using namespace std;
#define Number2Write 500

void write2CSV(float data[buffern], long long timestamp);

void cons(Buffer& b);



#endif /* Consumer_hpp */
