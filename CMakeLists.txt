cmake_minimum_required(VERSION 3.16)

project(OpenGLRenderer)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(OpenGLRenderer
    src/main.cpp
    src/Objects/Shader.cpp
    src/Objects/Texture.cpp
    src/Scene/Camera.cpp
    src/stb_image.cpp
    src/Utils/MathUtils.cpp
    src/Typedefs/Vec4.cpp
    src/Typedefs/Vec3.cpp
    src/Typedefs/Mat4.cpp
    src/Typedefs/Mat3.cpp
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

