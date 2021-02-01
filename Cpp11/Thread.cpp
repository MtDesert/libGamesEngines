#include "Thread.h"
#include<stdio.h>

#ifdef __i386
#define CLEAR_PID threadID.p=NULL;
#else
#define CLEAR_PID threadID=0;
#endif

Thread::Thread():threadFunction(NULL),threadArguments(NULL),errorNumber(0),whenThreadError(NULL){
	CLEAR_PID;
}
Thread::~Thread(){}

#define THREAD_CHECK_ERROR(code) \
errorNumber=(code);\
if(errorNumber){\
	if(whenThreadError)whenThreadError(this);\
	return false;\
}

void* Thread::threadStart(void *threadPtr){
	auto thrd=reinterpret_cast<Thread*>(threadPtr);
	thrd->threadFunction(thrd->threadArguments);//执行线程函数
	thrd->CLEAR_PID;
	return nullptr;
}

bool Thread::start(void* (*threadFunc)(void*),void *arguments){
	if(isRunning())return false;
	threadFunction=threadFunc;
	threadArguments=arguments;
	//启动线程
	return create(threadStart,this);
}

bool Thread::create(void *(*threadFunc)(void *),void *arguments){
	THREAD_CHECK_ERROR(::pthread_create(&threadID,NULL,threadFunc,arguments))
	return true;
}
bool Thread::detach(){
	THREAD_CHECK_ERROR(::pthread_detach(threadID))
	return true;
}
int Thread::join(){return pthread_join(threadID,NULL);}

bool Thread::isRunning()const{
#ifdef __i386
	return threadID.p;
#else
	return threadID;
#endif
}