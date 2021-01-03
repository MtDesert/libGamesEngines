#引擎源码目录
engineSrcDir += Cpp11 Cpp11/Containers Lua Math Network WarChess
#确定要编译的源文件(
#有游戏名时,编译{游戏内核|游戏客户端|游戏服务端|游戏工具集}之一
#无游戏名时,编译引擎源码)
allSrcDir += $(if $(GAME_NAME),\
	$(if $(Client)$(Server)$(Tools),,$(foreach name,$(GAME_DIR),Game/$(name)))\
	$(if $(Client),$(foreach name,$(CLIENT_DIR),Client/$(name)),)\
	$(if $(Server),$(foreach name,$(SERVER_DIR),Server/$(name)),)\
	$(if $(Tools),$(foreach name,$(TOOLS_DIR),Tools/$(name)),)\
	,\
	$(if $(Client)$(Server)$(Tools),,$(engineSrcDir)))

allSrcDir := $(if $(exeName),,$(allSrcDir)) #编译执行程序的时候消除源目录
#包含目录
allIncDir += $(if $(GAME_NAME),\
	$(if $(Client),\
		$(foreach name,$(GAME_DIR),$(gameProjectDir)/Game/$(name)),\
		),\
	) #添加游戏内核目录
allIncDir += $(if $(GAME_NAME)$(Client),\
	$(foreach name,$(engineSrcDir),../libGamesEngines/$(name)),\
	) #添加引擎包含目录
allIncDir += ../zlib ../lua/src ../curl/include #添加第三方库目录
#库文件
userLibs += $(if $(GAME_NAME),$(if $(Client),GamesClient $(GAME_NAME),),) #添加客户端库,游戏库
userLibs += $(if $(GAME_NAME)$(Client),GamesEngines,) #添加引擎库
userLibs += lua curl
sysLibs += ws2_32 pthread
#库名称
libName := $(if $(GAME_NAME),\
	$(if $(Client),$(GAME_NAME)Client,$(GAME_NAME)),\
	$(if $(Client),GamesClient,GamesEngines))
libName := $(if $(exeName),,$(libName)) #编译执行程序的时候消除库名
include ../CompileScripts/Makefile