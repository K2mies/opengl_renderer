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
    src/MathUtils.cpp
    src/typedefs/Vec4.cpp
    src/typedefs/Vec3.cpp
    src/typedefs/Mat4.cpp
    src/typedefs/Mat3.cpp
    third_party/glad/src/glad.c
)

target_include_directories(OpenGLRenderer PRIVATE
    third_party/glad/include
    third_party/stb/
    third_party
    include
)

target_link_libraries(OpenGLRenderer
    glfw
    OpenGL::GL
)

