#include "Thread.h"
#include<errno.h>

Thread::Thread():threadID(0),errorNumber(0),whenError(NULL),whenThreadError(NULL){}
Thread::~Thread(){}

void Thread::start(void* (*threadFunction)(void *),void *arguments){
	//启动线程
	errorNumber = ::pthread_create(&threadID,NULL,threadFunction,arguments);
	//错误处理
	if(errorNumber){
		if(whenError)whenError();
		if(whenThreadError)whenThreadError(this);
	}
}