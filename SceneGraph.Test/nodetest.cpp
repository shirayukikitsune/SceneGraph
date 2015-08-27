#include "stdafx.h"
#include "CppUnitTest.h"

#include "../SceneGraph/Component.h"
#include "../SceneGraph/Node.h"
#include "../SceneGraph/Scene.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace sg = kitsune::scenegraph;

namespace SceneGraphTest
{

	TEST_CLASS(NodeTest)
	{
	private:
		std::shared_ptr<sg::Scene> makeScene() {
			std::shared_ptr<sg::Scene> Scene(new sg::Scene);
			Scene->initialize();

			return Scene;
		}

	public:
		TEST_METHOD(AddChildNode)
		{
			auto Scene = makeScene();
			auto RootNode = Scene->getRootNode();

			Assert::IsNotNull(RootNode, L"Scene without root node");

			auto ChildNode = RootNode->addChildNode();

			Assert::IsTrue((bool)ChildNode, L"Failed to create child node");
			Assert::IsTrue((bool)ChildNode->getParentNode(), L"Child node without parent");
			Assert::IsTrue(ChildNode->getParentNode().get() == RootNode, L"Parent is not root");
		}

		TEST_METHOD(NodeTransform)
		{
			auto Scene = makeScene();
			auto RootNode = Scene->getRootNode();
			btTransform testTransform(btQuaternion(btVector3(0, 1, 0), SIMD_HALF_PI), btVector3(1, 0, 0));

			Assert::IsNotNull(RootNode, L"Scene without root node");

			RootNode->setLocalOffset(testTransform.getOrigin());
			RootNode->setLocalRotation(testTransform.getRotation());

			Assert::IsTrue(RootNode->getWorldTransform().getOrigin() == testTransform.getOrigin(), L"Invalid world position");
			Assert::IsTrue(RootNode->getLocalOffset() == testTransform.getOrigin(), L"Invalid local position");
			Assert::IsTrue(RootNode->getWorldTransform().getRotation() == testTransform.getRotation(), L"Invalid world rotation");
			Assert::IsTrue(RootNode->getLocalRotation() == testTransform.getRotation(), L"Invalid local rotation");

			RootNode->resetTransform();

			Assert::IsTrue(RootNode->getLocalTransform() == btTransform::getIdentity(), L"Root node with non-identity local transform");
			Assert::IsTrue(RootNode->getWorldTransform() == btTransform::getIdentity(), L"Root node with non-identity world transform");

			RootNode->setWorldTransform(testTransform);

			Assert::IsTrue(RootNode->getWorldTransform() == testTransform, L"Root node with invalid world transform");

			RootNode->resetTransform();

			RootNode->setLocalTransform(testTransform);

			Assert::IsTrue(RootNode->getLocalTransform() == testTransform, L"Root node with invalid local transform");
		}

		TEST_METHOD(NodeTransformPropagation)
		{
			auto Scene = makeScene();
			auto RootNode = Scene->getRootNode();
			btTransform testTransform(btQuaternion(btVector3(0, 1, 0), SIMD_HALF_PI), btVector3(1, 0, 0));

			Assert::IsNotNull(RootNode, L"Scene without root node");

			RootNode->setLocalTransform(testTransform);
			auto ChildNode = RootNode->addChildNode();

			Assert::IsTrue(ChildNode->getWorldTransform().getOrigin() == testTransform.getOrigin(), L"Invalid world position");
			Assert::IsTrue(ChildNode->getWorldTransform().getRotation() == testTransform.getRotation(), L"Invalid world rotation");

			Assert::IsTrue(ChildNode->getLocalTransform().getOrigin() == btVector3(0, 0, 0), L"Invalid local position");
			Assert::IsTrue(ChildNode->getLocalTransform().getRotation() == btQuaternion::getIdentity(), L"Invalid local rotation");

			Assert::IsTrue(ChildNode->getTransformToOrigin() == testTransform.inverse(), L"Invalid transform to origin");

			RootNode->resetTransform();

			Assert::IsTrue(ChildNode->getWorldTransform().getOrigin() == btVector3(0, 0, 0), L"Invalid world position");
			Assert::IsTrue(ChildNode->getWorldTransform().getRotation() == btQuaternion::getIdentity(), L"Invalid world rotation");

			Assert::IsTrue(ChildNode->getTransformToOrigin() == btTransform::getIdentity(), L"Invalid transform to origin");
		}

		TEST_METHOD(NodeActive)
		{
			auto Scene = makeScene();
			auto RootNode = Scene->getRootNode();

			Assert::IsNotNull(RootNode, L"Scene without root node");

			auto ChildNode = RootNode->addChildNode();

			Assert::IsTrue(ChildNode->isActive(), L"Expected default values");

			ChildNode->setActive(false);

			Assert::IsFalse(ChildNode->isActive(), L"Child node is active");
			Assert::IsTrue(RootNode->isActive(), L"Root node is not active");

			ChildNode->setActive(true);
			RootNode->setActive(false);

			Assert::IsFalse(ChildNode->isActive(), L"Child node is active");
			Assert::IsFalse(RootNode->isActive(), L"Root node is active");
		}

		TEST_METHOD(NodeScene)
		{
			std::shared_ptr<sg::Node> Node(new sg::Node(std::weak_ptr<sg::Scene>()));

			Assert::IsTrue(Node->getScene() == nullptr, L"Unexpected scene for node");
			Assert::IsTrue(Node->getParentNode() == nullptr, L"Unexpected parent node");

			auto Child = Node->addChildNode();

			Assert::IsTrue(Child->getScene() == nullptr, L"Unexpected scene for child");
			Assert::IsFalse(Child->getParentNode() == nullptr, L"Expected parent node");
		}
	};

}
