cmake_minimum_required(VERSION 3.7)
project(gl_start)

set(CMAKE_CXX_STANDARD 11)

set(path "C:/Program Files (x86)/Microsoft Visual Studio 14.0/VC/lib")

set(SOURCE_FILES
        main.cpp
        glad.c texture.cpp util.cc shader.cpp)
add_executable(gl_start ${SOURCE_FILES})

find_package(OpenGL REQUIRED)
find_package(glfw REQUIRED)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${OPENGL_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS} "C:/Program Files (x86)/Microsoft Visual Studio 14.0/VC/include")

target_link_libraries(gl_start ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES})

add_custom_command(TARGET gl_start PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/res
        $<TARGET_FILE_DIR:gl_start>/res
        )