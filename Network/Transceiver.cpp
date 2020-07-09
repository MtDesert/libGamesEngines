#include"Transceiver.h"
#include<string.h>

//发送缓冲的大小
#define SEND_BUF_SIZE 8192
#define HEADER_LEN 5

Transceiver::Transceiver():fileToSend(NULL),fileToRecv(NULL),sentSize(0),sendFileSize(0),receivedSize(0),recvFileSize(0){
	socket.whenSocketSent=whenSocketSent;
	socket.whenSocketReceived=whenSocketReceived;
	socket.userData=this;
}
Transceiver::~Transceiver(){}

#define GET_TRANSCEIVER auto transceiver=reinterpret_cast<Transceiver*>(socket->userData);
void Transceiver::whenSocketSent(Socket *socket){//当数据发送成功后,持续发送剩余数据
	GET_TRANSCEIVER
	if(transceiver->fileToSend){//发送
		transceiver->sentSize += socket->sentData.dataLength;
		int remain = transceiver->sendFileSize - transceiver->sentSize;
		if(remain>0){//没发完,继续发
			char buffer[SEND_BUF_SIZE];//发送用的缓冲
			//写数据头
			memcpy(buffer,"DATA\0",HEADER_LEN);
			//判断发送量
			remain=min(remain,SEND_BUF_SIZE-HEADER_LEN);//限制一次发送的总量
			remain=fread(buffer,remain,1,transceiver->fileToSend);//从文件读取数据
			socket->send(buffer,remain);//开始发送
		}else{//发完了,关闭文件
			transceiver->finishSendFile();
		}
	}
}
void Transceiver::whenSocketReceived(Socket *socket){//当数据接收成功后
	GET_TRANSCEIVER
	bool isFileData=false;
	auto &rcvDat=socket->recvData;
	if(transceiver->fileToRecv){//接收文件模式
		if(memcmp("DATA\0",rcvDat.dataPointer,HEADER_LEN)==0){//比对数据头
			isFileData=true;
			//写入数据
			rcvDat.set(&rcvDat.dataPointer[HEADER_LEN],rcvDat.dataLength-5);
			fwrite(rcvDat.dataPointer,rcvDat.dataLength,1,transceiver->fileToRecv);
			transceiver->receivedSize += rcvDat.dataLength;
			//检查是否写完
			if(transceiver->receivedSize >= transceiver->recvFileSize){
				transceiver->finishWriteFile();
			}
		}
	}
	//非文件数据时候,再做其它处理
	if(!isFileData){
	}
}

bool Transceiver::sendFile(const string &filename){
	fileToSend=fopen(filename.data(),"rb");
	if(fileToSend){//计算发送大小
		fseek(fileToSend,0,SEEK_END);
		sendFileSize=ftell(fileToSend);
		fseek(fileToSend,0,SEEK_SET);
	}
	return fileToSend;
}
bool Transceiver::readyWriteFile(const string &filename,SizeType fileSize){
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
bool Transceiver::finishSendFile(){
	if(fileToSend){
		auto ret=fclose(fileToSend);
		fileToSend=NULL;
		return ret==0;
	}
	return false;
}
bool Transceiver::finishWriteFile(){
	if(fileToRecv){
		int a=fflush(fileToRecv);//写入剩余数据
		int b=fclose(fileToRecv);//关闭文件
		fileToRecv=NULL;
		return a==0 && b==0;//都没问题
	}
	return false;
}