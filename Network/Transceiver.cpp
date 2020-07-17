#include"Transceiver.h"
#include<string.h>

//发送缓冲的大小
#define RW_BUFFER_SIZE 65536

Transceiver::Transceiver():socket(NULL),fileToSend(NULL),fileToRecv(NULL),
sentSize(0),sendFileSize(0),receivedSize(0),recvFileSize(0),
whenTransceiverReceived(NULL){
	//申请缓存
	if(readBuffer.dataLength<=0){
		readBuffer.memoryAllocate(RW_BUFFER_SIZE);
	}
	if(writeBuffer.dataLength<=0){
		writeBuffer.memoryAllocate(RW_BUFFER_SIZE);
	}
}
Transceiver::~Transceiver(){
	readBuffer.memoryFree();
	writeBuffer.memoryFree();
}

void Transceiver::setSocket(Socket &skt){
	socket=&skt;
	socket->userData=this;
	//事件关联
#define WHEN(name) socket->whenSocket##name=whenSocket##name;
	TRANSCEIVER_EVENT(WHEN)
#undef WHEN
}
int Transceiver::epollWait(){return socket->epollWait();}

//事件函数
#define GET_TRANSCEIVER auto transceiver=reinterpret_cast<Transceiver*>(socket->userData);
void Transceiver::whenSocketConnected(Socket *socket){//连接成功后,有数据则发送数据
	GET_TRANSCEIVER
	transceiver->sendData();
}
void Transceiver::whenSocketSent(Socket *socket){//当数据发送成功后,持续发送剩余数据
	GET_TRANSCEIVER
	auto sendingFile=transceiver->sendFileData();//发送文件数据
	if(!sendingFile){//非发送文件时候,可以发送剩余数据
		transceiver->sendData();//连续发送数据
	}
}
void Transceiver::whenSocketReceived(Socket *socket){//当数据接收成功后
	GET_TRANSCEIVER
	//将数据写入缓冲
	socket->recvData.debug();
	transceiver->readBuffer.add(socket->recvData);
	uint16 u16;
	if(transceiver->readBuffer.get_uint16(0,u16)){//读取报头长度值
		printf("socket received: %lu %lu %u\n",transceiver->readBuffer.rwSize,sizeof(uint16),u16);
		if(transceiver->readBuffer.rwSize - sizeof(uint16) < u16)return;//检查报文是否接收完毕,没接收玩则要等到接收完毕
	}else return;//报头都没接收完....
	//尝试读取数据
	bool isFileData=transceiver->receiveFileData();
	if(!isFileData){//非文件数据,我们可以做其它处理
		if(transceiver->whenTransceiverReceived)transceiver->whenTransceiverReceived(transceiver);
	}
}
void Transceiver::whenSocketDisconnected(Socket *socket){}

SocketDataBlock& Transceiver::readySend(const string &command){
	writeBuffer.rwSize=0;//清空
	writeBuffer.add((uint16)0);//添加报头
	if(!command.empty()){//客户端可能会有命令
		writeBuffer.add(command);
	}
	return writeBuffer;
}

#include<unistd.h>

bool Transceiver::sendData(){
	if(writeBuffer.rwSize<=0)return false;//无数据不发送
	//回到报头,写入报文长度
	auto size = writeBuffer.rwSize;
	writeBuffer.rwSize=0;
	writeBuffer.add((uint16)(size-sizeof(uint16)));//写入数据长度(不包括报头长度)
	writeBuffer.rwSize=size;
	//发送
	bool ret=socket->send(writeBuffer.dataPointer,writeBuffer.rwSize);//发送数据
	if(ret){
		writeBuffer.rwSize=0;//防止频繁发送
	}
	return ret;
}
//发送文件模块
bool Transceiver::sendFile(const string &filename){
	//打开文件
	if(fileToSend)return false;//防止频繁调用
	sendFileSize=sentSize=0;
	fileToSend=fopen(filename.data(),"rb");
	//计算发送大小
	if(fileToSend){
		fseek(fileToSend,0,SEEK_END);
		sendFileSize=ftell(fileToSend);
		fseek(fileToSend,0,SEEK_SET);
		return sendFileData();//开始发送
	}
	return fileToSend;
}
bool Transceiver::sendFileData(){
	if(!fileToSend)return false;
	auto sizeToSend = sendFileSize - sentSize;
	if(sizeToSend>0){//没发完,继续发
		readySend("DATA");
		//判断发送量
		sizeToSend = min(sizeToSend,writeBuffer.dataLength - writeBuffer.rwSize);//限制一次发送的总量
		sizeToSend = fread(&writeBuffer.dataPointer[writeBuffer.rwSize],sizeToSend,1,fileToSend);//从文件读取数据
		if(sendData()){//发送成功则累加数据
			sentSize += sizeToSend;
			return true;
		}
	}else{//发完了,关闭文件
		finishSendFile();
	}
	return false;
}
bool Transceiver::finishSendFile(){
	if(fileToSend){
		auto ret=fclose(fileToSend);
		fileToSend=NULL;
		return ret==0;
	}
	return false;
}
//接收文件模块
bool Transceiver::receiveFile(const string &filename,SizeType fileSize){
	if(fileToRecv){//已经处于接收模式了....
		return false;
	}else{//开启文件开始接收
		fileToRecv=fopen(filename.data(),"wb");
		if(fileToRecv){
			receivedSize=0;
			recvFileSize=fileSize;
		}
	}
	return fileToRecv;
}
bool Transceiver::receiveFileData(){
	if(!fileToRecv)return false;
	if(memcmp("DATA\0",&readBuffer.dataPointer[2],5)==0){//比对数据头
		//写入数据
		DataBlock rcvDat=readBuffer.subDataBlock(7,readBuffer.rwSize-7);
		fwrite(rcvDat.dataPointer,rcvDat.dataLength,1,fileToRecv);
		receivedSize += rcvDat.dataLength;
		//检查是否写完
		if(receivedSize >= recvFileSize){
			finishReceiveFile();
		}
		return true;
	}
	return false;
}
bool Transceiver::finishReceiveFile(){
	if(fileToRecv){
		int a=fflush(fileToRecv);//写入剩余数据
		int b=fclose(fileToRecv);//关闭文件
		fileToRecv=NULL;
		return a==0 && b==0;//都没问题
	}
	return false;
}