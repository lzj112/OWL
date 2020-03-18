#ifndef LOG_H_
#define LOG_H_

#include <stdio.h>

#include <fstream>
#include <iostream>
using namespace std;
 

#define ERROR(arg...) {\
printf("\e[1;31m[ERROR]:%s:%s:%d\e[0m", __FILE__, __FUNCTION__, __LINE__);\
printf(arg);\
fflush(stdout);\
}  

#define WARN(arg...) {\
printf("\e[1;33m[WARN]\e[0m");\
printf(arg);\
fflush(stdout);\
}  

#define INFO(arg...) {\
printf("\e[1;42m[INFO]\e[0m");\
printf(arg);\
fflush(stdout);\
}

#endif