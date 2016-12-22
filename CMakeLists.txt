cmake_minimum_required(VERSION 3.1)
project(KITSUNE_SCENEGRAPH)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED on)

find_package(Bullet REQUIRED)

include_directories(${BULLET_INCLUDE_DIR})
set(LIBS ${LIBS} ${BULLET_LIBRARIES})

add_subdirectory(SceneGraph)
