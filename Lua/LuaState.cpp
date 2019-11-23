#include"LuaState.h"

//此宏用于code会返回LUA_OK的代码,返回非LUA_OK则会告诉上层出错了
#define LUASTATE_EXECUTE(code)\
if(code!=LUA_OK){\
	if(whenError)whenError(lua_tostring(luaState,-1));\
	return false;\
}

//对code进行断言,断言失败则会返回错误信息
#define ASSERT(code,errStr)\
if(!code){\
	if(whenError)whenError(errStr);\
	return false;\
}

LuaState::LuaState():whenError(nullptr),luaState(luaL_newstate()){}
LuaState::~LuaState(){lua_close(luaState);}

bool LuaState::loadFile(const string &filename){
	LUASTATE_EXECUTE(luaL_loadfile(luaState,filename.data()));
	return true;
}
bool LuaState::protectCall(){
	LUASTATE_EXECUTE(lua_pcall(luaState,0,LUA_MULTRET,0));
	return true;
}
bool LuaState::doFile(const string &filename){
	LUASTATE_EXECUTE(luaL_dofile(luaState,filename.data()));
	return true;
}

bool LuaState::setGlobalInteger(const string &name,int value){
	lua_pushinteger(luaState,value);
	lua_setglobal(luaState,name.data());
	return true;
}
bool LuaState::getGlobalInteger(const string &name,int &value){
	bool ret=lua_getglobal(luaState,name.data())==LUA_TNUMBER;
	if(ret){
		value=lua_tointeger(luaState,1);
	}else{
		if(whenError)whenError("\""+name+"\" not a number");
	}
	lua_pop(luaState,1);
	return ret;
}
bool LuaState::setGlobalString(const string &name, const string &value){
	lua_pushstring(luaState,value.data());
	lua_setglobal(luaState,name.data());
	return true;
}
bool LuaState::getGlobalString(const string &name,string &value){
	bool ret=lua_getglobal(luaState,name.data())==LUA_TSTRING;
	if(ret){
		value=lua_tostring(luaState,1);
	}else{
		if(whenError)whenError("\""+name+"\" not a string");
	}
	lua_pop(luaState,1);
	return ret;
}
bool LuaState::toInteger(int index,int &value){
	bool b=lua_isinteger(luaState,index);
	if(b)value=lua_tointeger(luaState,index);
	return b;
}

bool LuaState::readEnum(const string &enumName,EnumType &enumType){
	ASSERT((lua_getglobal(luaState,enumName.data())==LUA_TTABLE),"\""+enumName+"\" is not a table");//获取表名
	//统计个数
	EnumType::amountType amount=0;
	LUASTATE_TABLE_FOREACH(luaState,
		ASSERT(lua_isnumber(luaState,-2)&&lua_isstring(luaState,-1),
			"\""+enumName+"["+lua_tostring(luaState,-2)+"]\" is not enum value?");
		++amount;
	);
	//开始添加
	enumType.setAmount(amount);
	amount=0;//循环用
	LUASTATE_TABLE_FOREACH(luaState,
		enumType.setEnumName(amount,lua_tostring(luaState,-1));
		++amount;
	);
	lua_pop(luaState,1);
	return true;
}
bool LuaState::getGlobalTable(const string &name){
	lua_getglobal(luaState,name.data());
	return lua_istable(luaState,1);
}
void LuaState::registerFunction(const char *name,lua_CFunction func){lua_register(luaState,name,func);}
void LuaState::clearStack(){
	lua_pop(luaState,lua_gettop(luaState));
}