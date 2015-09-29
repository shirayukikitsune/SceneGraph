#include "stdafx.h"
#include "CppUnitTest.h"

#include "../SceneGraph/Base/Node.h"
#include "../SceneGraph/Base/PhysicsScene.h"
#include "../SceneGraph/Components/RigidBodyComponent.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace sg = kitsune::scenegraph;

namespace SceneGraphTest
{

	TEST_CLASS(PhysicsTest)
	{
	private:

		std::shared_ptr<sg::PhysicsScene> makeScene() {
			std::shared_ptr<sg::PhysicsScene> Scene(new sg::PhysicsScene);
			Scene->initialize();

			return Scene;
		}

	public:
		TEST_METHOD(PhysicsUpdateEvent)
		{
			auto Scene = makeScene();

			Scene->setGravity(btVector3(0, -10, 0));

			auto Node = Scene->getRootNode()->addChildNode();
			Node->setWorldTransform(btTransform::getIdentity());
			auto RigidBody = Node->createComponent<sg::RigidBodyComponent>(1.0f, sg::RigidBodyComponent::RigidBodyType::Dynamic);

			for (int i = 0; i < 60; ++i)
				Scene->update(1.0f / 60.0f);

			Assert::AreEqual(0.0f, RigidBody->getLinearVelocity().x(), 0.1f, L"X velocity is not zero");
			Assert::AreEqual(-10.0f, RigidBody->getLinearVelocity().y(), 0.1f, L"Unexpected Y velocity");
			Assert::AreEqual(0.0f, RigidBody->getLinearVelocity().z(), 0.1f, L"Z velocity is not zero");

			Assert::AreEqual(0.0f, Node->getWorldTransform().getOrigin().x(), 0.1f, L"X position is not zero");
			Assert::AreEqual(-5.0f, Node->getWorldTransform().getOrigin().y(), 0.1f, L"Unexpected Y position");
			Assert::AreEqual(0.0f, Node->getWorldTransform().getOrigin().z(), 0.1f, L"Z position is not zero");
		}
	};

}
