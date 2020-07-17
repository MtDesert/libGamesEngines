#include"Socket.h"
#include<sys/epoll.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>

#ifdef __MINGW32__ //MinGW编译环境
#include<ws2tcpip.h>
#define SOCK_NONBLOCK 0
#define EWOULDBLOCK WSAEWOULDBLOCK
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

#ifdef __ANDROID__
#undef PTHREAD_YIELD
#define PTHREAD_YIELD usleep(1);
#endif

#define SOCKET_CONNECT_ARGUMENTS \
descriptor,(const sockaddr*)&socketAddress,sizeof(socketAddress)

#define SOCKET_WHEN_CALLBACK(name)\
if(whenSocket##name)whenSocket##name(this);

#define SOCKET_WHEN_CALLBACK_P(name)\
if(pSocket->whenSocket##name)pSocket->whenSocket##name(pSocket);

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

#define SOCKET_WHEN(name) whenSocket##name(NULL)
Socket::Socket():descriptor(0),epollFD(0),isConnected(false),newAcceptSocket(NULL),errorNumber(0),
	SOCKET_WHEN(Error),//错误处理
	SOCKET_WHEN(Connected),//主动连接成功
	SOCKET_WHEN(Accepted),//被动连接成功
	SOCKET_WHEN(Sent),//数据发送
	SOCKET_WHEN(Received),//数据接收
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
SocketDataBlock& SocketDataBlock::add(const Type &val){\
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

SocketDataBlock& SocketDataBlock::add(const DataBlock &data){
	data.memcpyTo(&dataPointer[rwSize],dataLength-rwSize);
	readWrote(data.dataLength);
	return *this;
}
SocketDataBlock& SocketDataBlock::add(const string &val){
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
SocketDataBlock& SocketDataBlock::add(const char *val){
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
	if(epollFD<=0)epollFD=epoll_create(1);
	SOCKET_CHECK_ERROR(epollFD)
	struct epoll_event ev;
	memset(&ev,0,sizeof(ev));
	ev.events=EPOLLIN|EPOLLOUT|EPOLLET;
	ev.data.ptr=this;
	SOCKET_CHECK_ERROR(epoll_ctl(epollFD,EPOLL_CTL_ADD,descriptor,&ev));
	//开始连接
	auto ret=::connect(SOCKET_CONNECT_ARGUMENTS);
	errorNumber=(ret==-1 ? ERR_NO : 0);//errorNumber很可能处于连接中的状态
	printf("socket连接中\n");
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

void printEpoll(epoll_event &ev){
	printf("events: %.8X fd==%d ptr==%p u32==%u u64==%lu\n",ev.events,ev.data.fd,ev.data.ptr,ev.data.u32,ev.data.u64);
}
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
			printEpoll(*pEvent);
			//判断是客户端的socket还是服务端的socket
			if(pEvent->data.ptr==this){//服务端监听用的socket
				socklen_t addrLen=sizeof(socketAddress);
				int fd=::accept(descriptor,(sockaddr*)&socketAddress,&addrLen);//接受连接
				//新socket
				auto newSocket=new Socket();
				newSocket->descriptor=fd;//描述符
				newSocket->socketAddress=socketAddress;//套接地址
				newSocket->isConnected=true;
				//回调函数
				//newSocket->whenSocketSent=whenSocketSent;
				//newSocket->whenSocketReceived=whenSocketReceived;
				//newSocket->whenSocketDisconnected=whenSocketDisconnected;
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
			pthread_yield();//让出CPU
		}
	}
	//close
	printf("关闭epoll\n");
	::close(epollFD);
}

//收发数据
bool Socket::send(const void *buffer,SizeType size){
	if(!isConnected)return isConnected;
	//连接状态下才发送
	int sndAmount=::send(descriptor,buffer,size,0);
	if(sndAmount>=0){//发送数据
		sentData.set(buffer,sndAmount);
	}else if(sndAmount==-1){//出错
		errorNumber=ERR_NO;
		SOCKET_WHEN_CALLBACK(Error)
	}
	return isConnected;
}
bool Socket::send(const DataBlock &block){return send(block.dataPointer,block.dataLength);}
int Socket::epollWait(){
	struct epoll_event ev;
	memset(&ev,0,sizeof(ev));
	auto amount=epoll_wait(epollFD,&ev,1,0);
	if(amount){
		epollEvent(ev);
	}
	return amount;
}
void Socket::epollEvent(epoll_event &ev){
	//开始处理事件
	printEpoll(ev);
	if(ev.events & EPOLLIN){//有数据进来了,可读
		uint8 buffer[BUFSIZ];
		auto recvLen=recv(descriptor,buffer,BUFSIZ,0);
		if(recvLen>0){
			recvData.set(buffer,recvLen);
			SOCKET_WHEN_CALLBACK(Received)
			recvData.set();
		}else if(recvLen==-1){
			errorNumber=ERR_NO;
			SOCKET_WHEN_CALLBACK(Error);
		}
	}
	if(ev.events & EPOLLPRI){
		printf("socket(%p) exceptional condition!\n",this);
	}
	if(ev.events & EPOLLOUT){//有数据发出去了
		if(isConnected){//已连接,触发发送事件
			SOCKET_WHEN_CALLBACK(Sent)
		}else{//未连接,改成连接状态
			isConnected=true;
			SOCKET_WHEN_CALLBACK(Connected)
		}
	}
	if(ev.events & EPOLLERR){
		printf("socket(%p) error!\n",this);
		SOCKET_WHEN_CALLBACK(Error)
	}
	if(ev.events & EPOLLHUP){
		printf("socket(%p) hang up!\n",this);
	}
	//不常见的事件
	/*if(ev.events & EPOLLRDNORM){printf("socket(%p) read normal!\n");}
	if(ev.events & EPOLLRDBAND){printf("socket(%p) read band\n");}
	if(ev.events & EPOLLWRNORM){printf("socket(%p) write normal\n");}
	if(ev.events & EPOLLWRBAND){printf("socket(%p) write band\n");}
	if(ev.events & EPOLLMSG){printf("socket(%p) message\n");}*/
}
//关闭连接
void Socket::close(){
	if(::close(descriptor)==0){
		SOCKET_WHEN_CALLBACK(Disconnected)
	}else{
		errorNumber=ERR_NO;
		SOCKET_WHEN_CALLBACK(Error)
	}
	descriptor=0;
}

IPAddress Socket::getIPaddress()const{return IPAddress(socketAddress.sin_addr.s_addr);}
uint16 Socket::getPort()const{return ntohs(socketAddress.sin_port);}

//轮询
int Socket::addTimeSlice(uint msec){
	return 0;
}