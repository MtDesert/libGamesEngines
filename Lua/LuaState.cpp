#include"LuaState.h"
#include"PrintF.h"
#include"define.h"

//调试函数
void luaStackDebug(lua_State *state){
	auto top=lua_gettop(state);
	PRINT_INFO("--------lua栈开始\n");
	PRINT_INFO("state: %p\n",state);
	PRINT_INFO("栈元素个数: %d\n",top);
	for(auto i=-1;i>=-top;--i){
		auto type=lua_type(state,i);
		PRINT_INFO("%d(%s): ",i,lua_typename(state,type));
		switch(type){
			case LUA_TNUMBER:PRINT_INFO("%lf",lua_tonumber(state,i));
			break;
			case LUA_TSTRING:PRINT_INFO("%s",lua_tostring(state,i));
			break;
			case LUA_TTABLE:
			break;
		}
		PRINT_INFO("\n");
	}
	PRINT_INFO("--------lua栈结束\n");
}

//此宏用于code会返回LUA_OK的代码,返回非LUA_OK则会告诉上层出错了
#define LUASTATE_EXECUTE(code)\
if((code)!=LUA_OK){\
	WHEN_ERROR(lua_tostring(luaState,-1));\
	return false;\
}

LuaState::LuaState():LuaState(luaL_newstate()){
	needClose=true;
	luaopen_base(luaState);//只允许使用基础功能,防止恶意代码通过系统调用
}
LuaState::LuaState(lua_State *another):luaState(another),needClose(false),paraAmount(0),whenError(nullptr){}
LuaState::~LuaState(){
	if(needClose)lua_close(luaState);
}

bool LuaState::loadFile(const string &filename){
	LUASTATE_EXECUTE(luaL_loadfile(luaState,filename.data()));
	return true;
}
bool LuaState::protectCall(){
	LUASTATE_EXECUTE(lua_pcall(luaState,paraAmount,LUA_MULTRET,0));
	return true;
}
bool LuaState::resume(){
	auto ret=lua_resume(luaState,NULL,0);
	if(ret==LUA_YIELD)ret=LUA_OK;//这俩返回值是正常的
	LUASTATE_EXECUTE(ret);
	return true;
}
bool LuaState::yield(){
	auto b=lua_isyieldable(luaState);
	if(b)lua_yield(luaState,0);
	//不会执行到这里,调用lua_yield后,LuaState::resume()的lua_resume()会执行结束并返回
	return b;
}
bool LuaState::doFile(const string &filename){
	LUASTATE_EXECUTE(luaL_dofile(luaState,filename.data()));
	return true;
}
bool LuaState::doString(const string &str){
	LUASTATE_EXECUTE(luaL_dostring(luaState,str.data()));
	return true;
}

//读写变量
void LuaState::setGlobalBoolean(const string &name,bool value){
	lua_pushboolean(luaState,value);
	lua_setglobal(luaState,name.data());
}
void LuaState::setGlobalNumber(const string &name,double value){
	lua_pushnumber(luaState,value);
	lua_setglobal(luaState,name.data());
}
void LuaState::setGlobalInteger(const string &name,int value){
	lua_pushinteger(luaState,value);
	lua_setglobal(luaState,name.data());
}
void LuaState::setGlobalString(const string &name,const string &value){
	lua_pushstring(luaState,value.data());
	lua_setglobal(luaState,name.data());
}
void LuaState::setGlobalLightUserData(const string &name,void *data){
	lua_pushlightuserdata(luaState,data);
	lua_setglobal(luaState,name.data());
}

#define LUASTATE_GET_GLOBAL_TYPE(type,toTypeFunc,errMsg)\
bool ret=lua_getglobal(luaState,name.data())==type;\
if(ret){\
	value=toTypeFunc(luaState,-1);\
}else{\
	WHEN_ERROR(errMsg);\
}\
lua_pop(luaState,1);\
return ret;

#define LUASTATE_GET_TYPE(type,toTypeFunc,errMsg)\
bool ret=lua_type(luaState,index)==type;\
if(ret){\
	value=toTypeFunc(luaState,index);\
}else{\
	WHEN_ERROR(errMsg);\
}\
return ret;

bool LuaState::getGlobalBoolean(const string &name){
	bool ret=false;
	getGlobalBoolean(name,ret);
	return ret;
}
int LuaState::getGlobalInteger(const string &name){
	int ret=0;
	getGlobalInteger(name,ret);
	return ret;
}

bool LuaState::getGlobalBoolean(const string &name,bool &value){
	LUASTATE_GET_GLOBAL_TYPE(LUA_TBOOLEAN,lua_toboolean,"\""+name+"\" not a boolean")
}
bool LuaState::getGlobalNumber(const string &name,double &value){
	LUASTATE_GET_GLOBAL_TYPE(LUA_TNUMBER,lua_tonumber,"\""+name+"\" not a number")
}
bool LuaState::getGlobalInteger(const string &name,int &value){
	LUASTATE_GET_GLOBAL_TYPE(LUA_TNUMBER,lua_tointeger,"\""+name+"\" not an integer")
}
bool LuaState::getGlobalString(const string &name,string &value){
	LUASTATE_GET_GLOBAL_TYPE(LUA_TSTRING,lua_tostring,"\""+name+"\" not a string")
}
bool LuaState::getGlobalFunction(const string &name){
	bool ret=lua_getglobal(luaState,name.data())==LUA_TFUNCTION;
	ASSERT(ret,"\""+name+"\" not a function");
	paraAmount=0;
	return ret;
}

#include"Number.h"
#define ERROR_STATEMEMT(para) "Index "+Number::toString(index)+" not a "#para

bool LuaState::getBoolean(int index,bool &value){
	LUASTATE_GET_TYPE(LUA_TBOOLEAN,lua_toboolean,ERROR_STATEMEMT(boolean))
}
bool LuaState::getNumber(int index,double &value){
	LUASTATE_GET_TYPE(LUA_TNUMBER,lua_tonumber,ERROR_STATEMEMT(number))
}
bool LuaState::getInteger(int index,int &value){
	LUASTATE_GET_TYPE(LUA_TNUMBER,lua_tointeger,ERROR_STATEMEMT(integer))
}
bool LuaState::getString(int index,string &value){
	LUASTATE_GET_TYPE(LUA_TBOOLEAN,lua_tostring,ERROR_STATEMEMT(string))
}
bool LuaState::getLightUserData(int index,const void *&value){
	LUASTATE_GET_TYPE(LUA_TLIGHTUSERDATA,lua_topointer,ERROR_STATEMEMT(pointer))
}

const char *LuaState::getTopString(){
	const char* ret=nullptr;
	if(lua_isstring(luaState,-1)){
		ret=lua_tostring(luaState,-1);
	}
	return ret;
}
int LuaState::getTopInteger(){
	int ret=0;
	getTopInteger(ret);
	return ret;
}
bool LuaState::getTopBoolean(){
	bool ret=false;
	getTopBoolean(ret);
	return ret;
}

bool LuaState::getTopInteger(int &value){
	bool ret=lua_isinteger(luaState,-1);
	if(ret){
		value=lua_tointeger(luaState,-1);
	}
	return ret;
}
bool LuaState::getTopBoolean(bool &value){
	bool ret=lua_isboolean(luaState,-1);
	if(ret){
		value=lua_toboolean(luaState,-1);
	}
	return ret;
}

bool LuaState::readEnum(const string &enumName,EnumType &enumType){
	return getGlobalTable(enumName,[this,&enumType](){
		enumType.setAmount(getTableLength());//确定枚举值数量
		return getTableForEach([this,&enumType](int index){
			return enumType.setEnumName(index,getTopString());//设置枚举值
		});
	});
}
bool LuaState::getGlobalTable(const string &name,function<bool()> callback){
	//获取表
	bool ret=lua_getglobal(luaState,name.data())==LUA_TTABLE;
	ASSERT(ret,"\""+name+"\" is not a table");
	if(ret && callback)ret=callback();
	return ret;
}
int LuaState::getTableLength(function<bool()> filterFunc){
	int ret=0;
	if(lua_istable(luaState,-1)){
		lua_pushnil(luaState);
		while(lua_next(luaState,-2)){
			if(!filterFunc || filterFunc())++ret;//无条件累加,或根据条件累加
			lua_pop(luaState,1);
		}
	}
	return ret;
}
bool LuaState::getTableForEach(function<bool(int)> forEachFunc){
	if(forEachFunc && lua_istable(luaState,-1)){
		bool ret=true;//目前还没发现错误
		lua_pushnil(luaState);
		auto idx=0;
		while(lua_next(luaState,-2)){
			if(forEachFunc(idx))++idx;//调用回调函数
			else ret=false;
			lua_pop(luaState,1);//下一个
		}
		return ret;
	}
	return false;
}

#define LUASTATE_GET_TABLE_TYPE(type,toTypeFunc,errMsg)\
if(!lua_istable(luaState,-1))return false;/*如果放入函数则会引发崩溃*/\
lua_pushstring(luaState,name.data());\
bool ret=(lua_gettable(luaState,-2)==type);\
if(ret){\
	value=toTypeFunc(luaState,-1);\
}\
lua_pop(luaState,1);/*无论是否成功都要pop*/\
return ret;

bool LuaState::getTableBoolean(const string &name){
	bool ret=false;
	getTableBoolean(name,ret);
	return ret;
}
int LuaState::getTableInteger(const string &name){
	int ret=0;
	getTableInteger(name,ret);
	return ret;
}
void* LuaState::getTableUserData(const string &name){
	void* ret=nullptr;
	getTableUserData(name,ret);
	return ret;
}
void* LuaState::getTableLightUserData(const string &name){
	void* ret=nullptr;
	getTableLightUserData(name,ret);
	return ret;
}

bool LuaState::getTableBoolean(const string &name,bool &value){
	LUASTATE_GET_TABLE_TYPE(LUA_TBOOLEAN,lua_toboolean,"\""+name+"\" not a boolean")
}
bool LuaState::getTableNumber(const string &name,double &value){
	LUASTATE_GET_TABLE_TYPE(LUA_TNUMBER,lua_tonumber,"\""+name+"\" not a number")
}
bool LuaState::getTableInteger(const string &name,int &value){
	LUASTATE_GET_TABLE_TYPE(LUA_TNUMBER,lua_tointeger,"\""+name+"\" not an integer")
}
bool LuaState::getTableString(const string &name,string &value){
	LUASTATE_GET_TABLE_TYPE(LUA_TSTRING,lua_tostring,"\""+name+"\" not a string")
}
bool LuaState::getTableFunction(const string &name){
	if(!lua_istable(luaState,-1))return false;/*如果放入函数则会引发崩溃*/\
	lua_pushstring(luaState,name.data());\
	bool ret=(lua_gettable(luaState,-2)==LUA_TFUNCTION);
	ASSERT(ret,"\""+name+"\" not a function");
	paraAmount=0;
	return ret;
}
bool LuaState::getTableUserData(const string &name, void *&value){
	LUASTATE_GET_TABLE_TYPE(LUA_TUSERDATA,lua_touserdata,"\""+name+"\" not a userdata")
}
bool LuaState::getTableLightUserData(const string &name, void *&value){
	LUASTATE_GET_TABLE_TYPE(LUA_TLIGHTUSERDATA,lua_touserdata,"\""+name+"\" not a lightuserdata")
}
bool LuaState::getTableTable(const string &name,function<bool()> callback){
	if(!lua_istable(luaState,-1))return false;/*如果放入函数则会引发崩溃*/
	lua_pushstring(luaState,name.data());
	bool ret=(lua_gettable(luaState,-2)==LUA_TTABLE);
	if(ret){
		if(callback)ret=callback();
	}
	lua_pop(luaState,1);
	return ret;
}

void LuaState::registerFunction(const char *name,lua_CFunction func){lua_register(luaState,name,func);}
LuaState& LuaState::push(const string &para){
	if(lua_pushstring(luaState,para.data()))++paraAmount;
	return *this;
}
LuaState& LuaState::push(int num){
	lua_pushinteger(luaState,num);
	++paraAmount;
	return *this;
}
LuaState& LuaState::push(void *userData){
	lua_pushlightuserdata(luaState,userData);
	++paraAmount;
	return *this;
}
void LuaState::registerClass(const char *name,lua_CFunction constructorFunc){
	clearStack();
	lua_newtable(luaState);//用table来模拟类
	addClassFunction("new",constructorFunc);
	lua_setglobal(luaState,name);
}
void LuaState::addClassObjectSelf(void *self){
	clearStack();
	lua_newtable(luaState);//用table来模拟类对象
	if(lua_pushstring(luaState,"self")){
		lua_pushlightuserdata(luaState,self);
		lua_settable(luaState,-3);
	}else{
		WHEN_ERROR("addClassObjectSelf() failed");
	}
}
void LuaState::addClassFunction(const char *name,lua_CFunction func){
	if(lua_pushstring(luaState,name)){
		lua_pushcfunction(luaState,func);
		lua_settable(luaState,-3);
	}else{
		WHEN_ERROR(string("registerClassFunction(\"")+name+"\")failed");
	}
}
void* LuaState::getClassObjectSelf(int index){
	if(!lua_istable(luaState,index))return nullptr;//首先必须是table
	lua_pushstring(luaState,"self");//只取self变量
	if(lua_gettable(luaState,index)==LUA_TLIGHTUSERDATA){//确保该变量是由C++传进来的
		void* self=lua_touserdata(luaState,-1);
		lua_pop(luaState,1);
		return self;
	}
	return nullptr;
}

void LuaState::clearStack(int remain){
	auto top=lua_gettop(luaState);
	if(remain>top)return;//不需要清除
	else if(remain<top){
		top-=remain;//根据保留数确定要弹出的数量
	}
	lua_pop(luaState,top);
}
size_t LuaState::memorySize(){
	auto kb=lua_gc(luaState,LUA_GCCOUNT,0);
	auto b=lua_gc(luaState,LUA_GCCOUNTB,0);
	return (kb<<10)+b;
}
void LuaState::luaStackDebug(lua_State *state)const{
	if(!state)state=luaState;
	::luaStackDebug(state);
}