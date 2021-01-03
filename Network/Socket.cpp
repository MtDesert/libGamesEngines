#include"Socket.h"
#include<unistd.h>
#include<errno.h>
#include<string.h>

#ifdef __MINGW32__ //MinGW编译环境
#include<ws2tcpip.h>
#define SOCK_NONBLOCK 0
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

#ifdef __ANDROID__
#undef PTHREAD_YIELD
#define PTHREAD_YIELD usleep(1);
#endif

#define SOCKET_CONNECT_ARGUMENTS \
descriptor,(const sockaddr*)&socketAddress,sizeof(socketAddress)
//设置回调函数的情况下,调用特定回调函数
#define SOCKET_WHEN_CALLBACK(name)\
if(whenSocket##name)whenSocket##name(this);
//出错时,设置错误号并调用回调函数
#define SOCKET_WHEN_ERROR \
errorNumber=ERR_NO;\
SOCKET_WHEN_CALLBACK(Error)

//检查错误,如果有错误则直接调用错误处理函数并返回
#define SOCKET_CHECK_ERROR(code) \
errorNumber = ((code)==-1 ? ERR_NO :0);\
if(errorNumber){\
SOCKET_WHEN_CALLBACK(Error)\
return;\
}

IPAddress::IPAddress(uint32 addr){address.s_addr=addr;}
IPAddress::IPAddress(const char *str){setAddress(str);}
IPAddress::IPAddress(const string &str){setAddress(str);}

#define WHEN(name) whenSocket##name(NULL),
Socket::Socket():descriptor(0),epollFD(0),newAcceptSocket(NULL),errorNumber(0),
	SOCKET_ALL_EVENTS(WHEN)
#undef WHEN
	userData(nullptr){}
Socket::~Socket(){}

//ip address
void IPAddress::setAddress(const char *str){address.s_addr=inet_addr(str);}
void IPAddress::setAddress(const string &str){setAddress(str.data());}
char *IPAddress::toString()const{return inet_ntoa(address);}
string IPAddress::toStdString()const{return toString();}

SocketDataBlock::SocketDataBlock(){readyReadWrite();}
void SocketDataBlock::readyReadWrite(){rwSize=0;}

#define BLOCK_RW(Type)\
SocketDataBlock& SocketDataBlock::write(const Type &val){\
	if(set_##Type(rwSize,val)){\
		readWrote(sizeof(Type));\
	}return *this;\
}\
SocketDataBlock& SocketDataBlock::read(Type &val){\
	if(get_##Type(rwSize,val)){\
		readWrote(sizeof(Type));\
	}return *this;\
}

BLOCK_RW(int8)
BLOCK_RW(int16)
BLOCK_RW(int32)
BLOCK_RW(long)
BLOCK_RW(int64)

BLOCK_RW(uint8)
BLOCK_RW(uint16)
BLOCK_RW(uint32)
BLOCK_RW(ulong)
BLOCK_RW(uint64)

BLOCK_RW(wchar_t)
BLOCK_RW(char16_t)
BLOCK_RW(char32_t)
BLOCK_RW(float)
BLOCK_RW(double)
#undef BLOCK_RW

SocketDataBlock& SocketDataBlock::write(const DataBlock &data){
	data.memcpyTo(&dataPointer[rwSize],dataLength-rwSize);
	readWrote(data.dataLength);
	return *this;
}
SocketDataBlock& SocketDataBlock::write(const string &val){
	if(set_string(rwSize,val)){
		readWrote(val.size());
		dataPointer[rwSize]='\0';
		readWrote(1);
	}return *this;
}
SocketDataBlock& SocketDataBlock::read(DataBlock &data){
	data.memcpyFrom(&dataPointer[rwSize],dataLength-rwSize);
	readWrote(data.dataLength);
	return *this;
}
SocketDataBlock& SocketDataBlock::read(string &val){
	if(get_string(rwSize,val)){
		readWrote(val.size()+1);
	}return *this;
}
SocketDataBlock& SocketDataBlock::write(const char *val){
	if(set_string(rwSize,val)){
		readWrote(strlen(val));
		dataPointer[rwSize]='\0';
		readWrote(1);
	}return *this;
}

void Socket::connect(const IPAddress &ipAddress,uint16 port){
	SOCKET_CHECK_ERROR(createSocket())//创建socket描述符
	setSocketAddress(ipAddress,port);//设置目标ip和端口
	//接入epoll
#ifndef __MINGW32__
	if(epollFD<=0){
		epollFD=epoll_create(1);
		SOCKET_CHECK_ERROR(epollFD)
		struct epoll_event ev;
		memset(&ev,0,sizeof(ev));
		ev.events=EPOLLIN|EPOLLOUT|EPOLLET;
		ev.data.ptr=this;
		SOCKET_CHECK_ERROR(epoll_ctl(epollFD,EPOLL_CTL_ADD,descriptor,&ev));
	}
#endif
	//开始连接
	auto ret=::connect(SOCKET_CONNECT_ARGUMENTS);
	errorNumber=(ret==-1 ? ERR_NO : 0);//errorNumber很可能处于连接中的状态EAGAIN
}

#define SOCKET_CONNECT(type) \
void Socket::connect(type ipAddress,uint16 port){connect(IPAddress(ipAddress),port);}

SOCKET_CONNECT(const string&)
SOCKET_CONNECT(const char*)
SOCKET_CONNECT(uint32)
#undef SOCKET_CONNECT
bool Socket::isConnected()const{
	char ch;
	int ret=::recv(descriptor,&ch,1,MSG_PEEK);
	return ret!=-1 && ERR_NO!=EAGAIN;
}

void Socket::listenPort(uint16 port){
	SOCKET_CHECK_ERROR(createSocket())
	setSocketAddress(IPAddress(),port);
	SOCKET_CHECK_ERROR(::bind(SOCKET_CONNECT_ARGUMENTS));
	SOCKET_CHECK_ERROR(::listen(descriptor,0xFFFF));
}

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
	//创建socket的描述符号
	if(descriptor<=0){
		descriptor=::socket(AF_INET,SOCK_STREAM|SOCK_NONBLOCK,0);//注意:要使用非阻塞模式
	}
	return descriptor;
}
void Socket::setSocketAddress(const IPAddress &ipAddress,uint16 port){
	socketAddress.sin_family=AF_INET;
	socketAddress.sin_addr=ipAddress.address;
	socketAddress.sin_port=htons(port);
}
#ifndef __MINGW32__
void Socket::acceptLoop(){
	//启用epoll
	auto epollFD=epoll_create(65536);
	struct epoll_event eventArray[256],*pEvent=NULL;
	memset(eventArray,0,sizeof(eventArray));
	//注册需要监听的描述符
	eventArray[0].events=EPOLLIN;
	eventArray[0].data.ptr=this;
	epoll_ctl(epollFD,EPOLL_CTL_ADD,descriptor,&eventArray[0]);
	//开始循环
	bool gameOver=false;
	while(!gameOver){
		auto eventAmount=epoll_wait(epollFD,eventArray,256,0);//等待事件发生
		for(int i=0;i<eventAmount;++i){//开始处理所有事件
			pEvent=&eventArray[i];
			//判断是客户端的socket还是服务端的socket
			if(pEvent->data.ptr==this){//服务端监听用的socket
				socklen_t addrLen=sizeof(socketAddress);
				int fd=::accept4(descriptor,(sockaddr*)&socketAddress,&addrLen,SOCK_NONBLOCK);//接受连接
				//新socket
				auto newSocket=new Socket();
				newSocket->descriptor=fd;//描述符
				newSocket->socketAddress=socketAddress;//套接地址
				//触发Accepted事件
				newAcceptSocket=newSocket;
				SOCKET_WHEN_CALLBACK(Accepted)
				newAcceptSocket=NULL;
				//将新的连接进行注册
				pEvent->events=EPOLLIN|EPOLLOUT|EPOLLET;
				pEvent->data.ptr=newSocket;
				epoll_ctl(epollFD,EPOLL_CTL_ADD,fd,pEvent);
			}else{//客户端连接进来的socket
				auto pSocket=reinterpret_cast<Socket*>(pEvent->data.ptr);
				pSocket->epollEvent(*pEvent);
			}
		}
		if(!eventAmount){
			PTHREAD_YIELD;//让出CPU
		}
	}
	//close
	printf("关闭epoll\n");
	::close(epollFD);
}
#define HAS_ERROR ev.events&EPOLLERR
void Socket::epollEvent(epoll_event &ev){
	//开始处理事件
	bool noErrHappen=true;
	if(ev.events & EPOLLIN){//有数据进来了,可读
		noErrHappen=recv();//系统的recv函数可以监测到对方断开的情况
	}
	if(ev.events & EPOLLPRI){
		printf("socket(%p) exceptional condition!\n",this);
	}
	if(ev.events & EPOLLOUT){//有数据发出去了
		if(noErrHappen){
			if(ev.events==EPOLLOUT){
				SOCKET_WHEN_CALLBACK(Connected)
			}
			send();
		}
	}
	if(HAS_ERROR){
		SOCKET_WHEN_ERROR
	}
	if(ev.events & EPOLLHUP){
		printf("socket(%p) hang up!\n",this);
		SOCKET_WHEN_ERROR
	}
	//不常见的事件
	if(ev.events & EPOLLRDNORM){printf("socket(%p) read normal!\n",this);}
	if(ev.events & EPOLLRDBAND){printf("socket(%p) read band\n",this);}
	if(ev.events & EPOLLWRNORM){printf("socket(%p) write normal\n",this);}
	if(ev.events & EPOLLWRBAND){printf("socket(%p) write band\n",this);}
	if(ev.events & EPOLLMSG){printf("socket(%p) message\n",this);}
}
#endif

bool Socket::send(){
	//未连接
	if(!isConnected())return false;
	//不断尝试send,直到数据发完,或者缓冲区过满为止
	int sendLen=0;
	do{
		SOCKET_WHEN_CALLBACK(Send)//用户先补充数据
		if(sendData.hasData()){//有数据,开始发送
			do{//循环发送
				sendLen = ::send(descriptor,(const char*)sendData.dataPointer,sendData.dataLength,0);//发送!
				if(sendLen>0){//调整发送缓冲,再发送
					sendData.set(&sendData.dataPointer[sendLen],sendData.dataLength-sendLen);
				}
			}while(sendLen>0 && sendData.hasData());
			//发送完毕事件
			if(!sendData.hasData()){
				SOCKET_WHEN_CALLBACK(Sent)
			}
		}else break;//没有补充数据,发送结束
	}while(sendLen>0);
	//发送完毕
	if(sendLen==-1 && ERR_NO!=EAGAIN){//出错
		SOCKET_WHEN_ERROR
	}
	return isConnected();
}
bool Socket::recv(){
	int recvLen=0;
	do{//循环收数据,读到无数据可读为止
		SOCKET_WHEN_CALLBACK(Receive)//先做好接收准备
		recvLen=::recv(descriptor,(char*)recvData.dataPointer,recvData.dataLength,0);
		if(recvLen>0){//读取数据后,调整接收缓冲
			recvData.set(&recvData.dataPointer[recvLen],recvData.dataLength-recvLen);
			SOCKET_WHEN_CALLBACK(Received)//收到数据后,立刻交给用户处理
		}
	}while(recvLen>0);
	//读取结束
	if(recvLen==0){//连接断开
		SOCKET_WHEN_CALLBACK(Disconnected)
		return false;
	}else if(recvLen==-1 && ERR_NO!=EAGAIN){//出错
		SOCKET_WHEN_ERROR
		return false;
	}
	return true;
}

int Socket::addTimeSlice(){
#ifdef __MINGW32__//针对Windows,没有epoll
	//需要优化
	recv();
	send();
	return 0;
#else//Linux系列,使用epoll
	struct epoll_event ev;
	memset(&ev,0,sizeof(ev));
	auto amount=epoll_wait(epollFD,&ev,1,0);
	if(amount>0){
		epollEvent(ev);
	}
	return amount;
#endif
}
//关闭连接
void Socket::close(){
	bool discon=isConnected();
	if(::close(descriptor)==0){
		if(discon)SOCKET_WHEN_CALLBACK(Disconnected)//防止多次触发
	}else{
		SOCKET_WHEN_ERROR
	}
	descriptor=0;
}

IPAddress Socket::getIPaddress()const{return IPAddress(socketAddress.sin_addr.s_addr);}
uint16 Socket::getPort()const{return ntohs(socketAddress.sin_port);}