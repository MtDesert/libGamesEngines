#include "Socket.h"
#include<arpa/inet.h>

#define SOCKET_CONNECT_ARGUMENTS \
descriptor,(const sockaddr*)&socketAddress,sizeof(socketAddress)

#define SOCKET_CHECK_ERROR(code) \
errorNumber = ((code)==-1 ? errno :0);\
if(errorNumber && whenError)whenError();

IPAddress::IPAddress(uint32 addr){address.s_addr=addr;}
IPAddress::IPAddress(const char *str){setAddress(str);}
IPAddress::IPAddress(const string &str){setAddress(str);}
Socket::Socket():descriptor(0){}

void IPAddress::setAddress(const char *str){address.s_addr=inet_addr(str);}
void IPAddress::setAddress(const string &str){setAddress(str.data());}
char *IPAddress::toString()const{return inet_ntoa(address);}
string IPAddress::toStdString()const{return toString();}

void Socket::connect(const IPAddress &ipAddress,uint16 port){
	if(!createSocket())return;//创建套接字
	setSocketAddress(ipAddress,port);//设置连接数据
	//线程
	thread.whenError=whenError;//设定出错函数
	thread.start(Socket::connect,this);//开始连接
}

void Socket::bind(const IPAddress &ipAddress,uint16 port){
	setSocketAddress(ipAddress,port);
	SOCKET_CHECK_ERROR(::bind(SOCKET_CONNECT_ARGUMENTS));
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
SOCKET_CPP_IPADDRESS_PORT_ALL(bind)

bool Socket::createSocket(){
	if(descriptor>0)return true;
	//创建socket
	descriptor=::socket(AF_INET,SOCK_STREAM,0);
	SOCKET_CHECK_ERROR(descriptor)
	return errorNumber==0;
}
void Socket::setSocketAddress(const IPAddress &ipAddress,uint16 port){
	socketAddress.sin_family=AF_INET;
	socketAddress.sin_addr=ipAddress.address;
	socketAddress.sin_port=htons(port);
}
void Socket::connect(){
	SOCKET_CHECK_ERROR(::connect(SOCKET_CONNECT_ARGUMENTS));
	if(!errorNumber && whenConnected)whenConnected();
}

void* Socket::connect(void *arg){
	((Socket*)arg)->connect();
	return NULL;
}