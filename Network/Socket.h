#ifndef SOCKET_H
#define SOCKET_H

#ifdef __MINGW32__
	//必须得有如下判定,否则编译的时候会报非常无聊的警告
	#ifndef _WINSOCKAPI_
		#define NOGDI//防止Polygen出现重名
		#include<winsock2.h>
	#endif
#else
	#include<sys/socket.h>
	#include<netinet/in.h>
	#include<sys/epoll.h>
#endif

#include"DataBlock.h"

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

//客户端数据包结构体,用于发数据
class SocketDataBlock:public DataBlock{
public:
	SizeType rwSize;//已经读写的数据的大小
	SocketDataBlock();

	void readyReadWrite();//准备进行读写
	inline void readWrote(int delta){rwSize+=delta;}//修改已经读写的数据量

#define BLOCK_RW(Type)\
	SocketDataBlock& write(const Type &val);\
	SocketDataBlock& read(Type &val);

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

	BLOCK_RW(string)
	BLOCK_RW(DataBlock)
#undef BLOCK_RW
	SocketDataBlock& write(const char* val);
};

//SOCKET的全部事件
#define SOCKET_ALL_EVENTS(MACRO)\
MACRO(Connected)/*连接成功*/\
MACRO(Disconnected)/*连接断开*/\
MACRO(Accepted)/*收到连接*/\
MACRO(Send)/*要发送数据,需要用户补充数据*/\
MACRO(Sent)/*数据发送完毕*/\
MACRO(Receive)/*要接收数据,需要用户准备接收*/\
MACRO(Received)/*接收数据完毕*/\
MACRO(Error)/*出错*/\

//套接字,原意为"插座",主要用于主动和被动连接
//主动连接的方法为connect,需要提供目标IP地址和端口
//被动连接的方法为accept,需要提供监听的端口号
//发送数据的方法为send,需要提供数据地址和长度
//接收数据的方法为receive,需要提供缓冲地址和长度
class Socket{
	int createSocket();
	void setSocketAddress(const IPAddress &ipAddress,uint16 port);

	//变量
	int descriptor;//socket描述符
	sockaddr_in socketAddress;//套接字信息
	//epoll
	int epollFD;
public:
	Socket();
	~Socket();

	//对IP地址和端口进行主动连接
	void connect(const IPAddress &ipAddress,uint16 port);
	void connect(const string &ipAddress,uint16 port);
	void connect(const char *ipAddress,uint16 port);
	void connect(uint32 ipAddress,uint16 port);
	bool isConnected()const;//是否处于连接状态

	//被动连接
	void listenPort(uint16 port);//开始接受端口port连进来的连接
	void acceptLoop();//接受循环,服务端专用
	Socket *newAcceptSocket;//新接入的socket

	//收发数据
	bool send();
	bool recv();

	int addTimeSlice();//增加时间片,由操作系统频繁调用
#ifdef __MINGW32__
#else
	void epollEvent(epoll_event &ev);
#endif
	DataBlock sendData;//要发送的数据,请在whenSocketSend中读取
	DataBlock recvData;//已收到的数据,请在whenSocketReceived中读取
	//关闭连接
	void close();

	//状态
	int errorNumber;//错误号,出错的原因保存在此
	IPAddress getIPaddress()const;
	uint16 getPort()const;

	//回调函数
#define WHEN(name) void (*whenSocket##name)(Socket *socket);
	SOCKET_ALL_EVENTS(WHEN)
#undef WHEN
	void *userData;//用户数据,可在回调函数中获得
};
#endif
