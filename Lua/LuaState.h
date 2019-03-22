#ifndef LUASTATE_H
#define LUASTATE_H

#include"EnumType.h"
#include"TranslationMap.h"

#include"lua.hpp"
#include<string>
using namespace std;

//对code进行断言,断言失败则会返回错误信息
#define LUASTATE_ASSERT(code,errStr)\
if(code);else{\
errorString=errStr;\
return false;\
}

/*基于lua_State工作的类,本类可以操作一个state,并封装常用的操作,
设计初衷是将一切和Lua交互的部分都放在这里
基本上每个方法的返回值都是bool类型,如果返回false则代表不成功,需要去查查errorString的内容
*/
struct LuaState{
	LuaState();
	~LuaState();

	bool doFile(const string &filename);//加载并执行filename文件
	//读写变量
	bool setGlobalInteger(const string &name,int value);//设置全局数字
	bool getGlobalInteger(const string &name,int &value);//获取全局数字
	bool setGlobalString(const string &name,const string &value);//设置全局字符串
	bool getGlobalString(const string &name,string &value);//读取全局字符串
	bool getGlobalTable(const string &name);//读取全局表
	bool toInteger(int index,int &value);//数字转换,非数字可能转换失败
	//枚举
	bool readEnum(const string &enumName,EnumType &enumType);//读取枚举类型enumName,存储到enumType中
	//清理
	void clearStack();
	//错误信息
	string errorString;//错误信息,可以保存lua的信息,也可以保存自定义信息
private:
	lua_State *luaState;//Lua的栈
};

#endif