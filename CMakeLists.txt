cmake_minimum_required(VERSION 2.8)
include(../CompileScripts/CMakeLists.txt)

#指定存放源码和头文件的所有路径(游戏工程文件可能会include此文件)
set(INCLUDE_LIST
	. #当前目录
	Cpp11 Cpp11/Containers #C++11模块,主要调用系统函数
	Lua #Lua读取游戏配置的模块,用于解读Lua数据并存入C++内存中
	Network #网络模块,有可能会封装系统模块或者第三方库
	Math #数学模块,定义数学上常用的概念,可能包含一些常用算法
	WarChess #战棋模块,战棋游戏专用
)

#系统头文件包含目录
foreach(name ../lua/src)
	include_directories(${name})
endforeach()

#默认库(包括源码编译的第三方库)
if(${TARGET_SYSTEM_NAME} STREQUAL Windows)
	set(LIBS_LIST lua53 curl ws2_32 z iconv pthread opengl32)
else()
	set(LIBS_LIST lua5.3 curl z GL) #lua5.3是当前版本,请自行修改lua的版本号
endif()

#编译过程
if(INCLUDE_DIR_ONLY)#仅包含目录
	includeDirAll(../libGamesEngines/)
elseif(COMPILE_GAME_LIB)#编译依赖本引擎的游戏库
	message(编译游戏库${GAME_NAME})
	project(${GAME_NAME})
	includeDirAll(../libGamesEngines/)
	#库输出
	add_library(${PROJECT_NAME} SHARED ${SRC_LIST})
	target_link_libraries(${PROJECT_NAME} ${LIBS_LIST} GamesEngines)
else()#编译本引擎库
	message(编译引擎库)
	project(GamesEngines)
	srcIncludeAll()
	#库输出
	add_library(${PROJECT_NAME} SHARED ${SRC_LIST})
	target_link_libraries(${PROJECT_NAME} ${LIBS_LIST})
endif()
