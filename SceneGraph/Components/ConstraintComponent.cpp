#include "ConstraintComponent.h"

#include "RigidBodyComponent.h"

#include <BulletDynamics/ConstraintSolver/btConeTwistConstraint.h>
#include <BulletDynamics/ConstraintSolver/btFixedConstraint.h>
#include <BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.h>
#include <BulletDynamics/ConstraintSolver/btGeneric6DofSpring2Constraint.h>
#include <BulletDynamics/ConstraintSolver/btHingeConstraint.h>
#include <BulletDynamics/ConstraintSolver/btPoint2PointConstraint.h>
#include <BulletDynamics/ConstraintSolver/btSliderConstraint.h>

#include <cassert>

using kitsune::scenegraph::ConstraintComponent;

ConstraintComponent::ConstraintComponent()
{
}

ConstraintComponent::~ConstraintComponent()
{
}

void ConstraintComponent::setConstraintType(ConstraintType Type)
{
	this->Type = Type;

	createConstraint();
}

void ConstraintComponent::setAssociatedNode(std::weak_ptr<kitsune::scenegraph::Node> AssociatedNode)
{
	auto associated = AssociatedNode.lock();
	if (!associated)
		return;

	associatedNodeComponentAddedListener = associated->addComponentAddedEvent(std::bind(&ConstraintComponent::onNodeAddComponent, this, std::placeholders::_1));
	associatedNodeComponentRemovedListener = associated->addComponentRemovedEvent(std::bind(&ConstraintComponent::onNodeRemoveComponent, this, std::placeholders::_1));

	createConstraint();
}

void ConstraintComponent::setSpringActivation(int DoF, bool SpringActivation)
{
	setConstraintLimits();

	assert("Invalid DoF" && DoF >= 0 && DoF <= 5);

	this->SpringActivation[DoF] = SpringActivation;
}

void ConstraintComponent::setSpringConstant(int DoF, float SpringConstant)
{
	setConstraintLimits();
	
	assert("Invalid DoF" && DoF >= 0 && DoF <= 5);

	this->SpringConstants[DoF] = SpringConstant;
}

void ConstraintComponent::createConstraint()
{
	auto node = getNode();
	auto associated = AssociatedNode.lock();

	assert("Constraint not associated" && (!associated || !node));
	assert("Constraint associated nodes with no rigid body" && (!associated->hasComponent<RigidBodyComponent>() || !node->hasComponent<RigidBodyComponent>()));

	btTransform frameFromB = getOffsetFromNode().inverseTimes(associated->getLocalTransform());

	switch (Type)
	{
	case ConstraintType::PointToPoint:
		this->Constraint.reset(new btPoint2PointConstraint(*node->getComponent<RigidBodyComponent>()->RigidBody.get(), *associated->getComponent<RigidBodyComponent>()->RigidBody.get(), getOffsetFromNode().getOrigin(), frameFromB.getOrigin()));
		break;
	case ConstraintType::Hinge:
		this->Constraint.reset(new btHingeConstraint(*node->getComponent<RigidBodyComponent>()->RigidBody.get(), *associated->getComponent<RigidBodyComponent>()->RigidBody.get(), getOffsetFromNode(), frameFromB));
		break;
	case ConstraintType::Slider:
		this->Constraint.reset(new btSliderConstraint(*node->getComponent<RigidBodyComponent>()->RigidBody.get(), *associated->getComponent<RigidBodyComponent>()->RigidBody.get(), getOffsetFromNode(), frameFromB, true));
		break;
	case ConstraintType::ConeTwist:
		Constraint.reset(new btConeTwistConstraint(*node->getComponent<RigidBodyComponent>()->RigidBody.get(), *associated->getComponent<RigidBodyComponent>()->RigidBody.get(), getOffsetFromNode(), frameFromB));
		break;
	case ConstraintType::Generic6DoF:
		this->Constraint.reset(new btGeneric6DofConstraint(*node->getComponent<RigidBodyComponent>()->RigidBody.get(), *associated->getComponent<RigidBodyComponent>()->RigidBody.get(), getOffsetFromNode(), frameFromB, true));
		break;
	case ConstraintType::Generic6DoFSpring:
		this->Constraint.reset(new btGeneric6DofSpring2Constraint(*node->getComponent<RigidBodyComponent>()->RigidBody.get(), *associated->getComponent<RigidBodyComponent>()->RigidBody.get(), getOffsetFromNode(), frameFromB));
		break;
	case ConstraintType::Fixed:
		this->Constraint.reset(new btFixedConstraint(*node->getComponent<RigidBodyComponent>()->RigidBody.get(), *associated->getComponent<RigidBodyComponent>()->RigidBody.get(), getOffsetFromNode(), frameFromB));
		break;
	default:
		return;
	}

	setConstraintLimits();
}

void ConstraintComponent::setConstraintLimits()
{
	switch (Type)
	{
	case ConstraintType::ConeTwist:
	{
		auto Constraint = static_cast<btConeTwistConstraint*>(this->Constraint.get());
		break;
	}
	case ConstraintType::Generic6DoF:
	{
		auto Constraint = static_cast<btGeneric6DofConstraint*>(this->Constraint.get());

		Constraint->setAngularLowerLimit(getLowerAngularLimit());
		Constraint->setAngularUpperLimit(getUpperAngularLimit());
		Constraint->setLinearLowerLimit(getLowerLinearLimit());
		Constraint->setLinearUpperLimit(getUpperLinearLimit());

		break;
	}
	case ConstraintType::Generic6DoFSpring:
	{
		auto Constraint = static_cast<btGeneric6DofSpring2Constraint*>(this->Constraint.get());

		Constraint->setAngularLowerLimit(getLowerAngularLimit());
		Constraint->setAngularUpperLimit(getUpperAngularLimit());
		Constraint->setLinearLowerLimit(getLowerLinearLimit());
		Constraint->setLinearUpperLimit(getUpperLinearLimit());

		for (int i = 0; i < 6; ++i) {
			Constraint->enableSpring(i, getSpringActivation(i));
			Constraint->setStiffness(i, getSpringConstant(i));
		}

		break;
	}
	case ConstraintType::Hinge:
	{
		auto Constraint = static_cast<btHingeConstraint*>(this->Constraint.get());

		Constraint->setLimit(getLowerAngularLimit().x(), getUpperAngularLimit().x());

		break;
	}
	default:
		return;
	}
}

void ConstraintComponent::onNodeAddComponent(Component * Component)
{
	if (Component->getComponentNameHash() != RigidBodyComponent::componentNameHash)
		return;

	createConstraint();
}

void ConstraintComponent::onNodeRemoveComponent(Component * Component)
{
	if (Component->getComponentNameHash() != RigidBodyComponent::componentNameHash)
		return;

	Constraint.reset();
}

void ConstraintComponent::onNodeSet()
{
	auto node = getNode();

	if (!node)
		return;

	nodeComponentAddedListener = node->addComponentAddedEvent(std::bind(&ConstraintComponent::onNodeAddComponent, this, std::placeholders::_1));
	nodeComponentRemovedListener = node->addComponentRemovedEvent(std::bind(&ConstraintComponent::onNodeRemoveComponent, this, std::placeholders::_1));

	createConstraint();
}
