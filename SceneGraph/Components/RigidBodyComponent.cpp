#include "RigidBodyComponent.h"

#include "../Base/PhysicsScene.h"

#include <BulletCollision/CollisionShapes/btEmptyShape.h>

#include <memory>

using kitsune::scenegraph::RigidBodyComponent;
namespace sg = kitsune::scenegraph;

namespace kitsune {
namespace scenegraph {
	class bulletMotionState : public btMotionState
	{
	private:
		std::weak_ptr<Node> Node;
		
	public:
		bulletMotionState()
		{

		}

		virtual ~bulletMotionState()
		{

		}

		void setNode(std::weak_ptr<kitsune::scenegraph::Node> Node)
		{
			this->Node = Node;
		}

		virtual void getWorldTransform(btTransform & WorldTransform) const
		{
			if (auto Ptr = Node.lock()) {
				WorldTransform = Ptr->getWorldTransform();
			}
			else WorldTransform = btTransform::getIdentity();
		}

		virtual void setWorldTransform(const btTransform & WorldTransform)
		{
			if (auto Ptr = Node.lock()) {
				Ptr->setWorldTransform(WorldTransform);
			}
		}
	};
}
}

RigidBodyComponent::RigidBodyComponent(float Mass, RigidBodyType Type)
	: Group(0), Mask(0xFFFF), ConstructionInfo(Mass, new bulletMotionState, new btEmptyShape), Type(Type)
{
	RigidBody.reset(new btRigidBody(ConstructionInfo));

	setRigidBodyType(Type);
}

RigidBodyComponent::~RigidBodyComponent()
{
	removeBodyFromWorld();

	delete ConstructionInfo.m_motionState;
	delete ConstructionInfo.m_collisionShape;
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

void RigidBodyComponent::setRigidBodyType(RigidBodyType Type)
{
	this->Type = Type;

	auto flags = RigidBody->getCollisionFlags();

	switch (Type) {
	case RigidBodyType::Static:
		flags = flags & ~btCollisionObject::CF_KINEMATIC_OBJECT | btCollisionObject::CF_STATIC_OBJECT;
		break;
	case RigidBodyType::Kinematic:
		flags = flags | btCollisionObject::CF_KINEMATIC_OBJECT & ~btCollisionObject::CF_STATIC_OBJECT;
		break;
	case RigidBodyType::Dynamic:
		flags = flags & ~btCollisionObject::CF_KINEMATIC_OBJECT & ~btCollisionObject::CF_STATIC_OBJECT;
		break;
	}

	RigidBody->setCollisionFlags(flags);
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

void RigidBodyComponent::updateRigidBodyPosition()
{
	auto node = getNode();

	RigidBody->setCenterOfMassTransform(node->getWorldTransform());
}

void RigidBodyComponent::updateNodePosition(float timeStep)
{
	auto node = getNode();

	node->setWorldTransform(RigidBody->getCenterOfMassTransform());
}

void RigidBodyComponent::onNodeSet()
{
	auto scene = getScene();
	if (!scene)
		return;

	auto physicsScene = std::dynamic_pointer_cast<sg::PhysicsScene>(scene);
	if (!physicsScene)
		return;

	//physicsScene->addUpdateEvent(std::bind(&RigidBodyComponent::updateNodePosition, this, std::placeholders::_1));

	auto node = getNode();

	//node->addInvalidatedByParentEvent(std::bind(&RigidBodyComponent::updateRigidBodyPosition, this));

	static_cast<bulletMotionState*>(RigidBody->getMotionState())->setNode(node);
	RigidBody->setCenterOfMassTransform(node->getWorldTransform());

	// TODO: Get a CollisionShape component and set the rigid body

	if (RigidBody->getBroadphaseHandle() == nullptr)
		physicsScene->getWorld()->addRigidBody(RigidBody.get(), Group, Mask);
}
