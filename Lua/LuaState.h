#ifndef LUASTATE_H
#define LUASTATE_H

#include"EnumType.h"

#include"lua.hpp"
#include<string>
using namespace std;

/*基于lua_State工作的类,本类可以操作一个state
*/
struct LuaState{
	LuaState();
	~LuaState();

	bool doFile(const string &filename);//加载并执行filename文件
	//枚举
	bool readEnum(const string &enumName,EnumType &enumType);//读取枚举类型enumName,存储到enumType中
private:
	lua_State *luaState;//Lua的栈
	string errorString;//错误信息,可以保存lua的信息,也可以保存自定义信息
};

#endif