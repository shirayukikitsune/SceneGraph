add_executable(scenegraph-sdl-example01 Example.cpp Example.h)

target_link_libraries(scenegraph-sdl-example01 LINK_PUBLIC scenegraph scenegraph-sdl)
target_include_directories(scenegraph-sdl-example01 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../Scenegraph ${CMAKE_CURRENT_SOURCE_DIR}/../../Scenegraph.SDL)
