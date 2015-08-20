#include "stdafx.h"
#include "CppUnitTest.h"

#include "../SceneGraph/Component.h"
#include "../SceneGraph/Node.h"
#include "../SceneGraph/SceneEventComponent.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace sg = kitsune::scenegraph;

namespace SceneGraphTest
{		

	class InvalidComponent
		: public sg::Component
	{
		KIT_SG_COMPONENT(SceneGraphTest::InvalidComponent);

	public:
		InvalidComponent(std::shared_ptr<sg::Node> Node) : Component(Node) {}
		virtual ~InvalidComponent() {}
	};

	class TestEventComponent
		: public sg::SceneEventComponent
	{
		KIT_SG_COMPONENT(SceneGraphTest::TestEventComponent);

	public:
		TestEventComponent(std::shared_ptr<sg::Node> Node) : SceneEventComponent(Node) { updated = 0; }
		virtual ~TestEventComponent() {}

		int updated;

		virtual void onPreUpdate(float DeltaTime) {
			updated = 1;
		}

		virtual void onUpdate(float DeltaTime) {
			updated = 2;
		}

		virtual uint32_t getAttachedEvents() { return (uint32_t)AttachedEvents::PreUpdate | (uint32_t)AttachedEvents::Update; }
	};

	TEST_CLASS(ComponentTest)
	{
	public:
		
		TEST_METHOD(NodeHasComponent)
		{
			std::shared_ptr<sg::Scene> Scene(new sg::Scene);
			Scene->initialize();

			auto Node = Scene->getRootNode()->addChildNode();
			sg::Component *Component = new sg::Component(Node);

			Node->addComponent(Component);

			Assert::IsTrue(Node->hasComponent<sg::Component>(), L"Component not found");
		}

		TEST_METHOD(NodeDoesNotHaveComponent)
		{
			std::shared_ptr<sg::Scene> Scene(new sg::Scene);
			Scene->initialize();

			auto Node = Scene->getRootNode()->addChildNode();
			sg::Component *Component = new sg::Component(Node);

			Node->addComponent(Component);

			Assert::IsFalse(Node->hasComponent<InvalidComponent>(), L"Component found");
		}

		TEST_METHOD(NodeGetComponent)
		{
			std::shared_ptr<sg::Scene> Scene(new sg::Scene);
			Scene->initialize();

			auto Node = Scene->getRootNode()->addChildNode();
			sg::Component *Component = new sg::Component(Node);

			Node->addComponent(Component);

			Assert::IsNotNull(Node->getComponent<sg::Component>(), L"Component not found");
		}

		TEST_METHOD(NodeGetInvalidComponent)
		{
			std::shared_ptr<sg::Scene> Scene(new sg::Scene);
			Scene->initialize();

			auto Node = Scene->getRootNode()->addChildNode();
			sg::Component *Component = new sg::Component(Node);

			Node->addComponent(Component);

			Assert::IsNull(Node->getComponent<InvalidComponent>(), L"Component found");
		}

		TEST_METHOD(NodeDeleteComponent)
		{
			std::shared_ptr<sg::Scene> Scene(new sg::Scene);
			Scene->initialize();

			auto Node = Scene->getRootNode()->addChildNode();
			std::unique_ptr<sg::Component> Component(new sg::Component(Node));

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