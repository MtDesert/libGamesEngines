#ifndef SYSTIME_H
#define SYSTIME_H

#include<sys/time.h>
class SysTime{
public:
	SysTime();
	int usecElapsed();
protected:
	timeval oldTime;
};

#endif