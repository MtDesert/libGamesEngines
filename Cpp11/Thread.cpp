#include "Thread.h"
#include<stdio.h>

Thread::Thread():threadFunction(NULL),threadArguments(NULL),errorNumber(0),whenThreadError(NULL){
#ifdef __MINGW32__
	threadID.p=NULL;threadID.x=0;
#else
	threadID=0;
#endif
}
Thread::~Thread(){}

#define THREAD_CHECK_ERROR(code) \
errorNumber=(code);\
if(errorNumber){\
	if(whenThreadError)whenThreadError(this);\
}

void* Thread::threadStart(void *threadPtr){
	auto thrd=reinterpret_cast<Thread*>(threadPtr);
	thrd->threadFunction(thrd->threadArguments);//执行线程函数
#ifdef __MINGW32__
	thrd->threadID.p=nullptr;
#else
	thrd->threadID=0;
#endif
	return nullptr;
}

void Thread::start(void* (*threadFunc)(void*),void *arguments){
#ifdef __MINGW32__
	if(threadID.p)return;
#else
	if(threadID)return;
#endif
	threadFunction=threadFunc;
	threadArguments=arguments;
	//启动线程
	create(threadStart,this);
}

void Thread::create(void *(*threadFunc)(void *),void *arguments){
	THREAD_CHECK_ERROR(::pthread_create(&threadID,NULL,threadFunc,arguments))
}
void Thread::detach(){
	THREAD_CHECK_ERROR(::pthread_detach(threadID))
}
int Thread::join(){return pthread_join(threadID,NULL);}