#ifndef SOCKET_H
#define SOCKET_H

#ifdef __MINGW32__
	//必须得有如下判定,否则编译的时候会报非常无聊的错误
	#ifndef _WINSOCK_H
		#include<winsock2.h>
	#endif
#else
	#include<sys/socket.h>
	#include<netinet/in.h>
#endif

#include"Thread.h"
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
	SocketDataBlock& add(const Type &val);\
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
	DataBlock toSendData;//要发送的数据

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
	~Socket();

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
	void send(const void *buffer,SizeType size);
	void send(const DataBlock &block);
	DataBlock sentData;//已发送的数据,请在whenSocketSent中读取
	DataBlock recvData;//已收到的数据,请在whenSocketReceived中读取
	//关闭连接
	void close();
	void waitCloseFinish();//等待关闭结束,可能阻塞

	//状态
	int errorNumber;//错误号,出错的原因保存在此
	IPAddress getIPaddress()const;
	uint16 getPort()const;
	enum ConnectStatus{//连接状态
		 Unconnected,//未链接
		 Connecting,//连接中
		 Connected//已连接
	};
	ConnectStatus getConnectStatus()const;
	//回调函数
#define SOCKET_WHEN(name) \
	void (*whenSocket##name)(Socket *socket);

	SOCKET_WHEN(Error)//错误处理
	SOCKET_WHEN(Connected)//主动连接成功
	SOCKET_WHEN(Accepted)//被动连接成功
	SOCKET_WHEN(Sent)//数据发送
	SOCKET_WHEN(Received)//数据接收
#undef SOCKET_WHEN
	void *userData;//用户数据,可在回调函数中获得
private:
	ConnectStatus connectStatus;
};
#endif