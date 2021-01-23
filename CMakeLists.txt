cmake_minimum_required(VERSION 2.8)
include(../CompileScripts/CMakeLists.txt)

#指定存放源码和头文件的所有路径(游戏工程文件可能会include此文件)
set(ENGINE_DIR
	. #当前目录
	Cpp11 Cpp11/Containers #C++11模块,主要调用系统函数
	Lua #Lua读取游戏配置的模块,用于解读Lua数据并存入C++内存中
	Algorithm #算法模块,主要放入游戏中可能用到的各种算法
	Network #网络模块,有可能会封装系统模块或者第三方库
	Math #数学模块,定义数学上常用的概念,可能包含一些常用算法
	WarChess #战棋模块,战棋游戏专用
)

#第三方库头文件包含目录
foreach(name ../lua/src)
	include_directories(${name})
endforeach()

#默认库(包括源码编译的第三方库)
set(LIBS_LIST curl lua z)

#编译过程
if(INCLUDE_DIR_ONLY) #仅包含头文件,不编译
	if(GAME_NAME)
		includeAll(../lib${GAME_NAME}/Game/ ${GAME_DIR})
	endif()
	includeAll(../libGamesEngines/ ${ENGINE_DIR})
else() #编译库
	message(编译引擎库)
	project(GamesEngines)
	#基础引擎
	srcIncludeAll(engineSrcs ./ ${ENGINE_DIR})
	add_library(${PROJECT_NAME} SHARED ${engineSrcs})
	target_link_libraries(${PROJECT_NAME} ${LIBS_LIST})
	#特定游戏库
	if(GAME_NAME)
		include(../lib${GAME_NAME}/CMakeLists.txt)#读取游戏的CMakeLists.txt
		srcIncludeAll(gameSrcs ../lib${GAME_NAME}/Game/ ${GAME_DIR})
		add_library(${GAME_NAME} SHARED ${gameSrcs})
		target_link_libraries(${GAME_NAME} ${PROJECT_NAME} ${LIBS_LIST})
		#编译工具库
		if(COMPILE_TOOLS)
			srcIncludeAll(toolsSrcs ../lib${GAME_NAME}/Tools/ ${TOOLS_DIR})
			#库输出
			add_library(${GAME_NAME}Tools SHARED ${toolsSrcs})
			target_link_libraries(${GAME_NAME}Tools ${GAME_NAME} GamesEngines ${LIBS_LIST})
			#输出工具集
			aux_source_directory(../lib${GAME_NAME}/Tools/executable exeList)
			foreach(exePath ${exeList})
				string(REPLACE ../lib${GAME_NAME}/Tools/executable/ "" exeName ${exePath})#去掉路径
				string(REPLACE .cpp "" exeName ${exeName})#去掉扩展名,得到程序名
				#程序输出
				add_executable(${exeName} ${exePath})
				target_link_libraries(${exeName} ${GAME_NAME}Tools ${GAME_NAME} GamesEngines ${LIBS_LIST})
			endforeach()
		endif()
	endif()
endif()