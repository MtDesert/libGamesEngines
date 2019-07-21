#include "Thread.h"
#include<stdio.h>

Thread::Thread():
#ifndef __MINGW32__
threadID(0),
#endif
errorNumber(0),whenError(NULL),whenThreadError(NULL){}
Thread::~Thread(){}

#define THREAD_CHECK_ERROR(code) \
errorNumber=(code);\
if(errorNumber){\
	if(whenError)whenError();\
	if(whenThreadError)whenThreadError(this);\
}

void Thread::start(void* (*threadFunction)(void *),void *arguments){
	if(threadID)return;
	//启动线程
	THREAD_CHECK_ERROR(::pthread_create(&threadID,NULL,threadFunction,arguments))
	//分离线程
	else{
		THREAD_CHECK_ERROR(::pthread_detach(threadID))
	}
}

void Thread::create(void *(*threadFunction)(void *), void *arguments){
	THREAD_CHECK_ERROR(::pthread_create(&threadID,NULL,threadFunction,arguments))
}
void Thread::detach(){
	THREAD_CHECK_ERROR(::pthread_detach(threadID))
}
int Thread::join(){return pthread_join(threadID,NULL);}
int Thread::cancel(){return pthread_cancel(threadID);}