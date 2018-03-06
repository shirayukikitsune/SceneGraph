set(HEADER_FILES
        Application.h
        Bootstrap.h
        Graphics.h
        Events/SDLEventHandler.h
        Graphics/Buffer.h
    )

add_library(scenegraph-sdl
        ${HEADER_FILES}
        Application.cpp
        Graphics.cpp
        Events/SDLEventHandler.cpp
    )

target_include_directories(scenegraph-sdl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../glad/include ${CMAKE_CURRENT_SOURCE_DIR}/../Scenegraph ${SDL2_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR})
target_link_libraries(scenegraph-sdl LINK_INTERFACE_LIBRARIES scenegraph glad ${SDL2_LIBRARY})

set_target_properties( 
    scenegraph
  PROPERTIES 
    FRAMEWORK ON
    SOVERSION 0
    VERSION 0.1
    ARCHIVE_OUTPUT_DIRECTORY "lib"
    LIBRARY_OUTPUT_DIRECTORY "lib"
    OUTPUT_NAME kitsune-scenegraph-sdl
)

install(TARGETS scenegraph-sdl
        FRAMEWORK DESTINATION lib
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib)

MACRO(INSTALL_HEADERS_WITH_DIRECTORY HEADER_LIST)

    FOREACH(HEADER ${${HEADER_LIST}})
        STRING(REGEX MATCH "(.*)[/\\]" DIR ${HEADER})
        INSTALL(FILES ${HEADER} DESTINATION include/kitsune/scenegraph/${DIR})
    ENDFOREACH(HEADER)

ENDMACRO(INSTALL_HEADERS_WITH_DIRECTORY)

INSTALL_HEADERS_WITH_DIRECTORY(HEADER_FILES)

