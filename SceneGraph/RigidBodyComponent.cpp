#include "RigidBodyComponent.h"

#include "PhysicsScene.h"

#include <BulletDynamics/Dynamics/btRigidBody.h>

#include <memory>

using kitsune::scenegraph::RigidBodyComponent;
namespace sg = kitsune::scenegraph;

RigidBodyComponent::RigidBodyComponent(std::shared_ptr<sg::Node> Node)
	: Component(Node), Group(0), Mask(0xFFFF), ConstructionInfo(0.0f, nullptr, nullptr)
{
	if (Node)
		ConstructionInfo.m_startWorldTransform = Node->getWorldTransform();
		
	RigidBody.reset(new btRigidBody(ConstructionInfo));
}


RigidBodyComponent::~RigidBodyComponent()
{
	removeBodyFromWorld();
}

void RigidBodyComponent::setGroup(short Group)
{
	this->Group = Group;

	readdBodyToWorld();
}

void RigidBodyComponent::setMask(short Mask)
{
	this->Mask = Mask;

	readdBodyToWorld();
}

void RigidBodyComponent::setMass(float Mass)
{
	ConstructionInfo.m_mass = Mass;

	RigidBody->setMassProps(ConstructionInfo.m_mass, ConstructionInfo.m_localInertia);
}

void RigidBodyComponent::setDamping(float LinearDamping, float AngularDamping)
{
	ConstructionInfo.m_linearDamping = LinearDamping;
	ConstructionInfo.m_angularDamping = AngularDamping;

	RigidBody->setDamping(ConstructionInfo.m_linearDamping, ConstructionInfo.m_angularDamping);
}

void RigidBodyComponent::setLinearDamping(float Damping)
{
	setDamping(Damping, getAngularDamping());
}

void RigidBodyComponent::setAngularDamping(float Damping)
{
	setDamping(getLinearDamping(), Damping);
}

void RigidBodyComponent::setFriction(float Friction)
{
	ConstructionInfo.m_friction = Friction;

	RigidBody->setFriction(Friction);
}

void RigidBodyComponent::setRestitution(float Restitution)
{
	ConstructionInfo.m_restitution = Restitution;

	RigidBody->setRestitution(Restitution);
}

void RigidBodyComponent::readdBodyToWorld()
{
	auto scene = getScene();
	if (!scene)
		return;

	auto physicsScene = std::dynamic_pointer_cast<sg::PhysicsScene>(scene);
	if (!physicsScene)
		return;

	physicsScene->getWorld()->removeRigidBody(RigidBody.get());
	physicsScene->getWorld()->addRigidBody(RigidBody.get(), getGroup(), getMask());
}

void RigidBodyComponent::removeBodyFromWorld()
{
	auto scene = getScene();
	if (!scene)
		return;

	auto physicsScene = std::dynamic_pointer_cast<sg::PhysicsScene>(scene);
	if (!physicsScene)
		return;;

	physicsScene->getWorld()->removeRigidBody(RigidBody.get());
}

void RigidBodyComponent::onNodeSet()
{
	auto scene = getScene();
	if (!scene)
		return;

	auto physicsScene = std::dynamic_pointer_cast<sg::PhysicsScene>(scene);
	if (!physicsScene)
		return;

	auto node = getNode();

	RigidBody->setCenterOfMassTransform(node->getWorldTransform());

	physicsScene->getWorld()->addRigidBody(RigidBody.get(), Group, Mask);
}
