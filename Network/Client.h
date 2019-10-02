#ifndef CLIENT_H
#define CLIENT_H

#include"Socket.h"
//客户端,用于向服务端发送请求,成员函数基本都会执行网络发送数据的动作
//正常情况下,Client对象在客户端中只需要一个,如果需要连接多个端口,应当考虑扩展Client中的socket个数
class Client{
	Socket socket;//用于向服务端收发数据
	const void *toSendData;
public:
	Client();
	//连接目标,在需要重新连接时候可用
	const string *serverIPaddress;
	const uint16 *serverPort;

	//客户端回调函数
	void (*whenError)();//错误回调
	void (*whenConnected)();
	//连接过程
	void connect(const string &ipAddress,uint16 port);
	//发送过程
	void sendData(void *dataPtr,size_t size);
	//请求
	void login(const string &username,const string &password);
};
#endif