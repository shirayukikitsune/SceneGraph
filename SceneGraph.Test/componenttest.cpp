#include "stdafx.h"
#include "CppUnitTest.h"

#include "../SceneGraph/Component.h"
#include "../SceneGraph/Node.h"

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

	TEST_CLASS(ComponentTest)
	{
	public:
		
		TEST_METHOD(NodeHasComponent)
		{
			std::shared_ptr<kitsune::scenegraph::Node> Node(new kitsune::scenegraph::Node);
			sg::Component *Component = new sg::Component(Node);

			Node->addComponent(Component);

			Assert::IsTrue(Node->hasComponent<sg::Component>(), L"Component not found");
		}

		TEST_METHOD(NodeDoesNotHaveComponent)
		{
			std::shared_ptr<kitsune::scenegraph::Node> Node(new kitsune::scenegraph::Node);
			sg::Component *Component = new sg::Component(Node);

			Node->addComponent(Component);

			Assert::IsFalse(Node->hasComponent<InvalidComponent>(), L"Component found");
		}

		TEST_METHOD(NodeGetComponent)
		{
			std::shared_ptr<kitsune::scenegraph::Node> Node(new kitsune::scenegraph::Node);
			sg::Component *Component = new sg::Component(Node);

			Node->addComponent(Component);

			Assert::IsNotNull(Node->getComponent<sg::Component>(), L"Component not found");
		}

		TEST_METHOD(NodeGetInvalidComponent)
		{
			std::shared_ptr<kitsune::scenegraph::Node> Node(new kitsune::scenegraph::Node);
			sg::Component *Component = new sg::Component(Node);

			Node->addComponent(Component);

			Assert::IsNull(Node->getComponent<InvalidComponent>(), L"Component found");
		}

		TEST_METHOD(NodeDeleteComponent)
		{
			std::shared_ptr<kitsune::scenegraph::Node> Node(new kitsune::scenegraph::Node);
			std::unique_ptr<sg::Component> Component(new sg::Component(Node));

			Node->addComponent(Component.get());

			Node.reset();

			try {
				Component.reset(); // This should throw an access violation

				Assert::IsFalse(true, L"Component not deleted");
			}
			catch (...) {
			}
		}

	};
}