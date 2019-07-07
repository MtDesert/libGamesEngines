#ifndef SOCKET_H
#define SOCKET_H

#include"typedef.h"
#include"Thread.h"

#include<sys/socket.h>
#include<netinet/in.h>
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
public:
	Socket();
	//连接特定地址
	void connect(const IPAddress &ipAddress,uint16 port);
	void connect(const string &ipAddress,uint16 port);
	void connect(const char *ipAddress,uint16 port);
	void connect(uint32 ipAddress,uint16 port);

	//状态
	error_t errorNumber;//错误号,出错的原因保存在此
	//回调函数
	void (*whenError)();//错误处理
	void (*whenConnected)();//连接成功时候调用此函数
private:
	bool createSocket();//创建套接字,返回是否创建成功
	bool checkError();//检查错误,返回是否无错误

	//回调函数
	static void* connect(void *socket);
	//变量
	int descriptor;//socket描述符
	sockaddr_in socketAddress;//套接字信息
	Thread thread;//线程,连接监听等动作通过线程进行异步操作
};
#endif