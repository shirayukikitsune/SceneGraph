#pragma once

#include "../Base/Component.h"

#include <BulletCollision/CollisionShapes/btCollisionShape.h>

namespace kitsune::scenegraph {

    class CollisionShapeComponent :
            public Component {
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

        btCollisionShape *getCollisionShape() { return CollisionShape.get(); }

        void setShape(ShapeFormat Shape);

        ShapeFormat getShape() const { return Shape; }

        void setDimensions(const btVector3 &Dimensions) { this->Dimensions = Dimensions; }

        const btVector3 &getDimensions() const { return Dimensions; }

        void setPlaneConstant(float PlaneConstant) { this->PlaneConstant = PlaneConstant; }

        float getPlaneConstant() const { return PlaneConstant; }

    private:
        ShapeFormat Shape;

        btVector3 Dimensions;
        float PlaneConstant;

        std::unique_ptr<btCollisionShape> CollisionShape;
    };

}

