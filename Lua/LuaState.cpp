#include"LuaState.h"

#define LUASTATE_EXECUTE(code)\
if(code!=LUA_OK){\
errorString=lua_tostring(luaState,-1);\
return false;\
}

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
	LUASTATE_EXECUTE(lua_getglobal(luaState,enumName.data()));//获取表名
	ASSERT(lua_gettop(luaState)!=1,"\""+enumName+"\" is not exist");//名字必须存在
	ASSERT(!lua_istable(luaState,-1),"\""+enumName+"\" is not a table");//必须是表
	//统计个数
	EnumType::amountType amount=0;
	lua_pushnil(luaState);//开始遍历
	while(lua_next(luaState,-2)){
		//要求lua代码格式:表名={枚举名1,枚举名2,...};
		ASSERT(lua_isnumber(luaState,-2)&&lua_isstring(luaState,-1),
			"\""+enumName+"["+lua_tostring(luaState,-2)+"]\" is not enum value?")
		++amount;
		lua_pop(luaState,1);//下一个
	}
	//开始添加
	enumType.setAmount(amount);
	amount=0;//循环用
	lua_pushnil(luaState);
	while(lua_next(luaState,-2)){
		enumType.setEnumName(amount,lua_tostring(luaState,-1));
		++amount;
		lua_pop(luaState,1);//下一个
	}
	return true;
}