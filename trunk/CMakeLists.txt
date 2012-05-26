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

# Add build of examples library
add_library(Examples STATIC
		     examples/Example.cpp
		     examples/flycam.cpp
		     examples/trackball.cpp
		     examples/video.cpp
		     examples/fps.cpp)

add_executable (
    examples/objloader
    examples/objloader.cpp
)

add_executable (
    examples/pointLights
    examples/pointLights.cpp
)

add_executable (
    examples/solarSystem
    examples/solarSystem.cpp
)

add_executable (
    examples/geometries
    examples/geometries.cpp
)

add_executable (
    examples/city
    examples/city.cpp
)

add_executable (
    examples/bullet_physics
    examples/bullet_physics.cpp
)

add_executable (
    examples/boundingvolume
    examples/boundingvolume.cpp
)

add_executable (
    examples/lookat
    examples/lookat.cpp
)

target_link_libraries (
    examples/boundingvolume
    ${SFML_LIBRARIES}
    ${OPENGL_LIBRARIES}
    glEngine
    Examples
)

target_link_libraries (
    examples/objloader
    ${SFML_LIBRARIES}
    ${OPENGL_LIBRARIES}
    glEngine
    Examples
)

target_link_libraries (
    examples/pointLights
    ${SFML_LIBRARIES}
    ${OPENGL_LIBRARIES}
    glEngine
    Examples
)

target_link_libraries (
    examples/solarSystem
    ${SFML_LIBRARIES}
    ${OPENGL_LIBRARIES}
    glEngine
    Examples
)

target_link_libraries (
    examples/geometries
    ${SFML_LIBRARIES}
    ${OPENGL_LIBRARIES}
    glEngine
    Examples
)

target_link_libraries (
    examples/city
    ${SFML_LIBRARIES}
    ${OPENGL_LIBRARIES}
    glEngine
    Examples
)

target_link_libraries (
    examples/bullet_physics
    ${SFML_LIBRARIES}
    ${OPENGL_LIBRARIES}
    glEngine
    Examples
    BulletDynamics
    BulletCollision
    LinearMath
    BulletMultiThreaded
    pthread
)

target_link_libraries (
    examples/lookat
    ${SFML_LIBRARIES}
    ${OPENGL_LIBRARIES}
    glEngine
    Examples
)
