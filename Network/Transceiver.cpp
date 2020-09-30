#include"Transceiver.h"
#include"ErrorNumber.h"
#include<string.h>

#define WHEN_CALLBACK(name) if(whenTransceiver##name)whenTransceiver##name(this);

//发送缓冲的大小
const SizeType Transceiver::defaultHeaderSize=sizeof(Transceiver::packetLength);
#define RW_BUFFER_SIZE 65536

Transceiver::Transceiver():socket(NULL),fileToSend(NULL),fileToRecv(NULL),
sentSize(0),sendFileSize(0),receivedSize(0),recvFileSize(0),packetLength(0),
whenTransceiverReceived(NULL),whenTransceiverReceivedFile(NULL){
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

//事件函数
#define GET_TRANSCEIVER auto transceiver=reinterpret_cast<Transceiver*>(socket->userData);
#define WHEN(name) \
void Transceiver::whenSocket##name(Socket *socket){\
	reinterpret_cast<Transceiver*>(socket->userData)->whenSocket##name();\
}
SOCKET_ALL_EVENTS(WHEN)
#undef WHEN

void Transceiver::whenSocketConnected(){printf("收发器-已连接\n");}
void Transceiver::whenSocketDisconnected(){printf("收发器-已断开连接\n");}
void Transceiver::whenSocketAccepted(){printf("收发器-接受连接\n");}
void Transceiver::whenSocketSend(){
	if(writeBuffer.rwSize<=0){//无数据,可以尝试填充文件数据
		sendFileData();
		if(writeBuffer.rwSize<=0)return;//依然没有数据,可以返回
	}
	//回到报头,写入报文长度
	auto size = writeBuffer.rwSize;
	writeBuffer.rwSize=0;
	writeBuffer.write((uint16)(size-defaultHeaderSize));//写入数据长度(不包括报头长度)
	writeBuffer.rwSize=size;
	//等待socket发送
	socket->sendData.set(writeBuffer.dataPointer,writeBuffer.rwSize);
}
void Transceiver::whenSocketSent(){
	//统计已经发送的文件量
	if(fileToSend){
		sentSize=ftell(fileToSend);
	}
	writeBuffer.rwSize=0;//发送缓冲清空
}
void Transceiver::whenSocketReceive(){
	auto &rb=readBuffer;
	socket->recvData.set(&rb.dataPointer[rb.rwSize],rb.dataLength-rb.rwSize);//设置可接收数据的区域
}
void Transceiver::whenSocketReceived(){
	//将数据写入缓冲
	readBuffer.rwSize = readBuffer.dataLength -socket->recvData.dataLength;
	if(readBuffer.get_uint16(0,packetLength)){//读取报头长度值
		if(readBuffer.rwSize < defaultHeaderSize + packetLength)return;//检查报文是否接收完毕,没接收玩则要等到接收完毕
	}else return;//报头都没接收完....

	//处理报文
	auto rwSize=readBuffer.rwSize;
	WHEN_CALLBACK(Received)//用户自行处理数据
	//处理完毕,处理后续数据
	auto remain = rwSize - defaultHeaderSize - packetLength;
	if(remain>0){
		readBuffer.memcpyFrom(&readBuffer.dataPointer[defaultHeaderSize+packetLength],remain);
	}
	readBuffer.rwSize=remain;
}
void Transceiver::whenSocketError(){WHEN_CALLBACK(Error)}

void Transceiver::setSocket(Socket &skt){
	socket=&skt;
	socket->userData=this;
	//事件关联
#define WHEN(name) socket->whenSocket##name=whenSocket##name;
	SOCKET_ALL_EVENTS(WHEN)
#undef WHEN
}
int Transceiver::epollWait(){return socket->epollWait();}

SocketDataBlock& Transceiver::readySend(const string &command){
	writeBuffer.rwSize=0;//清空
	writeBuffer.write((uint16)0);//添加报头
	if(!command.empty()){//客户端可能会有命令
		writeBuffer.write(command);
	}
	return writeBuffer;
}

bool Transceiver::sendData(){
	whenSocketSend();//准备好缓冲区数据
	return socket->send();
}
//发送文件模块
bool Transceiver::sendFile(const char *filename){
	//打开文件
	if(fileToSend)return false;//防止频繁调用
	sendFileSize=sentSize=0;
	fileToSend=fopen(filename,"rb");
	//计算发送大小
	if(fileToSend){
		fseek(fileToSend,0,SEEK_END);
		sendFileSize=ftell(fileToSend);
		fseek(fileToSend,0,SEEK_SET);
	}
	return fileToSend;
}

bool Transceiver::sendFileData(){
	if(!fileToSend)return false;
	auto sizeToSend = sendFileSize - sentSize;
	if(sizeToSend>0){//没发完,继续发
		readySend("FILE");
		//判断发送量
		sizeToSend = min(sizeToSend,writeBuffer.dataLength - writeBuffer.rwSize);//限制一次发送的总量
		sizeToSend *= fread(&writeBuffer.dataPointer[writeBuffer.rwSize],sizeToSend,1,fileToSend);//从文件读取数据
		writeBuffer.rwSize += sizeToSend;
		return true;
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
	//写入数据
	auto sizeToRecv = defaultHeaderSize+packetLength-readBuffer.rwSize;
	fwrite(&readBuffer.dataPointer[readBuffer.rwSize],sizeToRecv,1,fileToRecv);
	receivedSize += sizeToRecv;
	//检查是否写完
	if(receivedSize >= recvFileSize){
		finishReceiveFile();
	}
	return true;
}
bool Transceiver::finishReceiveFile(){
	if(fileToRecv){
		int a=fflush(fileToRecv);//写入剩余数据
		int b=fclose(fileToRecv);//关闭文件
		fileToRecv=NULL;
		WHEN_CALLBACK(ReceivedFile)//事件-文件接收完毕
		return a==0 && b==0;//都没问题
	}
	return false;
}