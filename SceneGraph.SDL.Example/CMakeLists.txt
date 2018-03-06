add_executable(scenegraph-sdl-example Example.cpp Example.h)

target_link_libraries(scenegraph-sdl-example LINK_PUBLIC scenegraph scenegraph-sdl)
target_include_directories(scenegraph-sdl-example PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../Scenegraph ${CMAKE_CURRENT_SOURCE_DIR}/../Scenegraph.SDL)
