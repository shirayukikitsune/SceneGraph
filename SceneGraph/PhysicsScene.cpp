#include "PhysicsScene.h"

#include <cassert>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <BulletSoftBody/btDefaultSoftBodySolver.h>
#include <BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h>
#include <BulletSoftBody/btSoftBodySolvers.h>

using kitsune::scenegraph::PhysicsScene;

PhysicsScene::PhysicsScene()
{
}


PhysicsScene::~PhysicsScene()
{
}

void PhysicsScene::setGravity(const btVector3 & Gravity)
{
	if (DynamicsWorld)
		DynamicsWorld->setGravity(Gravity);
}

void PhysicsScene::onInit()
{
	Broadphase.reset(new btDbvtBroadphase);
	assert(Broadphase != nullptr);

	CollisionConfiguration.reset(new btSoftBodyRigidBodyCollisionConfiguration);
	assert(CollisionConfiguration != nullptr);

	CollisionDispatcher.reset(new btCollisionDispatcher(CollisionConfiguration.get()));
	assert(CollisionDispatcher != nullptr);

	ConstraintSolver.reset(new btSequentialImpulseConstraintSolver);
	assert(ConstraintSolver != nullptr);

	SoftBodySolver.reset(new btDefaultSoftBodySolver);
	assert(SoftBodySolver != nullptr);

	DynamicsWorld.reset(new btSoftRigidDynamicsWorld(CollisionDispatcher.get(), Broadphase.get(), ConstraintSolver.get(), CollisionConfiguration.get(), SoftBodySolver.get()));
	assert(DynamicsWorld != nullptr);

	DynamicsWorld->setGravity(btVector3(0, -9.8f, 0));
}

void PhysicsScene::onUpdate(float DeltaTime)
{
	prePhysicsUpdateEvents(DeltaTime);

	DynamicsWorld->stepSimulation(DeltaTime);

	physicsUpdateEvents(DeltaTime);
}
