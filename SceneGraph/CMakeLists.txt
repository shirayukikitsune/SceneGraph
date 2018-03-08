set(HEADER_FILES 
        Base/Callback.h
        Base/Component.h
        Base/Hash.h
        Base/MultiPtr.h
        Base/Node.h
        Base/PhysicsScene.h
        Base/Scene.h
        Components/CollisionShapeComponent.h
        Components/ConstraintComponent.h
        Components/RigidBodyComponent.h
        Components/SceneEventComponent.h
    )

add_library(scenegraph
        Base/Component.cpp
        Base/Node.cpp
        Base/PhysicsScene.cpp
        Base/Scene.cpp
        Components/CollisionShapeComponent.cpp
        Components/ConstraintComponent.cpp
        Components/RigidBodyComponent.cpp
        Components/SceneEventComponent.cpp
    )

target_include_directories(scenegraph PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(scenegraph
        INTERFACE ${BULLET_COLLISION_LIBRARY}
        INTERFACE ${BULLET_DYNAMICS_LIBRARY}
        INTERFACE ${BULLET_MATH_LIBRARY}
        INTERFACE ${BULLET_SOFTBODY_LIBRARY}
    )

set_target_properties( 
  scenegraph
PROPERTIES 
  FRAMEWORK ON
  SOVERSION 0
  VERSION 0.1
  ARCHIVE_OUTPUT_DIRECTORY "lib"
  LIBRARY_OUTPUT_DIRECTORY "lib"
  OUTPUT_NAME kitsune-scenegraph
)   

install(TARGETS scenegraph
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib)

install(DIRECTORY . DESTINATION include/kitsune/scenegraph
        FILES_MATCHING PATTERN "*.h")
