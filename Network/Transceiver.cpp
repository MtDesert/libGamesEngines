#include"Transceiver.h"
#include"ErrorNumber.h"
#include<string.h>

//发送缓冲的大小
const SizeType Transceiver::defaultHeaderSize=sizeof(Transceiver::packetLength);
#define RW_BUFFER_SIZE 65536

Transceiver::Transceiver():socket(NULL),fileToSend(NULL),fileToRecv(NULL),
sentSize(0),sendFileSize(0),receivedSize(0),recvFileSize(0),packetLength(0),
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
	SOCKET_ALL_EVENTS(WHEN)
#undef WHEN
}
int Transceiver::epollWait(){return socket->epollWait();}

//事件函数
#define GET_TRANSCEIVER auto transceiver=reinterpret_cast<Transceiver*>(socket->userData);
void Transceiver::whenSocketConnected(Socket *socket){//连接成功后,有数据则发送数据
	printf("socket连接成功\n");
	GET_TRANSCEIVER
	transceiver->sendData();
}
void Transceiver::whenSocketDisconnected(Socket *socket){
	printf("socket断开连接\n");
}
void Transceiver::whenSocketAccepted(Socket *socket){
	printf("socket收到连接\n");
}
void Transceiver::whenSocketSend(Socket *socket){//当数据发送成功后,持续发送剩余数据
	//printf("socket要发送数据\n");
	GET_TRANSCEIVER
	transceiver->whenSend();
}
void Transceiver::whenSocketSent(Socket *socket){
	//printf("socket数据发送完毕\n");
	GET_TRANSCEIVER
	transceiver->whenSent();
}
void Transceiver::whenSocketReceive(Socket *socket){
	//printf("socket准备接收数据\n");
	GET_TRANSCEIVER
	transceiver->whenReceive();
}
void Transceiver::whenSocketReceived(Socket *socket){//当数据接收成功后
	//printf("socket已接收数据\n");
	GET_TRANSCEIVER
	transceiver->receiveData();
}
void Transceiver::whenSocketError(Socket *socket){
	printf("socket出错: %d %s\n",socket->errorNumber,ErrorNumber::getErrorString(socket->errorNumber));
}
void Transceiver::whenSend(){
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
void Transceiver::whenSent(){
	//统计已经发送的文件量
	if(fileToSend){
		sentSize=ftell(fileToSend);
	}
	writeBuffer.rwSize=0;//发送缓冲清空
}
void Transceiver::whenReceive(){
	auto &rb=readBuffer;
	socket->recvData.set(&rb.dataPointer[rb.rwSize],rb.dataLength-rb.rwSize);//设置可接收数据的区域
}

SocketDataBlock& Transceiver::readySend(const string &command){
	writeBuffer.rwSize=0;//清空
	writeBuffer.write((uint16)0);//添加报头
	if(!command.empty()){//客户端可能会有命令
		writeBuffer.write(command);
	}
	return writeBuffer;
}

bool Transceiver::sendData(){
	whenSend();//准备好缓冲区数据
	return socket->send();
}
bool Transceiver::receiveData(){
	//将数据写入缓冲
	readBuffer.rwSize = readBuffer.dataLength -socket->recvData.dataLength;
	if(readBuffer.get_uint16(0,packetLength)){//读取报头长度值
		if(readBuffer.rwSize < defaultHeaderSize + packetLength)return false;//检查报文是否接收完毕,没接收玩则要等到接收完毕
	}else return false;//报头都没接收完....

	//处理报文
	auto rwSize=readBuffer.rwSize;
	if(whenTransceiverReceived)whenTransceiverReceived(this);//用户自行处理数据
	//处理完毕,处理后续数据
	auto remain = rwSize - defaultHeaderSize - packetLength;
	if(remain>0){
		readBuffer.memcpyFrom(&readBuffer.dataPointer[defaultHeaderSize+packetLength],remain);
	}
	readBuffer.rwSize=remain;
	return true;
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
#include<unistd.h>
bool Transceiver::sendFileData(){
	if(!fileToSend)return false;
	auto sizeToSend = sendFileSize - sentSize;
	if(sizeToSend>0){//没发完,继续发
		readySend("DATA");
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
		return a==0 && b==0;//都没问题
	}
	return false;
}