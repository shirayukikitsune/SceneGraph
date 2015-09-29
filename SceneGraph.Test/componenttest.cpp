#include "stdafx.h"
#include "CppUnitTest.h"

#include "../SceneGraph/Base/Component.h"
#include "../SceneGraph/Base/Node.h"
#include "../SceneGraph/Components/SceneEventComponent.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace sg = kitsune::scenegraph;

namespace SceneGraphTest
{		

	class InvalidComponent
		: public sg::Component
	{
		KIT_SG_COMPONENT(SceneGraphTest::InvalidComponent);

	public:
		InvalidComponent() : Component() {}
		virtual ~InvalidComponent() {}
	};

	class TestEventComponent
		: public sg::SceneEventComponent
	{
		KIT_SG_COMPONENT(SceneGraphTest::TestEventComponent);

	public:
		TestEventComponent() : SceneEventComponent() { updated = 0; }
		virtual ~TestEventComponent() {}

		int updated;

		virtual void onPreUpdate(float DeltaTime) {
			updated = 1;

			sg::SceneEventComponent::onPreUpdate(DeltaTime);
		}

		virtual void onUpdate(float DeltaTime) {
			updated = 2;

			sg::SceneEventComponent::onUpdate(DeltaTime);
		}

		virtual uint32_t getAttachedEvents() { return (uint32_t)AttachedEvents::PreUpdate | (uint32_t)AttachedEvents::Update; }
	};

	TEST_CLASS(ComponentTest)
	{
	public:

		TEST_METHOD(ComponentIsActive)
		{
			sg::Component *Component = new sg::Component();

			Assert::IsTrue(Component->isLocalActive(), L"Component is not locally active");
			Assert::IsTrue(Component->isActive(), L"Component is not active");

			Component->setActive(false);

			Assert::IsFalse(Component->isLocalActive(), L"Component is locally active");
			Assert::IsFalse(Component->isActive(), L"Component is active");
		}

		TEST_METHOD(NodeComponentIsActive)
		{
			std::shared_ptr<sg::Node> Node(new sg::Node());
			auto Component = Node->createComponent<sg::Component>();

			Assert::IsNotNull(Component, L"Failed to create component");

			Assert::IsTrue(Node->isActive(), L"Node is not active");
			Assert::IsTrue(Component->isActive(), L"Component is not active");

			Node->setActive(false);

			Assert::IsFalse(Component->isActive(), L"Component is active");
			Assert::IsTrue(Component->isLocalActive(), L"Component is not locally active");

			Node->setActive(true);
			Component->setActive(false);

			Assert::IsFalse(Component->isActive(), L"Component is active");
			Assert::IsFalse(Component->isLocalActive(), L"Component is locally active");
		}
		
		TEST_METHOD(NodeHasComponent)
		{
			std::shared_ptr<sg::Scene> Scene(new sg::Scene);
			Scene->initialize();

			auto Node = Scene->getRootNode()->addChildNode();
			sg::Component *Component = new sg::Component();

			Node->addComponent(Component);

			Assert::IsTrue(Node->hasComponent<sg::Component>(), L"Component not found");
		}

		TEST_METHOD(NodeDoesNotHaveComponent)
		{
			std::shared_ptr<sg::Scene> Scene(new sg::Scene);
			Scene->initialize();

			auto Node = Scene->getRootNode()->addChildNode();
			sg::Component *Component = new sg::Component();

			Node->addComponent(Component);

			Assert::IsFalse(Node->hasComponent<InvalidComponent>(), L"Component found");
		}

		TEST_METHOD(NodeGetComponent)
		{
			std::shared_ptr<sg::Scene> Scene(new sg::Scene);
			Scene->initialize();

			auto Node = Scene->getRootNode()->addChildNode();
			sg::Component *Component = new sg::Component();

			Node->addComponent(Component);

			Assert::IsNotNull(Node->getComponent<sg::Component>(), L"Component not found");
		}

		TEST_METHOD(ComponentHasNodeAndScene)
		{
			sg::Component *Component = new sg::Component();

			Assert::IsTrue(Component->getScene() == nullptr, L"Component belongs to a scene");
			Assert::IsTrue(Component->getNode() == nullptr, L"Component belongs to a node");

			delete Component;

			std::shared_ptr<sg::Scene> Scene(new sg::Scene);
			Scene->initialize();

			auto Node = Scene->getRootNode();
			Component = Node->createComponent<sg::Component>();

			Assert::IsFalse(Component->getScene() == nullptr, L"Component doesn't belongs to a scene");
			Assert::IsFalse(Component->getNode() == nullptr, L"Component doesn't belongs to a node");
		}

		TEST_METHOD(NodeGetInvalidComponent)
		{
			std::shared_ptr<sg::Scene> Scene(new sg::Scene);
			Scene->initialize();

			auto Node = Scene->getRootNode()->addChildNode();
			sg::Component *Component = new sg::Component();

			Node->addComponent(Component);

			Assert::IsNull(Node->getComponent<InvalidComponent>(), L"Component found");
		}

		TEST_METHOD(NodeDeleteComponent)
		{
			std::shared_ptr<sg::Scene> Scene(new sg::Scene);
			Scene->initialize();

			auto Node = Scene->getRootNode()->addChildNode();
			std::unique_ptr<sg::Component> Component(new sg::Component());

			Node->addComponent(Component.get());

			Node.reset();
			Scene.reset();

			bool fail = false;
			try {
				Component.reset(); // This should throw an access violation
				fail = true;
			}
			catch (...) {
			}

			Assert::IsFalse(fail, L"Component not deleted");
		}

		TEST_METHOD(SceneEventComponent)
		{
			std::shared_ptr<sg::Scene> Scene(new sg::Scene);
			Scene->initialize();

			auto Node = Scene->getRootNode()->addChildNode();
			auto Component = Node->createComponent<TestEventComponent>();

			Component->registerEvents();

			Scene->update(100.0f);

			Assert::IsTrue(Component->updated == 2, L"Update events not called");
		}

	};
}