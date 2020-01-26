add_compile_definitions(GLM_ENABLE_EXPERIMENTAL)

add_executable(scenegraph-test-multiptr multiptrtest.cpp)
target_include_directories(scenegraph-test-multiptr PUBLIC ../SceneGraph ${Boost_INCLUDE_DIR})
target_link_libraries(scenegraph-test-multiptr LINK_PUBLIC ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} scenegraph)
add_test(NAME scenegraph-test-multiptr COMMAND scenegraph-test-multiptr -l all)

add_executable(scenegraph-test-component componenttest.cpp)
target_include_directories(scenegraph-test-component PUBLIC ../SceneGraph ${Boost_INCLUDE_DIR})
target_link_libraries(scenegraph-test-component LINK_PUBLIC ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} scenegraph)
add_test(NAME scenegraph-test-component COMMAND scenegraph-test-component -l all)

add_executable(scenegraph-test-node nodetest.cpp)
target_include_directories(scenegraph-test-node PUBLIC ../SceneGraph ${Boost_INCLUDE_DIR})
target_link_libraries(scenegraph-test-node LINK_PUBLIC ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} scenegraph)
add_test(NAME scenegraph-test-node COMMAND scenegraph-test-node -l all)

add_executable(scenegraph-test-physicsscene physicstest.cpp)
target_include_directories(scenegraph-test-physicsscene PUBLIC ../SceneGraph ${Boost_INCLUDE_DIR})
target_link_libraries(scenegraph-test-physicsscene LINK_PUBLIC ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} scenegraph)
add_test(NAME scenegraph-test-physicsscene COMMAND scenegraph-test-physicsscene -l all)

add_executable(scenegraph-test-scene scenetest.cpp)
target_include_directories(scenegraph-test-scene PUBLIC ../SceneGraph ${Boost_INCLUDE_DIR})
target_link_libraries(scenegraph-test-scene LINK_PUBLIC ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} scenegraph)
add_test(NAME scenegraph-test-scene COMMAND scenegraph-test-scene -l all)
