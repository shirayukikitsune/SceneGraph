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
