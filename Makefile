#常量
engineName:=GamesEngines
engineDir:=lib$(engineName)
engineSrcDir:=Cpp11 Cpp11/Containers Zipper Lua Math Network Images Images/FileStructs Images/ColorSpaces WarChess
engineIncDir:=../zlib ../lua/src ../curl/include
userLibs+=lua
sysLibs+=z pthread
ifeq ($(DEST_PLATFORM),$(findstring $(DEST_PLATFORM),MinGW Windows))
	sysLibs+=ws2_32
endif
#确定要编译的源文件(
#有游戏名时,编译{游戏内核|游戏客户端|游戏服务端|游戏工具集}之一
#无游戏名时,编译引擎源码)
ifeq ($(GAME_NAME),)#无游戏名,编译通用部分
	ifeq ($(projectPath),lib$(engineName))#编译引擎
		allSrcDir:=$(engineSrcDir)
		allIncDir:=$(engineIncDir)
		libName:=$(engineName)
	else
		allIncDir+=$(foreach name,$(engineSrcDir),../$(engineDir)/$(name)) $(engineIncDir)
		userLibs+=$(engineName)
		ifeq ($(DEST_PLATFORM),MinGW)
			sysLibs+=gdi32 iconv
		endif
	endif
else #编译特定游戏
	ifeq ($(gameProjectPath),Engines)#编译游戏内核
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