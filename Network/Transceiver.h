#ifndef TRANSCEIVER_H
#define TRANSCEIVER_H

#include"Socket.h"
//收发器,负责传输数据,包括各种简单报文,不定长报文,大文件等
class Transceiver{
	Socket socket;//收发用的套接字
	//文件收发
	FILE *fileToSend,*fileToRecv;//发送/接收文件
	SizeType sentSize,sendFileSize;//发送大小/总大小
	SizeType receivedSize,recvFileSize;//接收大小/总大小
public:
	Transceiver();
	~Transceiver();

	//套接字事件
	static void whenSocketSent(Socket*);
	static void whenSocketReceived(Socket*);

	bool sendFile(const string &filename);//发送文件
	bool readyWriteFile(const string &filename,SizeType fileSize);//准备将接收到的数据写入filename文件中,文件大小为fileSize以便统计
	bool finishSendFile();//发送文件结束
	bool finishWriteFile();//写入文件结束
};
#endif//TRANSCEIVER_H