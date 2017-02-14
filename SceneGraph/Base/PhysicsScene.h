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
		void setFps(float Fps);

		btSoftRigidDynamicsWorld * getWorld();

	protected:
		virtual void onPreInit();

		virtual void onUpdate(float DeltaTime);

	private:
		static void worldTickCallback(btDynamicsWorld *World, btScalar TimeStep);

		updateCallback prePhysicsUpdateEvents;

		updateCallback physicsUpdateEvents;

		std::unique_ptr<btBroadphaseInterface> Broadphase;
		std::unique_ptr<btCollisionConfiguration> CollisionConfiguration;
		std::unique_ptr<btCollisionDispatcher> CollisionDispatcher;
		std::unique_ptr<btConstraintSolver> ConstraintSolver;
		std::unique_ptr<btSoftRigidDynamicsWorld> DynamicsWorld;
		std::unique_ptr<btSoftBodySolver> SoftBodySolver;

		float FrameTime;
		btVector3 Gravity;
	};

}
}


