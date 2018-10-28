#include"LuaState.h"

LuaState::LuaState():luaState(luaL_newstate()){}
LuaState::~LuaState(){lua_close(luaState);}

const char* LuaState::doFile(const string &filename){
	if(luaL_dofile(luaState,filename.data())!=LUA_OK){
		return lua_tostring(luaState,-1);
	}
	return nullptr;
}