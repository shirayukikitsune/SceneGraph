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
	: PlaneConstant(0.0f)
{
	CollisionShape.reset(new btEmptyShape);
}

CollisionShapeComponent::~CollisionShapeComponent()
{
}

void CollisionShapeComponent::setShape(ShapeFormat Shape)
{
	this->Shape = Shape;

	switch (Shape) {
	case ShapeFormat::Box:
		CollisionShape.reset(new btBoxShape(getDimentions()));
		break;
	case ShapeFormat::Capsule:
		CollisionShape.reset(new btCapsuleShape(getDimentions().x(), getDimentions().y()));
		break;
	case ShapeFormat::Cone:
		CollisionShape.reset(new btConeShape(getDimentions().x(), getDimentions().y()));
		break;
	case ShapeFormat::Cylinder:
		CollisionShape.reset(new btCylinderShape(getDimentions()));
		break;
	case ShapeFormat::Plane:
		CollisionShape.reset(new btStaticPlaneShape(getDimentions(), getPlaneConstant()));
		break;
	case ShapeFormat::Sphere:
		CollisionShape.reset(new btSphereShape(getDimentions().x()));
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
		CollisionShape.reset(new btEmptyShape);
		break;
	}
}
