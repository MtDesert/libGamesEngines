#ifndef PROTOCOL_H
#define PROTOCOL_H

//游戏通讯协议
#define CLIENT_REQUEST(MACRO)\
MACRO(UpdateSOfiles)/*检查.so文件的更新情况,服务端返回哪些文件需要更新*/\
MACRO(UpgradeSOfiles)/*更新.so文件,客户端请求更新文件名,服务端返回整个文件*/
//基本请求,根据需求添加更多请求
#define PLAYER_REQUEST(MACRO)\
MACRO(Register)/*注册帐号,客户端发送注册信息,服务端返回注册情况*/\
MACRO(Login)/*登陆帐号,客户端发送登陆信息,服务端返回登陆情况*/\
MACRO(Logout)/*登出,退出帐号登陆*/

#endif