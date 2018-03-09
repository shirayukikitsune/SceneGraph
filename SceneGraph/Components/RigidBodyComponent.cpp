#include "RigidBodyComponent.h"

#include "CollisionShapeComponent.h"
#include "../Base/PhysicsScene.h"
#include "../Util/Util.h"

#include <BulletCollision/CollisionShapes/btEmptyShape.h>

#include <memory>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

using kitsune::scenegraph::RigidBodyComponent;
namespace sg = kitsune::scenegraph;

namespace kitsune {
namespace scenegraph {
	class bulletMotionState : public btMotionState
	{
	private:
        std::weak_ptr<Node> ParentNode;
		
	public:
		bulletMotionState()
		{

		}

		virtual ~bulletMotionState()
		{

		}

		void setNode(std::weak_ptr<kitsune::scenegraph::Node> Node)
		{
            this->ParentNode = Node;
		}

		virtual void getWorldTransform(btTransform & WorldTransform) const
		{
            if (auto Ptr = ParentNode.lock()) {
                auto transform = Ptr->getWorldTransform();
                glm::vec3 scale, translation, skew;
                glm::quat rotation;
                glm::vec4 perspective;
                glm::decompose(transform, scale, rotation, translation, skew, perspective);
                WorldTransform = btTransform(btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w), btVector3(translation.x, translation.y, translation.z));
			}
			else WorldTransform = btTransform::getIdentity();
		}

		virtual void setWorldTransform(const btTransform & WorldTransform)
		{
            if (auto Ptr = ParentNode.lock()) {
                auto transform = Ptr->getWorldTransform();
                glm::vec3 scale, translation, skew;
                glm::quat rotation;
                glm::vec4 perspective;
                glm::decompose(transform, scale, rotation, translation, skew, perspective);

                auto r = WorldTransform.getRotation();

                Ptr->setWorldTransform(glm::translate(glm::mat4(), glm::vec3(WorldTransform.getOrigin().x(), WorldTransform.getOrigin().y(), WorldTransform.getOrigin().z()))
                    * glm::toMat4(glm::quat(r.x(), r.y(), r.z(), r.w()))
                    * glm::scale(glm::mat4(), scale));
			}
		}
	};
}
}

RigidBodyComponent::RigidBodyComponent(float Mass, RigidBodyType Type)
	: Group(1), Mask(-1), ConstructionInfo(Mass, new bulletMotionState, nullptr), Type(Type)
{
	DefaultShape.reset(new btEmptyShape);
	RigidBody.reset(new btRigidBody(ConstructionInfo));

	setRigidBodyType(Type);
}

RigidBodyComponent::~RigidBodyComponent()
{
	removeBodyFromWorld();
    setNode(std::weak_ptr<Node>());

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
        flags = util::addFlag(util::removeFlag(flags, btCollisionObject::CF_KINEMATIC_OBJECT), btCollisionObject::CF_STATIC_OBJECT);
		break;
	case RigidBodyType::Kinematic:
        flags = util::removeFlag(util::addFlag(flags, btCollisionObject::CF_KINEMATIC_OBJECT), btCollisionObject::CF_STATIC_OBJECT);
		break;
	case RigidBodyType::Dynamic:
        flags = util::removeFlag(flags, btCollisionObject::CF_KINEMATIC_OBJECT | btCollisionObject::CF_STATIC_OBJECT);
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
    // Ensure that events are reset
    nodeComponentAddedListener.reset();
    nodeComponentRemovedListener.reset();

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

    auto transform = node->getWorldTransform();
    glm::vec3 scale, translation, skew;
    glm::quat rotation;
    glm::vec4 perspective;
    glm::decompose(transform, scale, rotation, translation, skew, perspective);
    btTransform wt(btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w), btVector3(translation.x, translation.y, translation.z));

	RigidBody->setCenterOfMassTransform(wt);

	auto CollisionShape = node->getComponent<CollisionShapeComponent>();
	if (CollisionShape != nullptr) {
		RigidBody->setCollisionShape(CollisionShape->getCollisionShape());

		if (CollisionShape->getShape() != CollisionShapeComponent::ShapeFormat::Null)
			CollisionShape->getCollisionShape()->calculateLocalInertia(ConstructionInfo.m_mass, ConstructionInfo.m_localInertia);
	}
	else {
		RigidBody->setCollisionShape(DefaultShape.get());
	}

	RigidBody->setMassProps(ConstructionInfo.m_mass, ConstructionInfo.m_localInertia);

	physicsScene->getWorld()->removeRigidBody(RigidBody.get());
	physicsScene->getWorld()->addRigidBody(RigidBody.get(), Group, Mask);
}
