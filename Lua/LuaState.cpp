#include"LuaState.h"

//此宏用于table的遍历,用code处理每一个table的元素
#define TABLE_FOREACH(code)\
lua_pushnil(luaState);\
while(lua_next(luaState,-2)){\
	code;\
	lua_pop(luaState,1);/*下一个*/\
}\

//此宏用于code会返回LUA_OK的代码,返回非LUA_OK则会告诉上层出错了
#define LUASTATE_EXECUTE(code)\
if(code!=LUA_OK){\
errorString=lua_tostring(luaState,-1);\
return false;\
}

//对code进行断言,断言失败则会返回错误信息
#define ASSERT(code,errStr)\
if(code);else{\
errorString=errStr;\
return false;\
}

LuaState::LuaState():luaState(luaL_newstate()){}
LuaState::~LuaState(){lua_close(luaState);}

bool LuaState::doFile(const string &filename){
	LUASTATE_EXECUTE(luaL_dofile(luaState,filename.data()))
	return true;
}

bool LuaState::readEnum(const string &enumName,EnumType &enumType){
	lua_getglobal(luaState,enumName.data());//获取表名
	ASSERT(lua_istable(luaState,-1),"\""+enumName+"\" is not a table");//必须是表
	//统计个数
	EnumType::amountType amount=0;
	TABLE_FOREACH(
		ASSERT(lua_isnumber(luaState,-2)&&lua_isstring(luaState,-1),
			"\""+enumName+"["+lua_tostring(luaState,-2)+"]\" is not enum value?");
		++amount;
	);
	//开始添加
	enumType.setAmount(amount);
	amount=0;//循环用
	TABLE_FOREACH(
		enumType.setEnumName(amount,lua_tostring(luaState,-1));
		++amount;
	);
	return true;
}