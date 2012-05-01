cmake_minimum_required(VERSION 2.6)
cmake_policy(SET CMP0015 NEW)

# Project name
project (glEngine)

# Find OpenGL
find_package (OpenGL)

# Find SFML
set(CMAKE_MODULE_PATH CMake/Modules ${CMAKE_MODULE_PATH})
find_package (SFML 2.0 REQUIRED window graphics system)

# Executable path
set (EXECUTABLE_OUTPUT_PATH ${CMAKE_BUILD_TYPE})

# Includes paths
include_directories (
    includes
    includes/gle
    examples/bullet
    src
)

link_directories (
    examples/bullet
    examples
)

# Sources files
file (
    GLOB_RECURSE
    SOURCES
    src/*
)

# Add build of glEngine library
add_library(glEngine SHARED ${SOURCES})


# Add examples
add_executable (
    examples/cube
    examples/cube.cpp
)

add_executable (
    examples/example00
    examples/example00.cpp
)

add_executable (
    examples/disneyHead
    examples/disneyHead.cpp
    examples/flycam.cpp
    examples/video.cpp
)

add_executable (
    examples/example02
    examples/example02.cpp
    examples/video.cpp
)

add_executable (
    examples/texture
    examples/texture.cpp
)

add_executable (
    examples/flycam_example
    examples/flycam.cpp
    examples/flycam_example.cpp
    examples/video.cpp
)

add_executable (
    examples/geometries
    examples/geometries.cpp
    examples/flycam.cpp
    examples/video.cpp
)

add_executable (
    examples/city
    examples/city.cpp
    examples/flycam.cpp
    examples/video.cpp
)

add_executable (
    examples/bullet_physics
    examples/bullet_physics.cpp
    examples/flycam.cpp
    examples/video.cpp
)

# Librairies to link
target_link_libraries (
    examples/cube
    ${SFML_LIBRARIES}
    ${OPENGL_LIBRARIES}
    glEngine
)

target_link_libraries (
    examples/example00
    ${SFML_LIBRARIES}
    ${OPENGL_LIBRARIES}
    glEngine
)

target_link_libraries (
    examples/disneyHead
    ${SFML_LIBRARIES}
    ${OPENGL_LIBRARIES}
    glEngine
)

target_link_libraries (
    examples/example02
    ${SFML_LIBRARIES}
    ${OPENGL_LIBRARIES}
    glEngine
)

target_link_libraries (
    examples/texture
    ${SFML_LIBRARIES}
    ${OPENGL_LIBRARIES}
    glEngine
)

target_link_libraries (
    examples/flycam_example
    ${SFML_LIBRARIES}
    ${OPENGL_LIBRARIES}
    glEngine
)

target_link_libraries (
    examples/geometries
    ${SFML_LIBRARIES}
    ${OPENGL_LIBRARIES}
    glEngine
)

target_link_libraries (
    examples/city
    ${SFML_LIBRARIES}
    ${OPENGL_LIBRARIES}
    glEngine
)

target_link_libraries (
    examples/bullet_physics
    ${SFML_LIBRARIES}
    ${OPENGL_LIBRARIES}
    glEngine
    BulletDynamics
    BulletCollision
    LinearMath
    BulletMultiThreaded
    pthread
)
