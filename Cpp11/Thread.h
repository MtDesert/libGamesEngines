#ifndef THREAD_H
#define THREAD_H

#include<pthread.h>

/**基于pthread的线程类,可以设定错误回调函数,以便在出错的时候进行处理
与C++11的std::thread不同,std::thread在对象创建的时候就开始启动线程
而Thread可以先创建,然后在需要的时候再调用start即可*/
class Thread{
	pthread_t threadID;//线程ID
public:
	Thread();
	~Thread();

	void start(void* (*threadFunction)(void*),void *arguments);//线程开始(线程函数,参数)

	//状态
	int errorNumber;//错误号
	//回调函数
	void (*whenError)();//出错时候调用此函数
	void (*whenThreadError)(Thread *thread);//出错时,如果需要读写本对象,请设置此函数
};

#endif