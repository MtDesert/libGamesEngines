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

if(${TARGET_SYSTEM_NAME} STREQUAL Android)
	include_directories(${NDK_ROOT}/sysroot/usr/include/GLES)
elseif(${TARGET_SYSTEM_NAME} STREQUAL Windows)
else()
	include_directories(/usr/include/GL)
endif()

#默认库(包括源码编译的第三方库)
set(LIBS_LIST curl lua z)
if(${TARGET_SYSTEM_NAME} STREQUAL Windows)
	set(LIBS_LIST ${LIBS_LIST} ws2_32 iconv pthread opengl32)
else()
	set(LIBS_LIST ${LIBS_LIST} GLESv1_CM)
endif()

#编译过程
if(INCLUDE_DIR_ONLY)
	if(GAME_NAME)
		includeAll(../lib${GAME_NAME}/Game/ ${GAME_DIR})
	endif()
	includeAll(../libGamesEngines/ ${ENGINE_DIR})
elseif(GAME_NAME)#编译依赖本引擎的游戏库
	if(COMPILE_TOOLS)
		message(编译工具集合${GAME_NAME})
		project(${GAME_NAME}Tools)
		#读取工程文件
		include(../lib${GAME_NAME}/CMakeLists.txt)#读取游戏的CMakeLists.txt
		srcIncludeAll(../lib${GAME_NAME}/Tools/ ${TOOLS_DIR})
		includeAll(../lib${GAME_NAME}/Game/ ${GAME_DIR})
		includeAll(./ ${ENGINE_DIR})
		#设定输出目录
		set(LIBRARY_OUTPUT_PATH ${LIBRARY_OUTPUT_PATH}/tools)
		set(EXECUTABLE_OUTPUT_PATH ${LIBRARY_OUTPUT_PATH})
		message(${LIBRARY_OUTPUT_PATH})
		message(${EXECUTABLE_OUTPUT_PATH})
		#库输出
		add_library(${PROJECT_NAME} SHARED ${SRC_LIST})
		target_link_libraries(${PROJECT_NAME} GamesEngines ${LIBS_LIST})
		#输出工具集
		aux_source_directory(../lib${GAME_NAME}/Tools/executable EXE_LIST)
		foreach(exePath ${EXE_LIST})
			string(REPLACE ../lib${GAME_NAME}/Tools/executable/ "" exeName ${exePath})#去掉路径
			string(REPLACE .cpp "" exeName ${exeName})#去掉扩展名,得到程序名
			#程序输出
			add_executable(${exeName} ${exePath})
			target_link_libraries(${exeName} ${PROJECT_NAME} ${GAME_NAME} GamesEngines ${LIBS_LIST})
		endforeach()
	else()
		message(编译游戏库${GAME_NAME})
		project(${GAME_NAME})
		#读取工程文件
		include(../lib${GAME_NAME}/CMakeLists.txt)#读取游戏的CMakeLists.txt
		srcIncludeAll(../lib${GAME_NAME}/Game/ ${GAME_DIR})
		includeAll(./ ${ENGINE_DIR})
		#库输出
		add_library(${PROJECT_NAME} SHARED ${SRC_LIST})
		target_link_libraries(${PROJECT_NAME} GamesEngines ${LIBS_LIST})
	endif()
else()#编译本引擎库
	message(编译引擎库)
	project(GamesEngines)
	srcIncludeAll(./ ${ENGINE_DIR})
	#库输出
	add_library(${PROJECT_NAME} SHARED ${SRC_LIST})
	target_link_libraries(${PROJECT_NAME} ${LIBS_LIST})
endif()