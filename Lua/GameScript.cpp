#include"GameScript.h"

//默认的函数
#define DEFAULT_SCRIPT_FUNCTION(MACRO)\
MACRO(say)\
MACRO(bodySay)\
MACRO(gotoScene)

GameScript* GameScript::gameScript=nullptr;
GameScript::GameScript():whenError(nullptr),luaScript(nullptr),isScriptWaiting(false){}
GameScript::~GameScript(){if(luaScript)delete luaScript;}

#define TO_STR(pStr,idx)\
GameScript::gameScript->pStr = lua_isstring(state,idx) ? lua_tostring(state,idx) : nullptr ;

//默认脚本函数
static int say(lua_State *state){
	TO_STR(strSay,1)
	GameScript::gameScript->strBody=nullptr;
	GameScript::gameScript->scriptWait();
	return 0;
}
static int bodySay(lua_State *state){
	TO_STR(strBody,1)
	TO_STR(strSay,2)
	GameScript::gameScript->scriptWait();
	return 0;
}
static int gotoScene(lua_State *state){
	TO_STR(strSceneName,1)
	GameScript::gameScript->scriptWait();
	return 0;
}

void* senarioScriptFunc(void *arg){
	auto script=reinterpret_cast<LuaState*>(arg);
	printf("剧情开始\n");
	script->protectCall();
	printf("剧情结束\n");
	return nullptr;
}

void GameScript::scriptInit(){
	//注册函数
#define CASE(name) luaScript->registerFunction(#name,name);
	DEFAULT_SCRIPT_FUNCTION(CASE)
#undef CASE
}
bool GameScript::executeSenarioScript(const string &filename){
	//初始化脚本环境
	if(!luaScript){
		luaScript=new LuaState();
		luaScript->whenError=whenError;
		scriptInit();//(初始化,子类重写)
	}
	//加载脚本文件
	if(luaScript->loadFile(filename)){
		return senarioScriptThread.start(senarioScriptFunc,luaScript);
	}
	return false;
}
void GameScript::scriptWait(uint msec){
	isScriptWaiting=true;
	while(isScriptWaiting){
		pthread_yield();
	}
}
void GameScript::scriptContinue(){isScriptWaiting=false;}