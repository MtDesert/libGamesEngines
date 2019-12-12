#ifndef GAMESCRIPT_H
#define GAMESCRIPT_H

#include"LuaState.h"
#include"Thread.h"

//#define GAME_SCRIPT_FUNCTION_DECL(name) static int name(lua_State *state);

#define GAMESCRIPT_ALL_STRING_VAR(MACRO)\
MACRO(strSay)\
MACRO(strBody)\
MACRO(strSceneName)

//游戏脚本,用于执行特定的游戏代码
//工作原理:基于lua脚本,以lua调用C++代码的方式执行,并且以线程的方式去执行调度
//开发目的:针对RPG类型的游戏,用于执行特定剧情脚本,比如需要显示对话又要执行各种复杂动作的场合
class GameScript{
public:
	GameScript();
	~GameScript();
	//
	static GameScript *gameScript;

	//脚本变量,从脚本中解读的结果,提供给控件读取
#define STRING_DECL(name) const char* name;
	GAMESCRIPT_ALL_STRING_VAR(STRING_DECL)
#undef STRING_DECL
	//错误处理
	decltype(LuaState::whenError) whenError;

	//脚本控制
	bool executeSenarioScript(const string &filename);//执行剧情脚本
	void scriptWait(uint msec=0);//脚本等待(等待毫秒数),直到isScriptWaiting为false
	void scriptContinue();//脚本从等待处继续执行
protected:
	//脚本模块
	LuaState *luaScript;//剧情脚本
	Thread senarioScriptThread;//脚本线程
	bool isScriptWaiting;//脚本线程暂停,控制剧情脚本执行速度
	virtual void scriptInit();//执行脚本前先对环境进行初始化(子类初始化)
};
#endif