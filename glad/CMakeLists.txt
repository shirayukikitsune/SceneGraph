add_library(glad src/glad.cpp)
target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

set_target_properties( 
    scenegraph
  PROPERTIES 
    FRAMEWORK ON
    SOVERSION 0
    VERSION 0.1
    ARCHIVE_OUTPUT_DIRECTORY "lib"
    LIBRARY_OUTPUT_DIRECTORY "lib"
    OUTPUT_NAME kitsune-scenegraph-glad
)

install(TARGETS glad
        FRAMEWORK DESTINATION lib
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib)


