#include "Socket.h"
#include<errno.h>

void* Socket::socketConnect(void *pSocket){
	auto skt=(Socket*)pSocket;
	::connect(skt->descriptor,&skt->socketAddress,sizeof(skt->socketAddress));
	return NULL;
}

Socket::Socket():descriptor(0){}

void Socket::connect(){
	//创建
	makeDescriptor();
	//线程处理连接
	pthread_create(&threadID,NULL,socketConnect,this);
}
void Socket::makeDescriptor(){
	if(!descriptor){
		descriptor=::socket(AF_INET,SOCK_STREAM,0);
	}
}