add_library(soil image_DXT.c image_helper.c SOIL.c stb_image_aug.c)

target_include_directories(soil PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/glad/include)
target_link_libraries(soil LINK_INTERFACE_LIBRARIES glad ${CMAKE_DL_LIBS})

set_target_properties( 
  soil
PROPERTIES 
  FRAMEWORK ON
  SOVERSION 0
  VERSION 0.1
  ARCHIVE_OUTPUT_DIRECTORY "lib"
  LIBRARY_OUTPUT_DIRECTORY "lib"
  OUTPUT_NAME soil
)   

install(TARGETS soil
        FRAMEWORK DESTINATION lib
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib)

install(DIRECTORY . DESTINATION include/kitsune/scenegraph
        FILES_MATCHING PATTERN "*.h")
