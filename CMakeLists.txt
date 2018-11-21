cmake_minimum_required(VERSION 2.8)
include(../CompileScripts/CMakeLists.txt)
project(GamesEngines)

#指定存放源码和头文件的所有路径
set(paths
	Cpp11 Cpp11/Containers
	Image Image/FileStructs Image/ColorSpaces
	Lua Math
	Zipper Zipper/FileStructs
)

foreach(name ${paths})
	srcInclude(${name})
endforeach()

#system include
include_directories(/usr/include/lua5.3)
include_directories(/usr/include/x86_64-linux-gnu/qt5)

#output
add_library(${PROJECT_NAME} SHARED ${SRC_LIST})

set(COMMON_LIBRARIES ${GAME_NAME} GamesEngines lua5.2 z)

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
