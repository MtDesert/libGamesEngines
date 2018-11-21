#include"SysTime.h"

SysTime::SysTime(){gettimeofday(&oldTime,nullptr);}

int SysTime::usecElapsed(){
	timeval newTime;
	gettimeofday(&newTime,nullptr);
	if(newTime.tv_usec < oldTime.tv_usec){
		newTime.tv_usec += 1000000;
		newTime.tv_sec -= 1;
	}
	int ret=(newTime.tv_sec-oldTime.tv_sec)*1000000+(newTime.tv_usec-oldTime.tv_usec);
	oldTime=newTime;
	return ret;
}