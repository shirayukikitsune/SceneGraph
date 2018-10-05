#include <memory>

#include <memory>

#include "Prefabs.h"

#include "Graphics/Material.h"

#include <Base/Node.h>
#include <Base/Scene.h>
#include <Components/CollisionShapeComponent.h>
#include <Components/RigidBodyComponent.h>

namespace sg = kitsune::scenegraph;
namespace sdlg = kitsune::scenegraph::sdl::graphics;

namespace kitsune {
namespace scenegraph {
namespace sdl {
namespace prefabs {

void CreatePlane(std::shared_ptr<Node> node, bool addPhysics) {
    if (addPhysics) {
        // Enable physics for this plane (need to add a CollisionShapeComponent and a RigidBodyComponent)
        auto CollisionShape = node->createComponent<sg::CollisionShapeComponent>();
        CollisionShape->setDimensions(btVector3(0, 1, 0));
        CollisionShape->setShape(sg::CollisionShapeComponent::ShapeFormat::Plane);
        
        node->createComponent<sg::RigidBodyComponent>(0.0f, sg::RigidBodyComponent::RigidBodyType::Kinematic);
    }

    // Add the Plane Material
    auto Material = node->createComponent<sdlg::Material<sdlg::vertex::PositionNormalUVVertex, unsigned short>>();
    Material->beginSetup();

    // Create index buffer
    unsigned short planeIndices[] = { 0, 1, 2, 1, 3, 2 };
    typedef sdlg::ArrayBuffer<unsigned short, 1> IBType;
    auto planeIndexBuffer = std::make_unique<IBType>(sdlg::BufferTarget::Index);
    planeIndexBuffer->create(planeIndices, sizeof(planeIndices) / sizeof(planeIndices[0]), sdlg::BufferFrequency::Static, sdlg::BufferAccess::Draw);

    // Create vertex buffer
    sdlg::vertex::PositionNormalUVVertex planeVertices[] = {
        sdlg::vertex::PositionNormalUVVertex(btVector3(1, -1, 0), btVector3(0, 0, 1), btVector4(0, 1, 0, 0)),
        sdlg::vertex::PositionNormalUVVertex(btVector3(1, 1, 0), btVector3(0, 0, 1), btVector4(1, 1, 0, 0)),
        sdlg::vertex::PositionNormalUVVertex(btVector3(-1, -1, 0), btVector3(0, 0, 1), btVector4(0, 0, 0, 0)),
        sdlg::vertex::PositionNormalUVVertex(btVector3(-1, 1, 0), btVector3(0, 0, 1), btVector4(1, 0, 0, 0))
    };
    typedef sdlg::ArrayBuffer<sdlg::vertex::PositionNormalUVVertex, 1> VBType;
    auto planeVertexBuffer = std::make_unique<VBType>(sdlg::BufferTarget::Vertex);
    planeVertexBuffer->create(planeVertices, sizeof(planeVertices) / sizeof(planeVertices[0]), sdlg::BufferFrequency::Static, sdlg::BufferAccess::Draw);

    Material->endSetup();
    Material->setIndexBuffer(std::move(planeIndexBuffer));
    Material->setVertexBuffer(std::move(planeVertexBuffer));
}

void CreateCube(std::shared_ptr<Node> node, bool addPhysics) {
    if (addPhysics) {
        // Enable physics for this cube (need to add a CollisionShapeComponent and a RigidBodyComponent)
        auto CollisionShape = node->createComponent<sg::CollisionShapeComponent>();
        CollisionShape->setDimensions(btVector3(node->getLocalScale().x, node->getLocalScale().y, node->getLocalScale().z));
        CollisionShape->setShape(sg::CollisionShapeComponent::ShapeFormat::Box);

        node->createComponent<sg::RigidBodyComponent>(0.0f, sg::RigidBodyComponent::RigidBodyType::Kinematic);
    }

    // Add the Plane Material
    auto Material = node->createComponent<sdlg::Material<sdlg::vertex::PositionNormalUVVertex, unsigned short>>();
    Material->beginSetup();

    // Create index buffer
    unsigned short cubeIndices[] = {
        2, 1, 0, 1, 2, 3,
        5, 6, 4, 5, 7, 6,
        10, 9, 8, 9, 10, 11,
        14, 12, 13, 13, 15, 14,
        18, 16, 17, 17, 19, 18,
        22, 21, 20, 21, 22, 23,
    };
    typedef sdlg::ArrayBuffer<unsigned short, 1> IBType;
    auto cubeIndexBuffer = std::make_unique<IBType>(sdlg::BufferTarget::Index);
    cubeIndexBuffer->create(cubeIndices, sizeof(cubeIndices) / sizeof(cubeIndices[0]), sdlg::BufferFrequency::Static, sdlg::BufferAccess::Draw);

    // Create vertex buffer
    sdlg::vertex::PositionNormalUVVertex cubeVertices[] = {
        sdlg::vertex::PositionNormalUVVertex(btVector3(0.5f, -0.5f, -0.5f), btVector3(0, 0, -1), btVector4(1.0f / 3.0f, 0, 0, 0)),
        sdlg::vertex::PositionNormalUVVertex(btVector3(0.5f, 0.5f, -0.5f), btVector3(0, 0, -1), btVector4(1.0f / 3.0f, 0.5f, 0, 0)),
        sdlg::vertex::PositionNormalUVVertex(btVector3(-0.5f, -0.5f, -0.5f), btVector3(0, 0, -1), btVector4(0, 0, 0, 0)),
        sdlg::vertex::PositionNormalUVVertex(btVector3(-0.5f, 0.5f, -0.5f), btVector3(0, 0, -1), btVector4(0, 0.5f, 0, 0)),

        sdlg::vertex::PositionNormalUVVertex(btVector3(0.5f, -0.5f, 0.5f), btVector3(0, 0, 1), btVector4(1.0f / 3.0f, 0.5f, 0, 0)),
        sdlg::vertex::PositionNormalUVVertex(btVector3(0.5f, 0.5f, 0.5f), btVector3(0, 0, 1), btVector4(1.0f / 3.0f, 1.0f, 0, 0)),
        sdlg::vertex::PositionNormalUVVertex(btVector3(-0.5f, -0.5f, 0.5f), btVector3(0, 0, 1), btVector4(0, 0.5f, 0, 0)),
        sdlg::vertex::PositionNormalUVVertex(btVector3(-0.5f, 0.5f, 0.5f), btVector3(0, 0, 1), btVector4(0, 1.0f, 0, 0)),

        sdlg::vertex::PositionNormalUVVertex(btVector3(-0.5f, -0.5f, -0.5f), btVector3(-1, 0, 0), btVector4(2.0f / 3.0f, 0, 0, 0)),
        sdlg::vertex::PositionNormalUVVertex(btVector3(-0.5f, 0.5f, -0.5f), btVector3(-1, 0, 0), btVector4(2.0f / 3.0f, 0.5f, 0, 0)),
        sdlg::vertex::PositionNormalUVVertex(btVector3(-0.5f, -0.5f, 0.5f), btVector3(-1, 0, 0), btVector4(1.0f / 3.0f, 0, 0, 0)),
        sdlg::vertex::PositionNormalUVVertex(btVector3(-0.5f, 0.5f, 0.5f), btVector3(-1, 0, 0), btVector4(1.0f / 3.0f, 0.5f, 0, 0)),

        sdlg::vertex::PositionNormalUVVertex(btVector3(0.5f, -0.5f, -0.5f), btVector3(1, 0, 0), btVector4(2.0f / 3.0f, 0.5f, 0, 0)),
        sdlg::vertex::PositionNormalUVVertex(btVector3(0.5f, 0.5f, -0.5f), btVector3(1, 0, 0), btVector4(2.0f / 3.0f, 1.0f, 0, 0)),
        sdlg::vertex::PositionNormalUVVertex(btVector3(0.5f, -0.5f, 0.5f), btVector3(1, 0, 0), btVector4(1.0f / 3.0f, 0.5f, 0, 0)),
        sdlg::vertex::PositionNormalUVVertex(btVector3(0.5f, 0.5f, 0.5f), btVector3(1, 0, 0), btVector4(1.0f / 3.0f, 1.0f, 0, 0)),

        sdlg::vertex::PositionNormalUVVertex(btVector3(0.5f, -0.5f, -0.5f), btVector3(0, -1, 0), btVector4(1.0f, 0, 0, 0)),
        sdlg::vertex::PositionNormalUVVertex(btVector3(0.5f, -0.5f, 0.5f), btVector3(0, -1, 0), btVector4(1.0f, 0.5f, 0, 0)),
        sdlg::vertex::PositionNormalUVVertex(btVector3(-0.5f, -0.5f, -0.5f), btVector3(0, -1, 0), btVector4(2.0f / 3.0f, 0, 0, 0)),
        sdlg::vertex::PositionNormalUVVertex(btVector3(-0.5f, -0.5f, 0.5f), btVector3(0, -1, 0), btVector4(2.0f / 3.0f, 0.5f, 0, 0)),

        sdlg::vertex::PositionNormalUVVertex(btVector3(0.5f, 0.5f, -0.5f), btVector3(0, 1, 0), btVector4(1.0f, 0.5f, 0, 0)),
        sdlg::vertex::PositionNormalUVVertex(btVector3(0.5f, 0.5f, 0.5f), btVector3(0, 1, 0), btVector4(1.0f, 1.0f, 0, 0)),
        sdlg::vertex::PositionNormalUVVertex(btVector3(-0.5f, 0.5f, -0.5f), btVector3(0, 1, 0), btVector4(2.0f / 3.0f, 0.5f, 0, 0)),
        sdlg::vertex::PositionNormalUVVertex(btVector3(-0.5f, 0.5f, 0.5f), btVector3(0, 1, 0), btVector4(2.0f / 3.0f, 1.0f, 0, 0))
    };
    typedef sdlg::ArrayBuffer<sdlg::vertex::PositionNormalUVVertex, 1> VBType;
    auto cubeVertexBuffer = std::make_unique<VBType>(sdlg::BufferTarget::Vertex);
    cubeVertexBuffer->create(cubeVertices, sizeof(cubeVertices) / sizeof(cubeVertices[0]), sdlg::BufferFrequency::Static, sdlg::BufferAccess::Draw);

    Material->endSetup();
    Material->setIndexBuffer(std::move(cubeIndexBuffer));
    Material->setVertexBuffer(std::move(cubeVertexBuffer));
}

}
}
}
}
