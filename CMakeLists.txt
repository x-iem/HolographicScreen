cmake_minimum_required(VERSION 3.16)
project(Engine)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenCV REQUIRED)

add_executable(engine src/main.cpp src/glad.c src/Shaders.cpp src/stb_image.cpp src/tracker.cpp src/utils.cpp)
include_directories(include /usr/include/opencv4)

target_link_libraries(engine PRIVATE glfw OpenGL::GL ${OpenCV_LIBS})
