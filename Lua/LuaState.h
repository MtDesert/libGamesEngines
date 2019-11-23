#ifndef LUASTATE_H
#define LUASTATE_H

#include"EnumType.h"
#include"TranslationMap.h"

#include"lua.hpp"
#include<string>
using namespace std;

//创建lua的state并打开文件
#define LUASTATE_OPENFILE(filename)\
/*创建state*/\
auto state=luaL_newstate();\
/*加载文件*/\
luaL_loadfile(state,filename);\
lua_pcall(state,0,LUA_MULTRET,0)


//此宏用于table的遍历,用code处理每一个table的元素
#define LUASTATE_TABLE_FOREACH(state,code)\
lua_pushnil(state);\
while(lua_next(state,-2)){\
	code;\
	lua_pop(state,1);\
}

/*基于lua_State工作的类,本类可以操作一个state,并封装常用的操作,
设计初衷是将一切和Lua交互的部分都放在这里
基本上每个方法的返回值都是bool类型,如果返回false则代表不成功,需要去查查errorString的内容
*/
struct LuaState{
	LuaState();
	~LuaState();

	bool loadFile(const string &filename);//加载文件,不执行
	bool protectCall();
	bool doFile(const string &filename);//加载并执行filename文件
	//读写变量
	bool setGlobalInteger(const string &name,int value);//设置全局数字
	bool getGlobalInteger(const string &name,int &value);//获取全局数字
	bool setGlobalString(const string &name,const string &value);//设置全局字符串
	bool getGlobalString(const string &name,string &value);//读取全局字符串
	bool toInteger(int index,int &value);//数字转换,非数字可能转换失败
	//枚举
	bool readEnum(const string &enumName,EnumType &enumType);//读取枚举类型enumName,存储到enumType中
	//表
	bool getGlobalTable(const string &name);//读取全局表
	//函数
	void registerFunction(const char *name,lua_CFunction func);//注册函数
	//清理
	void clearStack();
	//错误信息
	void (*whenError)(const string &errorString);//错误信息回调函数
private:
	lua_State *luaState;//Lua的栈
};

#endif