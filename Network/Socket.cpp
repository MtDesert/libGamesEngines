#include "Socket.h"
#include<unistd.h>

#ifdef __MINGW32__ //MinGW编译环境
#include<ws2tcpip.h>
#define SOCK_NONBLOCK 0
#define EINPROGRESS WSAEWOULDBLOCK
#define ERR_NO WSAGetLastError()
#define PTHREAD_YIELD
#define SOCKET_SHUTDOWN ::close(descriptor);
static WSADATA wsaData;
static bool wsaStartedUp=false;
#else //Linux默认
#include<arpa/inet.h>
#define ERR_NO errno
#define PTHREAD_YIELD pthread_yield();
#define SOCKET_SHUTDOWN ::shutdown(descriptor,SHUT_RDWR);
#endif

#define SOCKET_CONNECT_ARGUMENTS \
descriptor,(const sockaddr*)&socketAddress,sizeof(socketAddress)

#define SOCKET_WHEN_CALLBACK(name)\
if(whenSocket##name)whenSocket##name(this);

//检查错误,如果有错误则直接调用错误处理函数并返回
#define SOCKET_CHECK_ERROR(code) \
errorNumber = ((code)==-1 ? ERR_NO :0);\
if(errorNumber){\
SOCKET_WHEN_CALLBACK(Error)\
return;\
}

#define SOCKET_CHECK_ERRNO(code,correctErrno,successName) \
errorNumber = ((code)==-1 ? ERR_NO :0);\
if(errorNumber == correctErrno){/*运行状态*/\
	PTHREAD_YIELD\
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

#define SOCKET_WHEN(name) whenSocket##name(NULL)
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

void Socket::connect(const IPAddress &ipAddress,uint16 port){
	SOCKET_CHECK_ERROR(createSocket())
	setSocketAddress(ipAddress,port);
	//准备启动线程
	command=Command_Connect;
	thread.whenThreadError=whenThreadError;
	thread.start(Socket::commandLoop,this);//开始连接
}

#define SOCKET_CONNECT(type) \
void Socket::connect(type ipAddress,uint16 port){connect(IPAddress(ipAddress),port);}

SOCKET_CONNECT(const string&)
SOCKET_CONNECT(const char*)
SOCKET_CONNECT(uint32)
#undef SOCKET_CONNECT

void Socket::listenPort(uint16 port){
	SOCKET_CHECK_ERROR(createSocket())
	setSocketAddress(IPAddress(),port);
	SOCKET_CHECK_ERROR(::bind(SOCKET_CONNECT_ARGUMENTS));
	SOCKET_CHECK_ERROR(::listen(descriptor,0xFFFF));
	thread.start(Socket::acceptLoop,this);//开始连接
}
void Socket::waitListenFinish(){
	thread.join();
}
Socket* Socket::acceptedSocket()const{return newAcceptedSocket;}

int Socket::createSocket(){
#ifdef __MINGW32__ //Windows下要初始化
	if(!wsaStartedUp){
		errorNumber=WSAStartup(MAKEWORD(2,2),&wsaData);
		wsaStartedUp=(errorNumber==0);
		if(!wsaStartedUp){
			return -1;
		}
	}
#endif
	if(descriptor<=0){
		descriptor=::socket(AF_INET,SOCK_STREAM|SOCK_NONBLOCK,0);
	}
	return descriptor;
}
void Socket::setSocketAddress(const IPAddress &ipAddress,uint16 port){
	socketAddress.sin_family=AF_INET;
	socketAddress.sin_addr=ipAddress.address;
	socketAddress.sin_port=htons(port);
}
void Socket::whenThreadError(Thread *thread){
	printf("socket thread error: %d\n",thread->errorNumber);
}
void Socket::Data::set(decltype(addr) addr,decltype(size) size){
	this->addr=addr;
	this->size=size;
}

void Socket::commandLoop(){
	//循环收发
	while(command!=Command_Close){
		switch(command){
			case Command_Connect:{
				SOCKET_CHECK_ERRNO(::connect(SOCKET_CONNECT_ARGUMENTS),EINPROGRESS,Connected)
			}break;
			case Command_Send:{
				//发送数据
				int sndAmount=::send(descriptor,toSendData.addr,toSendData.size,0);
				if(sndAmount>0){
					sentData.set(toSendData.addr,sndAmount);
					SOCKET_WHEN_CALLBACK(Sent)
					sentData.set();
				}else if(sndAmount==-1){
					errorNumber=ERR_NO;
					SOCKET_WHEN_CALLBACK(Error);
				}else{
					PTHREAD_YIELD
				}
				//命令终止
				command=Command_None;
			}break;
			//啥也没干
			default:PTHREAD_YIELD;
		}
		//接收数据(注意,如果不做接收处理,则数据会被立刻废弃)
		uint8 buf[BUFSIZ];
		int rcvAmount=::recv(descriptor,buf,BUFSIZ,0);
		if(rcvAmount>0){
			recvData.set(buf,rcvAmount);
			SOCKET_WHEN_CALLBACK(Received)
			recvData.set();
		}else if(rcvAmount==-1){
			errorNumber=ERR_NO;
			SOCKET_WHEN_CALLBACK(Error);
		}else{
			PTHREAD_YIELD
		}
	}
	//循环结束
	SOCKET_SHUTDOWN
	if(newAcceptedSocket==this)delete this;//由accept函数new出来的
}
void Socket::acceptLoop(){
	while(command!=Command_Close){//不断监听连接
		socklen_t len=sizeof(socketAddress);
		int fd=::accept(descriptor,(sockaddr*)&socketAddress,&len);//等待连接
		if(fd==-1){
			if(ERR_NO==EAGAIN){
				PTHREAD_YIELD
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
	SOCKET_SHUTDOWN
}
//pthread线程函数
void* Socket::commandLoop(void *socket){
	((Socket*)socket)->commandLoop();
	return NULL;
}
void* Socket::acceptLoop(void *socket){
	((Socket*)socket)->acceptLoop();
	return NULL;
}

//收发数据
void Socket::send(const void *buffer,size_t size){
	toSendData.addr=buffer;
	toSendData.size=size;
	command=Command_Send;
}
//关闭连接
void Socket::close(){command=Command_Close;}

IPAddress Socket::getIPaddress()const{return IPAddress(socketAddress.sin_addr.s_addr);}
uint16 Socket::getPort()const{return ntohs(socketAddress.sin_port);}