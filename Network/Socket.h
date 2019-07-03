#ifndef SOCKET_H
#define SOCKET_H

#include<sys/socket.h>
#include<pthread.h>

//套接字,原意为"插座"
class Socket{
public:
	Socket();
	//连接特定地址
	void connect();
private:
	void makeDescriptor();
	//变量
	int descriptor;//socket描述符
	sockaddr socketAddress;
	pthread_t threadID;//线程,用于处理异步连接
	//线程函数
#define SOCKET_THREAD_FUNC(name) static void* name(void *pSocket);
	SOCKET_THREAD_FUNC(socketConnect)
};
#endif