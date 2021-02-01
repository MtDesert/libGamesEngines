#常量
engineName:=GamesEngines
engineDir:=lib$(engineName)
engineSrcDir:=Cpp11 Cpp11/Containers Lua Math Network WarChess
engineIncDir:=../zlib ../lua/src ../curl/include
userLibs+=lua curl
sysLibs+=z pthread
ifeq ($(DEST_PLATFORM),$(findstring $(DEST_PLATFORM),MinGW Windows))
	sysLibs+=ws2_32
endif
#确定要编译的源文件(
#有游戏名时,编译{游戏内核|游戏客户端|游戏服务端|游戏工具集}之一
#无游戏名时,编译引擎源码)
ifeq ($(GAME_NAME),)#无游戏名,编译通用部分
	ifeq ($(Client)$(Server)$(Tools),)#编译引擎
		allSrcDir:=$(engineSrcDir)
		allIncDir:=$(engineIncDir)
		libName:=$(engineName)
	else ifeq ($(Client),true)#编译通用客户端
		allIncDir+=$(foreach name,$(engineSrcDir),../$(engineDir)/$(name)) $(engineIncDir)
		userLibs+=$(engineName)
		ifeq ($(DEST_PLATFORM),MinGW)
			sysLibs+=gdi32 iconv
		endif
	else ifeq ($(Server),true)#编译服务端(待定)
	else ifeq ($(Tools),true)#编译服务端(待定)
	else
		$(error 引擎制造错误，请检查Makefile中的选项)
	endif
else #编译特定游戏
	ifeq ($(Game),true)#编译游戏内核
		allSrcDir:=$(foreach name,$(GAME_DIR),Game/$(name))
		libName:=$(GAME_NAME)
	else #编译游戏的其它依赖库,则仅包含即可
		allIncDir+=$(foreach name,$(GAME_DIR),../lib$(GAME_NAME)/Game/$(name))
	endif
	allIncDir+=$(foreach name,$(engineSrcDir),../$(engineDir)/$(name)) $(engineIncDir)
	userLibs+=$(engineName)
endif
libName:=$(if $(exeName),,$(libName))#编译执行程序的时候消除库名

include ../CompileScripts/Makefile