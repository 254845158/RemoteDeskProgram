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

using namespace std;
#define Number2Write 500

void write2CSV(std::vector<float> data);

void cons(Buffer& b);



#endif /* Consumer_hpp */
