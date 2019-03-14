cmake_minimum_required(VERSION 3.7)
project(gl_start)

set(CMAKE_CXX_STANDARD 11)

set(path "C:/Program Files (x86)/Microsoft Visual Studio 14.0/VC/lib")

set(SOURCE_FILES
        main.cpp
        glad.c texture2.cpp util.cc shader.cpp screenshot.c)
add_executable(gl_start ${SOURCE_FILES})

set(GLFW_DIR "${CMAKE_CURRENT_SOURCE_DIR}/glfw-3.2.1")
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
set(GLFW_INSTALL ON CACHE INTERNAL "Generate installation target")
add_subdirectory("${GLFW_DIR}")
target_link_libraries(${PROJECT_NAME} "glfw")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLFW_DIR}/include")
target_compile_definitions(${PROJECT_NAME} PRIVATE "GLFW_INCLUDE_NONE")

find_package(OpenGL REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${OPENGL_INCLUDE_DIRS})

target_link_libraries(gl_start ${OPENGL_LIBRARIES})

add_custom_command(TARGET gl_start PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/res
        $<TARGET_FILE_DIR:gl_start>/res
        )