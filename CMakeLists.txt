cmake_minimum_required(VERSION 3.2)
project(GamesEngines)
include_directories(../../libCpp11)
include(../../CompileScripts/CMakeLists.txt)

add_library(${PROJECT_NAME} SHARED ${SRC_LIST})

if(${TARGET_SYSTEM_NAME} STREQUAL Linux)
add_executable(GamesGLUT ./executable/GamesGLUT.cpp)
add_executable(GamesGLEW ./executable/GamesGLEW.cpp)
add_executable(GamesGLFW ./executable/GamesGLFW.cpp)

target_link_libraries(GamesGLUT ${PROJECT_NAME} glut GL)
target_link_libraries(GamesGLEW ${PROJECT_NAME} GLEW GL)
target_link_libraries(GamesGLFW ${PROJECT_NAME} glfw GL)
endif()