add_executable(scenegraph-sdl-example03 Example.cpp Example.h)
target_sources(scenegraph-sdl-example03 PRIVATE example03.frag example03.vert)

target_link_libraries(scenegraph-sdl-example03 LINK_PUBLIC scenegraph scenegraph-sdl)
target_include_directories(scenegraph-sdl-example03 PUBLIC ${CMAKE_SOURCE_DIR}/Scenegraph ${CMAKE_SOURCE_DIR}/Scenegraph.SDL ${BULLET_INCLUDE_DIR})
