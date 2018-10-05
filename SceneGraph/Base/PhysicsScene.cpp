#include "PhysicsScene.h"

#include <cassert>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <BulletSoftBody/btDefaultSoftBodySolver.h>
#include <BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h>
#include <BulletSoftBody/btSoftBodySolvers.h>

using kitsune::scenegraph::PhysicsScene;

PhysicsScene::PhysicsScene() :
        Gravity(0, -9.8f, 0) {
    this->FrameTime = 1 / 60.0f;
}

PhysicsScene::~PhysicsScene() {
}

void PhysicsScene::setGravity(const btVector3 &Gravity) {
    this->Gravity = Gravity;

    if (DynamicsWorld)
        DynamicsWorld->setGravity(Gravity);
}

void PhysicsScene::setFps(float Fps) {
    this->FrameTime = 1 / Fps;
}

btSoftRigidDynamicsWorld *PhysicsScene::getWorld() {
    return DynamicsWorld.get();
}

void PhysicsScene::onPreInit() {
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

    DynamicsWorld.reset(
            new btSoftRigidDynamicsWorld(CollisionDispatcher.get(), Broadphase.get(), ConstraintSolver.get(),
                                         CollisionConfiguration.get(), SoftBodySolver.get()));
    assert(DynamicsWorld != nullptr);

    DynamicsWorld->setGravity(this->Gravity);
    DynamicsWorld->setInternalTickCallback(PhysicsScene::worldTickCallback, this);
}

void PhysicsScene::onUpdate(float DeltaTime) {
    prePhysicsUpdateEvents(DeltaTime);

    DynamicsWorld->stepSimulation(DeltaTime, std::max(1, (int) (DeltaTime / this->FrameTime)) * 10, this->FrameTime);
}

void PhysicsScene::worldTickCallback(btDynamicsWorld *World, btScalar TimeStep) {
    PhysicsScene *scene = static_cast<PhysicsScene *>(World->getWorldUserInfo());
    scene->physicsUpdateEvents(TimeStep);
}
