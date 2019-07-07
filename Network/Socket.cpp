#include "Socket.h"
#include<arpa/inet.h>

IPAddress::IPAddress(uint32 addr){address.s_addr=addr;}
IPAddress::IPAddress(const char *str){setAddress(str);}
IPAddress::IPAddress(const string &str){setAddress(str);}
Socket::Socket():descriptor(0){}

void IPAddress::setAddress(const char *str){address.s_addr=inet_addr(str);}
void IPAddress::setAddress(const string &str){setAddress(str.data());}
char *IPAddress::toString()const{return inet_ntoa(address);}
string IPAddress::toStdString()const{return toString();}

void Socket::connect(const IPAddress &ipAddress,uint16 port){
	//创建套接字
	if(!createSocket())return;
	//设置连接数据
	socketAddress.sin_family=AF_INET;
	socketAddress.sin_addr=ipAddress.address;
	socketAddress.sin_port=htons(port);
	//线程
	thread.whenError=whenError;//设定出错函数
	thread.start(Socket::connect,this);//开始连接
}
void Socket::connect(const string &ipAddress,uint16 port){
	connect(IPAddress(ipAddress),port);
}
void Socket::connect(const char *ipAddress,uint16 port){
	connect(IPAddress(ipAddress),port);
}
void Socket::connect(uint32 ipAddress, uint16 port){
	connect(IPAddress(ipAddress),port);
}

bool Socket::createSocket(){
	if(descriptor>0)return true;
	//创建socket
	descriptor=::socket(AF_INET,SOCK_STREAM,0);
	errorNumber=(descriptor==-1 ? errno : 0);
	return checkError();
}

bool Socket::checkError(){
	if(errorNumber){
		if(this->whenError)this->whenError();
		return false;
	}
	return true;
}

void* Socket::connect(void *arg){
	auto socket=(Socket*)arg;
	auto ret=::connect(socket->descriptor,(const sockaddr*)&socket->socketAddress,sizeof(socket->socketAddress));
	socket->errorNumber=(ret==-1 ? errno :0);
	socket->checkError();
	return NULL;
}