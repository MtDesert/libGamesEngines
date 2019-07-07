#include "Thread.h"
#include<errno.h>

Thread::Thread():threadID(0),errorNumber(0){}

void Thread::start(void* (*threadFunction)(void *),void *arguments){
	errorNumber = ::pthread_create(&threadID,NULL,threadFunction,arguments);
	//错误处理
	if(errorNumber){
		if(whenError)whenError();
	}
}