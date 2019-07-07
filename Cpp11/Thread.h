#ifndef THREAD_H
#define THREAD_H

#include<pthread.h>

class Thread{
	pthread_t threadID;//线程ID
public:
	Thread();

	void start(void* (*threadFunction)(void*),void *arguments);

	//状态
	int errorNumber;//错误号
	//回调函数
	void (*whenError)();
};

#endif