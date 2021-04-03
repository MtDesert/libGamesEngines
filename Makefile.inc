#提取后缀
gameProjectPath=$(subst lib$(GAME_NAME)/,,$(projectPath))
#判断需要包含的Makefile
ifeq ($(gameProjectPath),Game)
	gameProjectPath=Engines
endif
include ../libGames$(gameProjectPath)/Makefile