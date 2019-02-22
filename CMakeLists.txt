cmake_minimum_required(VERSION 3.13)
project(Graphics)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)


add_executable(Graphics Main.cpp model.cpp camera.cpp headers/stb_image.h)

find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(glfw3 3.2 REQUIRED)

target_include_directories(Graphics PUBLIC ${GLM_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS} PRIVATE headers)

target_link_libraries(Graphics glfw ${OPENGL_LIBRARY} ${GLM_LIBRARY} ${GLEW_LIBRARY} ${GLFW_LIBRARIES})
