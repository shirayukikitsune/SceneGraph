add_executable(scenegraph-sdl-example02 Example.cpp Example.h)
# target_sources(scenegraph-sdl-example02 PRIVATE example01.frag example01.vert)

target_link_libraries(scenegraph-sdl-example02 LINK_PUBLIC scenegraph scenegraph-sdl)
target_include_directories(scenegraph-sdl-example02 PUBLIC ${CMAKE_SOURCE_DIR}/Scenegraph ${CMAKE_SOURCE_DIR}/Scenegraph.SDL ${BULLET_INCLUDE_DIR})
