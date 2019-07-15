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

	IPAddress(uint32 addr=INADDR_ANY);
	IPAddress(const char *str);
	IPAddress(const string &str);
	//字符串
	void setAddress(const char *str);
	void setAddress(const string &str);
	char* toString()const;
	string toStdString()const;
};

//套接字,原意为"插座"
class Socket{
	bool createSocket();//创建套接字,返回是否创建成功
	void setSocketAddress(const IPAddress &ipAddress,uint16 port);

	//pthread回调函数,用线程处理阻塞函数
#define SOCKET_PTHREAD(name) \
	static void* name(void *socket);\
	void name();

	SOCKET_PTHREAD(connect)
	SOCKET_PTHREAD(accept)
#undef SOCKET_PTHREAD
	//变量
	int descriptor;//socket描述符
	sockaddr_in socketAddress;//套接字信息
	Thread thread;//线程,连接监听等动作通过线程进行异步操作
public:
	Socket();

//对IP地址和端口进行操作
#define SOCKET_IPADDRESS_PORT(name) \
void name(const IPAddress &ipAddress,uint16 port);\
void name(const string &ipAddress,uint16 port);\
void name(const char *ipAddress,uint16 port);\
void name(uint32 ipAddress,uint16 port);

	SOCKET_IPADDRESS_PORT(connect)//连接特定地址
	SOCKET_IPADDRESS_PORT(accept)//接受外部连接
#undef SOCKET_IPADDRESS_PORT
	void accept(uint16 port);//开始接受端口port连进来的连接

	//状态
	int errorNumber;//错误号,出错的原因保存在此
	//回调函数
#define SOCKET_WHEN(name) \
	void (*when##name)();\
	void (*whenSocket##name)(Socket *socket);

	SOCKET_WHEN(Error)//错误处理
	SOCKET_WHEN(Connected)//主动连接成功
	SOCKET_WHEN(Accepted)//被动连接成功
#undef SOCKET_WHEN
};
#endif