cmake_minimum_required(VERSION 3.16)

project(OpenGLRenderer)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(OpenGLRenderer
    src/main.cpp
    src/Shader.cpp
    src/Texture.cpp
    src/stb_image.cpp
    third_party/glad/src/glad.c
)

target_include_directories(OpenGLRenderer PRIVATE
    third_party/glad/include
    third_party/stb/
    include
)

target_link_libraries(OpenGLRenderer
    glfw
    OpenGL::GL
)

