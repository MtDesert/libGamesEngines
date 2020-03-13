#ifndef LUASTATE_H
#define LUASTATE_H

#include"EnumType.h"
#include"lua.hpp"

/*基于lua_State工作的类,本类可以操作一个state,并封装常用的操作,
设计初衷是将一切和Lua交互的部分都放在这里
基本上每个方法的返回值都是bool类型,如果返回false则代表不成功,需要去查查errorString的内容
*/
struct LuaState{
	LuaState();
	~LuaState();

	//文件加载
	bool loadFile(const string &filename);//加载文件,不执行
	bool protectCall();//保护方式执行
	bool doFile(const string &filename);//加载并执行filename文件
	//写全局变量(变量名,变量值),返回是否成功
	void setGlobalBoolean(const string &name,bool value);
	void setGlobalNumber(const string &name,double value);
	void setGlobalInteger(const string &name,int value);
	void setGlobalString(const string &name,const string &value);
	//读全局变量(变量名,接收变量),返回是否成功
	bool getGlobalBoolean(const string &name);//返回值为接收变量
	int getGlobalInteger(const string &name);//返回值为接收变量
	bool getGlobalBoolean(const string &name,bool &value);
	bool getGlobalNumber(const string &name,double &value);
	bool getGlobalInteger(const string &name,int &value);
	bool getGlobalString(const string &name,string &value);
	bool getGlobalFunction(const string &name);
	//读栈顶变量
	const char* getTopString();//返回值为接收变量
	int getTopInteger();//返回值为接收变量
	bool getTopInteger(int &value);
	//枚举
	bool readEnum(const string &enumName,EnumType &enumType);//读取枚举类型enumName,存储到enumType中
	//表操作
	bool getGlobalTable(const string &name,function<bool()> callback=nullptr);//读取全局表,成功后调用readFunc来读取数据(针对元素基本确定的表)
	int getTableLength(function<bool()> filterFunc=nullptr);//获取table的元素个数(filterFunc用于过滤)
	bool getTableForEach(function<bool(int index)> forEachFunc=nullptr);
	//读取表中变量(变量名name,接收变量value),返回是否成功
	bool getTableBoolean(const string &name);//返回值为接收变量
	int getTableInteger(const string &name);//返回值为接收变量
	bool getTableBoolean(const string &name, bool &value);
	bool getTableNumber(const string &name,double &value);
	bool getTableInteger(const string &name,int &value);
	bool getTableString(const string &name,string &value);
	bool getTableTable(const string &name,function<bool()> callback=nullptr);//获取当前table中名为name的table变量,获取成功时调用callback
	//函数
	void registerFunction(const char *name,lua_CFunction func);//注册函数,把lua的名为name的函数和C函数func关联起来
	LuaState& push(const string &para);//添加函数参数(字符串类型),返回对象本身
	LuaState& push(int num);//添加函数参数(整数类型),返回对象本身
	//清理
	void clearStack();

	//全局状态
	size_t memorySize();//lua目前的占用大小
	//错误信息
	WhenErrorString whenError;//错误信息回调函数
private:
	lua_State *luaState;//Lua的栈
	int paraAmount;//参数数量,执行函数调用时用
};
#endif