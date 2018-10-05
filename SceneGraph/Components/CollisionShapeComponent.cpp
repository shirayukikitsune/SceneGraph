#include <memory>

#include "CollisionShapeComponent.h"

#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include <BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h>
#include <BulletCollision/CollisionShapes/btCapsuleShape.h>
#include <BulletCollision/CollisionShapes/btConeShape.h>
#include <BulletCollision/CollisionShapes/btConvexHullShape.h>
#include <BulletCollision/CollisionShapes/btCylinderShape.h>
#include <BulletCollision/CollisionShapes/btEmptyShape.h>
#include <BulletCollision/CollisionShapes/btShapeHull.h>
#include <BulletCollision/CollisionShapes/btSphereShape.h>
#include <BulletCollision/CollisionShapes/btStaticPlaneShape.h>

using kitsune::scenegraph::CollisionShapeComponent;
namespace sg = kitsune::scenegraph;

CollisionShapeComponent::CollisionShapeComponent()
        : PlaneConstant(0.0f), Dimensions(btVector3(0, 0, 0)) {
    Shape = ShapeFormat::Null;
    CollisionShape = std::make_unique<btEmptyShape>();
}

void CollisionShapeComponent::setShape(ShapeFormat Shape) {
    Shape = Shape;

    switch (Shape) {
        case ShapeFormat::Box:
            CollisionShape = std::make_unique<btBoxShape>(getDimensions());
            break;
        case ShapeFormat::Capsule:
            CollisionShape = std::make_unique<btCapsuleShape>(getDimensions().x(), getDimensions().y());
            break;
        case ShapeFormat::Cone:
            CollisionShape = std::make_unique<btConeShape>(getDimensions().x(), getDimensions().y());
            break;
        case ShapeFormat::Cylinder:
            CollisionShape = std::make_unique<btCylinderShape>(getDimensions());
            break;
        case ShapeFormat::Plane:
            CollisionShape = std::make_unique<btStaticPlaneShape>(getDimensions(), getPlaneConstant());
            break;
        case ShapeFormat::Sphere:
            CollisionShape = std::make_unique<btSphereShape>(getDimensions().x());
            break;
        case ShapeFormat::StaticTriangleMesh:
            //CollisionShape.reset(new btBvhTriangleMeshShape(mesh, true, true));
            //break;
        case ShapeFormat::TriangleMesh:
        case ShapeFormat::OptimizedTriangleMesh:
            /*{
                btConvexShape * originalShape;
                btShapeHull * hull = new btShapeHull(originalShape);
                btScalar margin = originalShape->getMargin();
                hull->buildHull(margin);
                CollisionShape.reset(new btConvexHullShape(reinterpret_cast<const btScalar*>(hull->getVertexPointer()), hull->numVertices()));
                break;
            }*/
        case ShapeFormat::Null:
            CollisionShape = std::make_unique<btEmptyShape>();
            break;
    }
}
