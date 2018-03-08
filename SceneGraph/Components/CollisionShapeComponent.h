#pragma once

#include "../Base/Component.h"

#include <BulletCollision/CollisionShapes/btCollisionShape.h>

namespace kitsune {
namespace scenegraph {

	class CollisionShapeComponent :
		public Component
	{
        KIT_SG_COMPONENT(kitsune::scenegraph::CollisionShapeComponent)

	public:
		enum class ShapeFormat {
			Box,
			Capsule,
			Cone,
			Cylinder,
			Plane,
			Sphere,
			StaticTriangleMesh, // TODO: implement this
			OptimizedTriangleMesh, // TODO: implement this
			TriangleMesh, // TODO: implement this
			Null,
		};

		CollisionShapeComponent();
		virtual ~CollisionShapeComponent();

		btCollisionShape* getCollisionShape() { return CollisionShape.get(); }

		void setShape(ShapeFormat Shape);
		ShapeFormat getShape() const { return Shape; }

		void setDimentions(const btVector3 & Dimentions) { this->Dimentions = Dimentions; }
		const btVector3& getDimentions() const { return Dimentions; }

		void setPlaneConstant(float PlaneConstant) { this->PlaneConstant = PlaneConstant; }
		float getPlaneConstant() const { return PlaneConstant; }

	private:
		ShapeFormat Shape;

		btVector3 Dimentions;
		float PlaneConstant;

		std::unique_ptr<btCollisionShape> CollisionShape;
	};

}
}

