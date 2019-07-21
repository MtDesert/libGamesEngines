#include "Socket.h"
#include<unistd.h>
#ifndef __MINGW32__
#include<arpa/inet.h>
#endif

#define SOCKET_CONNECT_ARGUMENTS \
descriptor,(const sockaddr*)&socketAddress,sizeof(socketAddress)

#define SOCKET_WHEN_CALLBACK(name)\
if(when##name)when##name();\
if(whenSocket##name)whenSocket##name(this);

//检查错误,如果有错误则直接调用错误处理函数并返回
#define SOCKET_CHECK_ERROR(code) \
errorNumber = ((code)==-1 ? errno :0);\
if(errorNumber){\
SOCKET_WHEN_CALLBACK(Error)\
return;\
}

#define SOCKET_CHECK_ERRNO(code,correctErrno,successName) \
errorNumber = ((code)==-1 ? errno :0);\
if(errorNumber == correctErrno){/*运行状态*/\
	pthread_yield();\
}else{/*完成状态*/\
	command=Command_None;\
	if(errorNumber){/*完成,但失败*/\
		SOCKET_WHEN_CALLBACK(Error)\
	}else{/*完成,成功*/\
		SOCKET_WHEN_CALLBACK(successName)\
	}\
}

IPAddress::IPAddress(uint32 addr){address.s_addr=addr;}
IPAddress::IPAddress(const char *str){setAddress(str);}
IPAddress::IPAddress(const string &str){setAddress(str);}

#define SOCKET_WHEN(name) when##name(NULL),whenSocket##name(NULL)
Socket::Socket():descriptor(0),newAcceptedSocket(NULL),command(Command_None),errorNumber(0),
	SOCKET_WHEN(Error),//错误处理
	SOCKET_WHEN(Connected),//主动连接成功
	SOCKET_WHEN(Accepted),//被动连接成功
	SOCKET_WHEN(Sent),//数据发送
	SOCKET_WHEN(Received)//数据接收
{}

void IPAddress::setAddress(const char *str){address.s_addr=inet_addr(str);}
void IPAddress::setAddress(const string &str){setAddress(str.data());}
char *IPAddress::toString()const{return inet_ntoa(address);}
string IPAddress::toStdString()const{return toString();}

#define SOCKET_SET_ADDRESS \
if(descriptor<=0){\
	descriptor=::socket(AF_INET,SOCK_STREAM|SOCK_NONBLOCK,0);\
	SOCKET_CHECK_ERROR(descriptor)\
}\
setSocketAddress(ipAddress,port);/*设置连接数据*/\
thread.whenError=whenError;/*设定出错函数*/

void Socket::connect(const IPAddress &ipAddress,uint16 port){
	SOCKET_SET_ADDRESS
	command=Command_Connect;
	thread.start(Socket::commandLoop,this);//开始连接
}

void Socket::accept(const IPAddress &ipAddress,uint16 port){
	SOCKET_SET_ADDRESS
	SOCKET_CHECK_ERROR(::bind(SOCKET_CONNECT_ARGUMENTS));
	SOCKET_CHECK_ERROR(::listen(descriptor,0xFFFF));
	thread.start(Socket::accept,this);//开始连接
}

#define SOCKET_CPP_IPADDRESS_PORT(name,type) \
void Socket::name(type ipAddress,uint16 port){\
	name(IPAddress(ipAddress),port);\
}

#define SOCKET_CPP_IPADDRESS_PORT_ALL(name) \
SOCKET_CPP_IPADDRESS_PORT(name,const string&)\
SOCKET_CPP_IPADDRESS_PORT(name,const char*)\
SOCKET_CPP_IPADDRESS_PORT(name,uint32)

SOCKET_CPP_IPADDRESS_PORT_ALL(connect)
SOCKET_CPP_IPADDRESS_PORT_ALL(accept)

void Socket::listenPort(uint16 port){accept((uint32)0,port);}
Socket* Socket::acceptedSocket()const{return newAcceptedSocket;}

void Socket::setSocketAddress(const IPAddress &ipAddress,uint16 port){
	socketAddress.sin_family=AF_INET;
	socketAddress.sin_addr=ipAddress.address;
	socketAddress.sin_port=htons(port);
}

void Socket::commandLoop(){
	//循环收发
	while(command!=Command_Close){
		switch(command){
			case Command_Connect:{
				SOCKET_CHECK_ERRNO(::connect(SOCKET_CONNECT_ARGUMENTS),EINPROGRESS,Connected)
			}break;
			case Command_Send:{
				::send(descriptor,sendData.addr,sendData.size,0);
				command=Command_None;
			}break;
			case Command_Receive:{
				SOCKET_CHECK_ERRNO(::recv(descriptor,recvData.addr,recvData.size,0),EAGAIN,Received)
			}break;
			//啥也没干
			default:pthread_yield();
		}
	}
	//循环结束
	::shutdown(descriptor,SHUT_RDWR);
	if(newAcceptedSocket==this)delete this;//由accept函数new出来的
}
void Socket::accept(){
	while(command!=Command_Close){//不断监听连接
		socklen_t len=sizeof(socketAddress);
		int fd=::accept(descriptor,(sockaddr*)&socketAddress,&len);//等待连接
		if(fd==-1){
			if(errno==EAGAIN){
				pthread_yield();
			}else{
				command=Command_Close;
				SOCKET_WHEN_CALLBACK(Error)
			}
		}else{
			//新socket
			auto newSocket=new Socket();
			newSocket->descriptor=fd;//描述符
			newSocket->socketAddress=socketAddress;//套接地址
			newSocket->newAcceptedSocket=newSocket;//参考commondLoop的回收部分
			//回调函数
			newSocket->whenSocketSent=whenSocketSent;
			newSocket->whenSocketReceived=whenSocketReceived;
			//连接成功
			newAcceptedSocket=newSocket;
			SOCKET_WHEN_CALLBACK(Accepted)//连接成功
			newAcceptedSocket=NULL;
			//启动收发线程
			newSocket->thread.start(Socket::commandLoop,newSocket);
		}
	}
	//循环结束
	::shutdown(descriptor,SHUT_RDWR);
}
//pthread线程函数
void* Socket::commandLoop(void *socket){
	((Socket*)socket)->commandLoop();
	return NULL;
}
void* Socket::accept(void *socket){
	((Socket*)socket)->accept();
	return NULL;
}

//收发数据
void Socket::send(const void *buffer,size_t size){
	sendData.addr=(void*)buffer;
	sendData.size=size;
	command=Command_Send;
}
void Socket::receive(void *buffer,size_t size){
	recvData.addr=buffer;
	recvData.size=size;
	command=Command_Receive;
}
//关闭连接
void Socket::close(){command=Command_Close;}