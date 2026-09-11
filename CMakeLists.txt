cmake_minimum_required(VERSION 3.16)

project(OpenGLRenderer)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(glfw3  REQUIRED)

# --------------------------------------------------------- Assimp

# First try to use an Assimp installation already on the system.
find_package(assimp QUIET)

# If Assimp isn't installed, download and build it locally.
if(NOT assimp_FOUND)

    message(STATUS "Assimp not found locally - fetching Assimp")

    include(FetchContent)

    FetchContent_Declare(
        assimp
        GIT_REPOSITORY https://github.com/assimp/assimp.git
        GIT_TAG v6.0.2
    )

    set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
    set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(assimp)

else()

    message(STATUS "Using installed Assimp")

endif()

add_executable(OpenGLRenderer
    src/main.cpp

    src/Objects/Shader.cpp
    src/Objects/Texture.cpp

    src/Scene/Camera.cpp

    src/stb_image.cpp

    src/Utils/MathUtils.cpp

    src/Typedefs/Vec4.cpp
    src/Typedefs/Vec3.cpp
    src/Typedefs/Vec2.cpp
    src/Typedefs/Mat4.cpp
    src/Typedefs/Mat3.cpp
    src/Typedefs/Mat2.cpp

    third_party/glad/src/glad.c
)

target_include_directories(OpenGLRenderer PRIVATE
    third_party/glad/include
    third_party/stb/
    third_party
    include
)

target_link_libraries(OpenGLRenderer
    PRIVATE
    glfw
    OpenGL::GL
    assimp::assimp
)

