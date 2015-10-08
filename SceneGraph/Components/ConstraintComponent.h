#pragma once

#include "../Base/Component.h"

#include <BulletDynamics/ConstraintSolver/btTypedConstraint.h>

#include <array>
#include <bitset>
#include <memory>

namespace kitsune {
namespace scenegraph {
	
	class ConstraintComponent :
		public Component
	{
		KIT_SG_COMPONENT(kitsune::scenegraph::ConstraintComponent);

	public:
		enum class ConstraintType {
			PointToPoint,
			Hinge,
			Slider,
			ConeTwist,
			Generic6DoF,
			Generic6DoFSpring,
			Fixed
		};

		ConstraintComponent();
		~ConstraintComponent();

		void setConstraintType(ConstraintType Type);
		ConstraintType getConstraintType() const { return this->Type; }

		void setOffsetFromNode(const btTransform& OffsetFromNode) { this->OffsetFromNode = OffsetFromNode; }
		const btTransform & getOffsetFromNode() const { return this->OffsetFromNode; }

		void setAssociatedNode(std::weak_ptr<kitsune::scenegraph::Node> AssociatedNode);
		std::weak_ptr<kitsune::scenegraph::Node> getAssociatedRigidBody() const { return AssociatedNode; }

		void setLowerAngularLimit(const btVector3 & LowerAngularLimit) { setConstraintLimits(); this->LowerAngularLimit = LowerAngularLimit; }
		const btVector3& getLowerAngularLimit() const { return this->LowerAngularLimit; }

		void setLowerLinearLimit(const btVector3 & LowerLinearLimit) { setConstraintLimits(); this->LowerLinearLimit = LowerLinearLimit; }
		const btVector3& getLowerLinearLimit() const { return this->LowerLinearLimit; }

		void setUpperAngularLimit(const btVector3 & UpperAngularLimit) { setConstraintLimits(); this->UpperAngularLimit = UpperAngularLimit; }
		const btVector3& getUpperAngularLimit() const { return this->UpperAngularLimit; }

		void setUpperLinearLimit(const btVector3 & UpperLinearLimit) { setConstraintLimits(); this->UpperLinearLimit = UpperLinearLimit; }
		const btVector3& getUpperLinearLimit() const { return this->UpperLinearLimit; }

		void setSpringConstant(int DoF, float SpringConstant) { setConstraintLimits(); this->SpringConstants[DoF] = SpringConstant; }
		float getSpringConstant(int DoF) const { return this->SpringConstants[DoF]; }

		void setSpringActivation(int DoF, bool SpringActivation) { setConstraintLimits(); this->SpringActivation[DoF] = SpringActivation; }
		bool getSpringActivation(int DoF) const { return this->SpringActivation[DoF]; }

	private:
		void createConstraint();
		void setConstraintLimits();

		void onNodeAddComponent(Component * Component);
		void onNodeRemoveComponent(Component * Component);

		std::unique_ptr<btTypedConstraint> Constraint;

		ConstraintType Type;

		btTransform OffsetFromNode;

		std::weak_ptr<kitsune::scenegraph::Node> AssociatedNode;

		Node::componentChangedCallback::auto_remover_type nodeComponentAddedListener, associatedNodeComponentAddedListener;
		Node::componentChangedCallback::auto_remover_type nodeComponentRemovedListener, associatedNodeComponentRemovedListener;

		btVector3 LowerAngularLimit, LowerLinearLimit;
		btVector3 UpperAngularLimit, UpperLinearLimit;

		std::array<float, 6> SpringConstants;
		std::bitset<6> SpringActivation;

	protected:
		virtual void onNodeSet();
	};

}
}

