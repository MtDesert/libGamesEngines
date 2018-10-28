#ifndef LUASTATE_H
#define LUASTATE_H

#include"lua.hpp"
#include<string>
using namespace std;

struct LuaState{
	LuaState();
	~LuaState();

	const char* doFile(const string &filename);
private:
	lua_State *luaState;
};

#endif