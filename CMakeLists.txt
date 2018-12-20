cmake_minimum_required(VERSION 2.8)
include(../CompileScripts/CMakeLists.txt)

#指定存放源码和头文件的所有路径(游戏工程文件可能会include此文件)
set(paths
	. #当前目录
	Cpp11 Cpp11/Containers #C++11模块,主要调用系统函数
	Lua #Lua读取游戏配置的模块,用于解读Lua数据并存入C++内存中
	Math #数学模块,定义数学上常用的概念,可能包含一些常用算法
	Zipper Zipper/FileStructs #压缩模块,负责解压
	Text #文本文件处理模块,用于读取各种文本格式
	Image Image/FileStructs Image/ColorSpaces #图像模块,主要用于图像解码和处理
	Font #字体处理模块,文字可以当作一种特殊的图像
	Renderer #渲染模块,主要用于控制显存和绘制
	Charset #字符集,处理字符转换过程
	WarChess #战棋模块,战棋游戏专用
	Widgets #控件集,类似于各种平台特有的控件
	Scenes #场景,提供公共的常见场景(如选择文件场景)
)

#系统头文件包含目录
foreach(name /usr/include/lua5.3 /usr/include/x86_64-linux-gnu/qt5)
	include_directories(${name})
endforeach()

#此文件可能会被别的CMakeLists.txt文件引用,一般是游戏项目的CMakeLists.txt文件,游戏项目在编译时必须设置此值
if(GAME_LIB)
	project(${GAME_NAME})
	#添加引擎的所有包含路径
	foreach(name ${paths})
		include_directories(../libGamesEngines/${name})
	endforeach()
	#编译成库
	add_library(${PROJECT_NAME} SHARED ${SRC_LIST})
else()
	project(GamesEngines)#编译整个引擎
	#添加所有paths下的源码和头文件
	foreach(name ${paths})
		srcInclude(${name})
	endforeach()
	#库输出
	add_library(${PROJECT_NAME} SHARED ${SRC_LIST})

	#设置共用库,给不同平台的可执行文件用
	set(COMMON_LIBRARIES ${GAME_NAME} GamesEngines lua5.3 z)

	#以下为各种可执行文件的编译过程,根据情况选择编译自己需要的可执行文件即可
	add_executable(GamesGLUT ./executable/GamesGLUT.cpp)
	target_link_libraries(GamesGLUT ${COMMON_LIBRARIES} glut GL)

	add_executable(GamesGLFW ./executable/GamesGLFW.cpp)
	target_link_libraries(GamesGLFW ${COMMON_LIBRARIES} glfw GL)

	add_executable(GamesSDL ./executable/GamesSDL.cpp)
	target_link_libraries(GamesSDL ${COMMON_LIBRARIES} SDL)

	add_executable(GamesSDL2 ./executable/GamesSDL2.cpp)
	target_link_libraries(GamesSDL2 ${COMMON_LIBRARIES} SDL2)

	add_executable(GamesQt ./executable/GamesQt.cpp)
	target_link_libraries(GamesQt ${COMMON_LIBRARIES} Qt5Widgets Qt5Core)
endif()