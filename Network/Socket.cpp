#include "Socket.h"
#ifndef __MINGW32__
#include<arpa/inet.h>
#endif

#define SOCKET_CONNECT_ARGUMENTS \
descriptor,(const sockaddr*)&socketAddress,sizeof(socketAddress)

#define SOCKET_WHEN_CALLBACK(name)\
if(when##name)when##name();\
if(whenSocket##name)when##name();

#define SOCKET_CHECK_ERROR(code) \
errorNumber = ((code)==-1 ? errno :0);\
if(errorNumber){SOCKET_WHEN_CALLBACK(Error)}

IPAddress::IPAddress(uint32 addr){address.s_addr=addr;}
IPAddress::IPAddress(const char *str){setAddress(str);}
IPAddress::IPAddress(const string &str){setAddress(str);}

#define SOCKET_WHEN(name) when##name(NULL),whenSocket##name(NULL)
Socket::Socket():descriptor(0),errorNumber(0),
	SOCKET_WHEN(Error),//错误处理
	SOCKET_WHEN(Connected),//主动连接成功
	SOCKET_WHEN(Accepted)//被动连接成功
{}

void IPAddress::setAddress(const char *str){address.s_addr=inet_addr(str);}
void IPAddress::setAddress(const string &str){setAddress(str.data());}
char *IPAddress::toString()const{return inet_ntoa(address);}
string IPAddress::toStdString()const{return toString();}

#define SOCKET_SET_ADDRESS \
if(!createSocket())return;/*创建套接字*/\
setSocketAddress(ipAddress,port);/*设置连接数据*/\
thread.whenError=whenError;/*设定出错函数*/

void Socket::connect(const IPAddress &ipAddress,uint16 port){
	SOCKET_SET_ADDRESS
	thread.start(Socket::connect,this);//开始连接
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

void Socket::accept(uint16 port){accept((uint32)0,port);}

bool Socket::createSocket(){
	if(descriptor>0)return true;
	//创建socket
	descriptor=::socket(AF_INET,SOCK_STREAM,0);//默认使用TCP
	SOCKET_CHECK_ERROR(descriptor)
	return errorNumber==0;
}
void Socket::setSocketAddress(const IPAddress &ipAddress,uint16 port){
	socketAddress.sin_family=AF_INET;
	socketAddress.sin_addr=ipAddress.address;
	socketAddress.sin_port=htons(port);
}

void Socket::connect(){
	SOCKET_CHECK_ERROR(::connect(SOCKET_CONNECT_ARGUMENTS));//连接过程
	if(!errorNumber){//连接成功
		SOCKET_WHEN_CALLBACK(Connected)
	}
}
void Socket::accept(){
	while(true){//不断监听连接
		socklen_t len=sizeof(socketAddress);
		int fd=::accept(descriptor,(sockaddr*)&socketAddress,&len);//等待连接
		SOCKET_CHECK_ERROR(fd)
		if(!errorNumber){//接受连接成功
			SOCKET_WHEN_CALLBACK(Accepted)
		}
	}
}
//pthread线程函数
void* Socket::connect(void *socket){
	((Socket*)socket)->connect();
	return NULL;
}
void* Socket::accept(void *socket){
	((Socket*)socket)->accept();
	return NULL;
}