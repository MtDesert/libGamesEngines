#ifndef PROTOCOL_H
#define PROTOCOL_H

//游戏通讯协议
//基本请求,根据需求添加更多请求
#define CLIENT_BASE_REQUEST(MACRO)\
MACRO(Update)\
MACRO(Upgrade)\
MACRO(SelectGame)\
MACRO(Register)\
MACRO(Login)\
MACRO(Logout)

//请求类型
#define ENUM(name) enum##name,
enum EnumRequest{
	CLIENT_BASE_REQUEST(ENUM)
	AmountOfBaseRequest
};
#undef ENUM

#endif