#include "Client.h"

static Client *client=nullptr;
static void whenSocketError(Socket *socket){
	printf("socket error: %d\n",socket->errorNumber);
	if(client->whenError)client->whenError();
}
static void whenSocketConnected(Socket *socket){
	if(client->whenConnected)client->whenConnected();
}
static void whenSocketSent(Socket *socket){}
static void whenSocketReceived(Socket *socket){}

Client::Client():serverIPaddress(nullptr),serverPort(nullptr){
	//socket回调函数
	socket.whenSocketError=whenSocketError;
	socket.whenSocketConnected=whenSocketConnected;
	socket.whenSocketSent=whenSocketSent;
	socket.whenSocketReceived=whenSocketReceived;
	client=this;
}

void Client::connect(const string &ipAddress, uint16 port){
	switch(socket.getConnectStatus()){
		case Socket::Unconnected:
			socket.connect(ipAddress,port);
		break;
		case Socket::Connected:
			::whenSocketConnected(&socket);
		break;
		default:;
	}
}
void Client::sendData(void *dataPtr,size_t size){
	switch(socket.getConnectStatus()){
		case Socket::Unconnected:
			if(serverIPaddress && serverPort){
				//保存要发的数据
				socket.connect(*serverIPaddress,*serverPort);//重连
			}
		break;
		case Socket::Connected:
			socket.send(dataPtr,size);
		break;
		default:;
	}
}

void Client::login(const string &username,const string &password){
}