#常量
engineName:=GamesEngines
engineDir:=lib$(engineName)
engineSrcDir:=Cpp11 Cpp11/Containers Lua Math Network WarChess
engineIncDir:=../zlib ../lua/src ../curl/include
userLibs:=lua curl
sysLibs:=z pthread
ifeq ($(DEST_PLATFORM),MinGW)
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
	ifeq ($(Client)$(Server)$(Tools),)#编译游戏内核
		allSrcDir:=$(foreach name,$(GAME_DIR),Game/$(name))
		allIncDir:=$(foreach name,$(engineSrcDir),../$(engineDir)/$(name)) $(engineIncDir)
		userLibs+=$(engineName)
		libName:=$(GAME_NAME)
	else ifeq ($(Client),true)#编译客户端
		allSrcDir:=$(foreach name,$(CLIENT_DIR),Client/$(name))
	else ifeq ($(Server),true)#编译服务端(待定)
		allSrcDir:=$(foreach name,$(SERVER_DIR),Server/$(name))
	else ifeq ($(Tools),true)#编译工具集(待定)
		allSrcDir:=$(foreach name,$(TOOLS_DIR),Tools/$(name))
	else
		$(error $(GAME_NAME)制造错误，请检查Makefile中的选项)
	endif
endif
libName:=$(if $(exeName),,$(libName))#编译执行程序的时候消除库名

include ../CompileScripts/Makefile