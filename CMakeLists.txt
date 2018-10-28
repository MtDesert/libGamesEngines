cmake_minimum_required(VERSION 2.8)
project(GamesEngines)
include_directories(../libCpp11 ../libCpp11/Containers)
include_directories(/usr/include/lua5.3)
include(../CompileScripts/CMakeLists.txt)
aux_source_directory(Lua SRC_LIST)
add_library(${PROJECT_NAME} SHARED ${SRC_LIST})

set(COMMON_LIBRARIES GamesEngines Cpp11 lua5.2)

add_executable(GamesGLUT ./executable/GamesGLUT.cpp)
target_link_libraries(GamesGLUT ${COMMON_LIBRARIES} glut GL)

add_executable(GamesGLFW ./executable/GamesGLFW.cpp)
target_link_libraries(GamesGLFW ${COMMON_LIBRARIES} glfw GL)

add_executable(GamesSDL ./executable/GamesSDL.cpp)
target_link_libraries(GamesSDL ${COMMON_LIBRARIES} SDL)

add_executable(GamesSDL2 ./executable/GamesSDL2.cpp)
target_link_libraries(GamesSDL2 ${COMMON_LIBRARIES} SDL2)