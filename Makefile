#游戏工程目录
gameProjectDir := $(word 1,${MAKEFILE_LIST})
gameProjectDir := $(shell dirname $(gameProjectDir))
#引擎目录
engineDir := $(word $(words ${MAKEFILE_LIST}),${MAKEFILE_LIST})
engineDir := $(shell dirname $(engineDir))

#引擎源码目录
engineSrcDir += Cpp11 Cpp11/Containers Lua Math Network WarChess
#确定要编译的源文件(游戏引擎|特定游戏内核|特定游戏客户端)
allSrcDir += $(if $(GAME_NAME),\
	$(if $(Client),\
		$(foreach name,$(CLIENT_DIR),Client/$(name)),\
		$(foreach name,$(GAME_DIR),Game/$(name))),\
	$(if $(Client),,$(engineSrcDir)))
allSrcDir := $(if $(exeName),,$(allSrcDir)) #编译执行程序的时候消除源目录
#包含目录
allIncDir += $(if $(GAME_NAME),$(if $(Client),$(foreach name,$(GAME_DIR),$(gameProjectDir)/Game/$(name)),),) #添加游戏内核目录
allIncDir += $(if $(GAME_NAME)$(Client),$(foreach name,$(engineSrcDir),$(engineDir)/$(name)),) #添加引擎包含目录
allIncDir += ../zlib ../lua/src #添加第三方库目录
#库文件
userLibs += $(if $(GAME_NAME),$(if $(Client),GamesClient $(GAME_NAME),),) #添加客户端库,游戏库
userLibs += $(if $(GAME_NAME)$(Client),GamesEngines,) #添加引擎库
userLibs += lua
sysLibs += ws2_32 pthread
#库名称
libName := $(if $(GAME_NAME),\
	$(if $(Client),$(GAME_NAME)Client,$(GAME_NAME)),\
	$(if $(Client),GamesClient,GamesEngines))
libName := $(if $(exeName),,$(libName)) #编译执行程序的时候消除库名
include ../CompileScripts/Makefile