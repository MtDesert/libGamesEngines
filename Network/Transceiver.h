#ifndef TRANSCEIVER_H
#define TRANSCEIVER_H

#include"Socket.h"
//收发器,负责传输数据,包括各种简单报文,不定长报文,大文件等
class Transceiver{
protected:
	Socket *socket;//收发用的套接字
	//文件收发
	FILE *fileToSend,*fileToRecv;//发送/接收文件
	SizeType sentSize,sendFileSize;//发送大小/总大小
	SizeType receivedSize,recvFileSize;//接收大小/总大小
	//收发缓存
	static const SizeType defaultHeaderSize;//默认包头大小
	uint16 packetLength;//包体大小(不包括包头)
	SocketDataBlock readBuffer,writeBuffer;//读写缓冲
public:
	Transceiver();
	~Transceiver();

	void setSocket(Socket &skt);
	int epollWait();
	//套接字事件
#define WHEN(name) static void whenSocket##name(Socket*);
	SOCKET_ALL_EVENTS(WHEN)
#undef WHEN
	//收发器事件
	void (*whenTransceiverReceived)(Transceiver *transceiver);//接收完毕后调用此参数
	void whenSend();//当需要发送时,自动填充数据
	void whenSent();//当发送完毕时
	void whenReceive();//需要接收时,准备好缓冲区
	//收发数据过程
	SocketDataBlock& readySend(const string &command="");//准备发送,并以指令command为准,可利用返回的数据块来添加数据
	virtual bool sendData();//开始生成报文,发送数据
	virtual bool receiveData();//开始接收数据,并检查数据完整度和触发事件
	//发送文件过程
	bool sendFile(const char *filename);//发送文件
	bool sendFileData();//发送文件数据,返回是否执行了发送过程(此函数在发送文件的过程中可能会被频繁调用)
	bool finishSendFile();//发送文件结束
	//接收文件过程
	bool receiveFile(const string &filename,SizeType fileSize);//准备将接收到的数据写入filename文件中,文件大小为fileSize以便统计
	bool receiveFileData();//接收文件数据,返回是否执行了接收过程(此函数在接收文件的过程中可能会被频繁调用)
	bool finishReceiveFile();//写入文件结束
};
#endif//TRANSCEIVER_H