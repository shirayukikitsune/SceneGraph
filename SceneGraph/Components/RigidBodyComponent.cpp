#include "RigidBodyComponent.h"

#include "CollisionShapeComponent.h"
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
	: Group(0), Mask(0xFFFF), ConstructionInfo(Mass, new bulletMotionState, nullptr), Type(Type)
{
	DefaultShape.reset(new btEmptyShape);
	RigidBody.reset(new btRigidBody(ConstructionInfo));

	setRigidBodyType(Type);
}

RigidBodyComponent::~RigidBodyComponent()
{
	removeBodyFromWorld();

	delete ConstructionInfo.m_motionState;
}

void RigidBodyComponent::setActive(bool State)
{
	Component::setActive(State);

	RigidBody->setActivationState(State == true ? ACTIVE_TAG : DISABLE_SIMULATION);
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

void RigidBodyComponent::onNodeComponentAdded(Component * Component)
{
	if (Component->getComponentNameHash() != CollisionShapeComponent::componentNameHash)
		return;

	auto CollisionShape = static_cast<CollisionShapeComponent*>(Component);
	RigidBody->setCollisionShape(CollisionShape->getCollisionShape());

	DefaultShape.reset();

	if (CollisionShape->getShape() != CollisionShapeComponent::ShapeFormat::Null)
		CollisionShape->getCollisionShape()->calculateLocalInertia(ConstructionInfo.m_mass, ConstructionInfo.m_localInertia);

	RigidBody->setMassProps(ConstructionInfo.m_mass, ConstructionInfo.m_localInertia);
}

void RigidBodyComponent::onNodeComponentRemoved(Component * Component)
{
	DefaultShape.reset(new btEmptyShape);
	RigidBody->setCollisionShape(DefaultShape.get());

	ConstructionInfo.m_localInertia.setZero();
	RigidBody->setMassProps(ConstructionInfo.m_mass, ConstructionInfo.m_localInertia);
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

	nodeComponentAddedListener = node->addComponentAddedEvent(std::bind(&RigidBodyComponent::onNodeComponentAdded, this, std::placeholders::_1));
	nodeComponentRemovedListener = node->addComponentRemovedEvent(std::bind(&RigidBodyComponent::onNodeComponentRemoved, this, std::placeholders::_1));

	static_cast<bulletMotionState*>(RigidBody->getMotionState())->setNode(node);
	RigidBody->setCenterOfMassTransform(node->getWorldTransform());

	auto CollisionShape = node->getComponent<CollisionShapeComponent>();
	if (CollisionShape != nullptr) {
		RigidBody->setCollisionShape(CollisionShape->getCollisionShape());
		DefaultShape.reset();

		if (CollisionShape->getShape() != CollisionShapeComponent::ShapeFormat::Null)
			CollisionShape->getCollisionShape()->calculateLocalInertia(ConstructionInfo.m_mass, ConstructionInfo.m_localInertia);

		RigidBody->setMassProps(ConstructionInfo.m_mass, ConstructionInfo.m_localInertia);
	}

	if (RigidBody->getBroadphaseHandle() == nullptr)
		physicsScene->getWorld()->addRigidBody(RigidBody.get(), Group, Mask);
}
