#ifndef SOCKET_H
#define SOCKET_H

#include"typedef.h"
#include"Thread.h"

#ifdef __MINGW32__
#include<winsock2.h>
#else
#include<sys/socket.h>
#include<netinet/in.h>
#endif

#include<string>
using namespace std;

//IP地址
struct IPAddress{
	in_addr address;

	explicit IPAddress(uint32 addr=INADDR_ANY);
	explicit IPAddress(const char *str);
	explicit IPAddress(const string &str);
	//字符串
	void setAddress(const char *str);
	void setAddress(const string &str);
	char* toString()const;
	string toStdString()const;
};

//套接字,原意为"插座",主要用于主动和被动连接
//主动连接的方法为connect,需要提供目标IP地址和端口
//被动连接的方法为accept,需要提供监听的端口号
//发送数据的方法为send,需要提供数据地址和长度
//接收数据的方法为receive,需要提供缓冲地址和长度
class Socket{
	int createSocket();
	void setSocketAddress(const IPAddress &ipAddress,uint16 port);

	//pthread回调函数,用线程处理阻塞函数
#define SOCKET_PTHREAD(name) \
	static void* name(void *socket);\
	void name();

	SOCKET_PTHREAD(commandLoop)
	SOCKET_PTHREAD(acceptLoop)
#undef SOCKET_PTHREAD
	static void whenThreadError(Thread *thread);
	//变量
	int descriptor;//socket描述符
	sockaddr_in socketAddress;//套接字信息
	Thread thread;//线程,连接监听等动作通过线程进行异步操作
	//缓冲区
	Socket *newAcceptedSocket;//新接收到的套接字,具体请看accept相关过程
	struct Data{//收发用的内存数据结构
		const void *addr;//数据地址
		size_t size;//数据长度
		void set(decltype(addr) addr=nullptr,decltype(size) size=0);
	};
	Data toSendData;//要发送的数据

	//命令
	enum Command{
		Command_None,//无操作
		Command_Connect,//连接命令
		Command_Send,//发送数据
		Command_Close//关闭连接
	};
	Command command;
public:
	Socket();

	//对IP地址和端口进行主动连接
	void connect(const IPAddress &ipAddress,uint16 port);
	void connect(const string &ipAddress,uint16 port);
	void connect(const char *ipAddress,uint16 port);
	void connect(uint32 ipAddress,uint16 port);

	//被动连接
	void listenPort(uint16 port);//开始接受端口port连进来的连接
	void waitListenFinish();//等待连接完成,可能阻塞
	Socket* acceptedSocket()const;//获取刚连进来的套接字

	//收发数据
	void send(const void *buffer,size_t size);
	Data sentData;//已发送的数据,请在whenSocketSent中读取
	Data recvData;//已收到的数据,请在whenSocketReceived中读取
	//关闭连接
	void close();

	//状态
	int errorNumber;//错误号,出错的原因保存在此
	IPAddress getIPaddress()const;
	uint16 getPort()const;
	//回调函数
#define SOCKET_WHEN(name) \
	void (*whenSocket##name)(Socket *socket);

	SOCKET_WHEN(Error)//错误处理
	SOCKET_WHEN(Connected)//主动连接成功
	SOCKET_WHEN(Accepted)//被动连接成功
	SOCKET_WHEN(Sent)//数据发送
	SOCKET_WHEN(Received)//数据接收
#undef SOCKET_WHEN
};
#endif