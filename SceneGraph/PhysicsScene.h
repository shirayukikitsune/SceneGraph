#pragma once

#include "Scene.h"

#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>

namespace kitsune {
namespace scenegraph {

	class PhysicsScene
		: public Scene
	{
	public:
		PhysicsScene();
		~PhysicsScene();

		void setGravity(const btVector3 & Gravity);

		btSoftRigidDynamicsWorld * getWorld();

	protected:
		virtual void onInit();

		virtual void onUpdate(float DeltaTime);

	private:
		updateCallback prePhysicsUpdateEvents;

		updateCallback physicsUpdateEvents;

		std::unique_ptr<btBroadphaseInterface> Broadphase;
		std::unique_ptr<btCollisionConfiguration> CollisionConfiguration;
		std::unique_ptr<btCollisionDispatcher> CollisionDispatcher;
		std::unique_ptr<btConstraintSolver> ConstraintSolver;
		std::unique_ptr<btSoftRigidDynamicsWorld> DynamicsWorld;
		std::unique_ptr<btSoftBodySolver> SoftBodySolver;
	};

}
}


