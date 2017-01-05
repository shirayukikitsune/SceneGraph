#include "stdafx.h"
#include "CppUnitTest.h"

#include "../SceneGraph/Base/Node.h"
#include "../SceneGraph/Base/PhysicsScene.h"
#include "../SceneGraph/Components/CollisionShapeComponent.h"
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
            auto CollisionShape = Node->createComponent<sg::CollisionShapeComponent>();
            CollisionShape->setDimentions(btVector3(1, 1, 1));
            CollisionShape->setShape(sg::CollisionShapeComponent::ShapeFormat::Box);
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

		TEST_METHOD(PhysicsCollision)
		{
			auto Scene = makeScene();

			auto PlaneNode = Scene->getRootNode()->addChildNode();
			PlaneNode->setWorldTransform(btTransform(btQuaternion::getIdentity(), btVector3(0, -1.0f, 0)));
			auto CollisionShape = PlaneNode->createComponent<sg::CollisionShapeComponent>();
			CollisionShape->setDimentions(btVector3(0, 1, 0)); // Up vector
			CollisionShape->setPlaneConstant(1.0f);
			CollisionShape->setShape(sg::CollisionShapeComponent::ShapeFormat::Plane);
			auto RigidBody = PlaneNode->createComponent<sg::RigidBodyComponent>(0.0f, sg::RigidBodyComponent::RigidBodyType::Static);

			auto BoxNode = Scene->getRootNode()->addChildNode();
			BoxNode->setWorldTransform(btTransform(btQuaternion::getIdentity(), btVector3(0, 50, 0)));
			CollisionShape = BoxNode->createComponent<sg::CollisionShapeComponent>();
			CollisionShape->setDimentions(btVector3(1, 1, 1));
			CollisionShape->setShape(sg::CollisionShapeComponent::ShapeFormat::Box);
			RigidBody = BoxNode->createComponent<sg::RigidBodyComponent>(1.0f, sg::RigidBodyComponent::RigidBodyType::Dynamic);
			
			for (int i = 0; i < 10000; ++i)
				Scene->update(1.0f / 60.0f);

			Assert::AreEqual(1.0f, BoxNode->getWorldTransform().getOrigin().y(), 0.1f, L"Unexpected Y position");
		}
	};

}
