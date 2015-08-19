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
	public:
		TEST_METHOD(AddChildNode)
		{
			std::unique_ptr<sg::Scene> Scene(new sg::Scene);

			auto RootNode = Scene->getRootNode();

			Assert::IsNotNull(RootNode, L"Scene without root node");

			auto ChildNode = RootNode->addChildNode();

			Assert::IsTrue((bool)ChildNode, L"Failed to create child node");
			Assert::IsTrue((bool)ChildNode->getParentNode(), L"Child node without parent");
			Assert::IsTrue(ChildNode->getParentNode().get() == RootNode, L"Parent is not root");
		}
	};

}
