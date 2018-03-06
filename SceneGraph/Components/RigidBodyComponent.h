#pragma once

#include "../Base/Component.h"

#include <BulletDynamics/Dynamics/btRigidBody.h>

namespace kitsune {
namespace scenegraph {

	class ConstraintComponent;

	class RigidBodyComponent :
		public Component
	{
        KIT_SG_COMPONENT(kitsune::scenegraph::RigidBodyComponent)

	public:
		enum struct RigidBodyType {
			Static,
			Kinematic,
			Dynamic
		};

		RigidBodyComponent(float Mass, RigidBodyType Type);
		virtual ~RigidBodyComponent();

        virtual void setActive(bool State) override;

		void setGroup(short Group);
		short getGroup() const { return Group; }

		void setMask(short Mask);
		short getMask() const { return Mask; }

		void setMass(float Mass);
		float getMass() const { return ConstructionInfo.m_mass; }

		void setDamping(float LinearDamping, float AngularDamping);

		void setLinearDamping(float Damping);
		float getLinearDamping() const { return ConstructionInfo.m_linearDamping; }

		void setAngularDamping(float Damping);
		float getAngularDamping() const { return ConstructionInfo.m_angularDamping; }

		void setFriction(float Friction);
		float getFriction() const { return ConstructionInfo.m_friction; }

		void setRestitution(float Restitution);
		float getRestitution() const { return ConstructionInfo.m_restitution; }

		void setLinearVelocity(const btVector3 &velocity) { RigidBody->setLinearVelocity(velocity); }
		const btVector3& getLinearVelocity() const { return RigidBody->getLinearVelocity(); }

		void setAngularVelocity(const btVector3 &velocity) { RigidBody->setAngularVelocity(velocity); }
		const btVector3& getAngularVelocity() const { return RigidBody->getAngularVelocity(); }

		void setRigidBodyType(RigidBodyType Type);
		RigidBodyType getRigidBodyType() const { return Type; }

	private:
		btRigidBody::btRigidBodyConstructionInfo ConstructionInfo;

		std::unique_ptr<btRigidBody> RigidBody;
		std::unique_ptr<btCollisionShape> DefaultShape;

		short Group;
		
		short Mask;

		void readdBodyToWorld();
		void removeBodyFromWorld();

		void onNodeComponentAdded(Component * Component);
		void onNodeComponentRemoved(Component * Component);

		RigidBodyType Type;

		Node::componentChangedCallback::auto_remover_type nodeComponentAddedListener;
		Node::componentChangedCallback::auto_remover_type nodeComponentRemovedListener;

		friend class ConstraintComponent;

	protected:
        virtual void onNodeSet() override;
	};

}
}
