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
        CollisionShape->setDimentions(btVector3(0, 1, 0));
        CollisionShape->setShape(sg::CollisionShapeComponent::ShapeFormat::Plane);
        
        auto RigidBody = node->createComponent<sg::RigidBodyComponent>(0, sg::RigidBodyComponent::RigidBodyType::Static);
    }

    // Add the Plane Material
    auto Material = node->createComponent<sdlg::Material<sdlg::vertex::PositionNormalUVVertex, unsigned short>>();
    Material->beginSetup();

    // Create index buffer
    unsigned short planeIndices[] = { 0, 1, 2, 1, 3, 2 };
    typedef sdlg::ArrayBuffer<unsigned short, 1> IBType;
    auto planeIndexBuffer = std::unique_ptr<IBType>(new IBType(sdlg::BufferTarget::Index));
    planeIndexBuffer->create(planeIndices, sizeof(planeIndices) / sizeof(planeIndices[0]), sdlg::BufferFrequency::Static, sdlg::BufferAccess::Draw);

    // Create vertex buffer
    sdlg::vertex::PositionNormalUVVertex planeVertices[] = {
        sdlg::vertex::PositionNormalUVVertex(btVector3(1, -1, 0), btVector3(0, 1, 0), btVector4(0, 1, 0, 0)),
        sdlg::vertex::PositionNormalUVVertex(btVector3(1, 1, 0), btVector3(0, 1, 0), btVector4(1, 1, 0, 0)),
        sdlg::vertex::PositionNormalUVVertex(btVector3(-1, -1, 0), btVector3(0, 1, 0), btVector4(0, 0, 0, 0)),
        sdlg::vertex::PositionNormalUVVertex(btVector3(-1, 1, 0), btVector3(0, 1, 0), btVector4(1, 0, 0, 0))
    };
    typedef sdlg::ArrayBuffer<sdlg::vertex::PositionNormalUVVertex, 1> VBType;
    auto planeVertexBuffer = std::unique_ptr<VBType>(new VBType(sdlg::BufferTarget::Vertex));
    planeVertexBuffer->create(planeVertices, sizeof(planeVertices) / sizeof(planeVertices[0]), sdlg::BufferFrequency::Static, sdlg::BufferAccess::Draw);

    Material->endSetup();
    Material->setIndexBuffer(std::move(planeIndexBuffer));
    Material->setVertexBuffer(std::move(planeVertexBuffer));
}

}
}
}
}
