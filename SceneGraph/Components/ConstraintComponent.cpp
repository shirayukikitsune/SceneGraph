#include <memory>

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

void ConstraintComponent::setConstraintType(ConstraintType Type) {
    this->Type = Type;

    createConstraint();
}

void ConstraintComponent::setAssociatedNode(std::weak_ptr<kitsune::scenegraph::Node> AssociatedNode) {
    auto associated = AssociatedNode.lock();
    if (!associated) {
        return;
    }

    associatedNodeComponentAddedListener = associated->addComponentAddedEvent(
            std::bind(&ConstraintComponent::onNodeAddComponent, this, std::placeholders::_1));
    associatedNodeComponentRemovedListener = associated->addComponentRemovedEvent(
            std::bind(&ConstraintComponent::onNodeRemoveComponent, this, std::placeholders::_1));

    createConstraint();
}

void ConstraintComponent::setSpringActivation(int DoF, bool SpringActivation) {
    setConstraintLimits();

    assert("Invalid DoF" && DoF >= 0 && DoF <= 5);

    this->SpringActivation[DoF] = SpringActivation;
}

void ConstraintComponent::setSpringConstant(int DoF, float SpringConstant) {
    setConstraintLimits();

    assert("Invalid DoF" && DoF >= 0 && DoF <= 5);

    this->SpringConstants[DoF] = SpringConstant;
}

void ConstraintComponent::createConstraint() {
    auto node = getNode();
    auto associated = AssociatedNode.lock();

    assert("Constraint not associated" && (!associated || !node));
    assert("Constraint associated nodes with no rigid body" &&
           (!associated->hasComponent<RigidBodyComponent>() || !node->hasComponent<RigidBodyComponent>()));

    btTransform frameFromB = getOffsetFromNode().inverseTimes(btTransform(
            btQuaternion(associated->getLocalRotation().x, associated->getLocalRotation().y,
                         associated->getLocalRotation().z, associated->getLocalRotation().w),
            btVector3(associated->getLocalOffset().x, associated->getLocalOffset().y, associated->getLocalOffset().z)));

    switch (Type) {
        case ConstraintType::PointToPoint:
            Constraint = std::make_unique<btPoint2PointConstraint>(
                    *node->getComponent<RigidBodyComponent>()->RigidBody,
                    *associated->getComponent<RigidBodyComponent>()->RigidBody,
                    getOffsetFromNode().getOrigin(),
                    frameFromB.getOrigin());
            break;
        case ConstraintType::Hinge:
            Constraint = std::make_unique<btHingeConstraint>(
                    *node->getComponent<RigidBodyComponent>()->RigidBody,
                    *associated->getComponent<RigidBodyComponent>()->RigidBody,
                    getOffsetFromNode(),
                    frameFromB);
            break;
        case ConstraintType::Slider:
            Constraint = std::make_unique<btSliderConstraint>(
                    *node->getComponent<RigidBodyComponent>()->RigidBody,
                    *associated->getComponent<RigidBodyComponent>()->RigidBody,
                    getOffsetFromNode(),
                    frameFromB,
                    true);
            break;
        case ConstraintType::ConeTwist:
            Constraint = std::make_unique<btConeTwistConstraint>(
                    *node->getComponent<RigidBodyComponent>()->RigidBody,
                    *associated->getComponent<RigidBodyComponent>()->RigidBody,
                    getOffsetFromNode(),
                    frameFromB);
            break;
        case ConstraintType::Generic6DoF:
            Constraint = std::make_unique<btGeneric6DofConstraint>(
                    *node->getComponent<RigidBodyComponent>()->RigidBody,
                    *associated->getComponent<RigidBodyComponent>()->RigidBody,
                    getOffsetFromNode(),
                    frameFromB,
                    true);
            break;
        case ConstraintType::Generic6DoFSpring:
            Constraint = std::make_unique<btGeneric6DofSpring2Constraint>(
                    *node->getComponent<RigidBodyComponent>()->RigidBody,
                    *associated->getComponent<RigidBodyComponent>()->RigidBody,
                    getOffsetFromNode(),
                    frameFromB);
            break;
        case ConstraintType::Fixed:
            Constraint = std::make_unique<btFixedConstraint>(
                    *node->getComponent<RigidBodyComponent>()->RigidBody,
                    *associated->getComponent<RigidBodyComponent>()->RigidBody,
                    getOffsetFromNode(),
                    frameFromB);
            break;
    }

    setConstraintLimits();
}

void ConstraintComponent::setConstraintLimits() {
    switch (Type) {
        case ConstraintType::ConeTwist: {
            auto Constraint = dynamic_cast<btConeTwistConstraint *>(this->Constraint.get());
            break;
        }
        case ConstraintType::Generic6DoF: {
            auto Constraint = dynamic_cast<btGeneric6DofConstraint *>(this->Constraint.get());

            Constraint->setAngularLowerLimit(getLowerAngularLimit());
            Constraint->setAngularUpperLimit(getUpperAngularLimit());
            Constraint->setLinearLowerLimit(getLowerLinearLimit());
            Constraint->setLinearUpperLimit(getUpperLinearLimit());

            break;
        }
        case ConstraintType::Generic6DoFSpring: {
            auto Constraint = dynamic_cast<btGeneric6DofSpring2Constraint *>(this->Constraint.get());

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
        case ConstraintType::Hinge: {
            auto Constraint = dynamic_cast<btHingeConstraint *>(this->Constraint.get());

            Constraint->setLimit(getLowerAngularLimit().x(), getUpperAngularLimit().x());

            break;
        }
        default:
            return;
    }
}

void ConstraintComponent::onNodeAddComponent(Component *Component) {
    if (Component->getComponentNameHash() != RigidBodyComponent::componentNameHash)
        return;

    createConstraint();
}

void ConstraintComponent::onNodeRemoveComponent(Component *Component) {
    if (Component->getComponentNameHash() != RigidBodyComponent::componentNameHash)
        return;

    Constraint.reset();
}

void ConstraintComponent::onNodeSet() {
    auto node = getNode();

    if (!node)
        return;

    nodeComponentAddedListener = node->addComponentAddedEvent(
            std::bind(&ConstraintComponent::onNodeAddComponent, this, std::placeholders::_1));
    nodeComponentRemovedListener = node->addComponentRemovedEvent(
            std::bind(&ConstraintComponent::onNodeRemoveComponent, this, std::placeholders::_1));

    createConstraint();
}
