#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE kitsune::scenegraph::Node
#include <boost/test/unit_test.hpp>
#include <memory>

#include "Base/Component.h"
#include "Base/Node.h"
#include "Base/Scene.h"

namespace sg = kitsune::scenegraph;

std::shared_ptr<sg::Scene> makeScene() {
	std::shared_ptr<sg::Scene> Scene(new sg::Scene);
	Scene->initialize();

	return Scene;
}

BOOST_AUTO_TEST_SUITE(Node)

BOOST_AUTO_TEST_CASE(AddChildNode)
{
	auto Scene = makeScene();
	auto RootNode = Scene->getRootNode();

	BOOST_TEST((RootNode != nullptr), "Scene without root node");

	auto ChildNode = RootNode->addChildNode();

	BOOST_TEST((bool)ChildNode, "Failed to create child node");
	BOOST_TEST((bool)ChildNode->getParentNode(), "Child node without parent");
	BOOST_TEST((ChildNode->getParentNode().get() == RootNode), "Parent is not root");
}

BOOST_AUTO_TEST_CASE(NodeTransform)
{
	auto Scene = makeScene();
	auto RootNode = Scene->getRootNode();
	btTransform testTransform(btQuaternion(btVector3(0, 1, 0), SIMD_HALF_PI), btVector3(1, 0, 0));

	BOOST_TEST((RootNode != nullptr), "Scene without root node");

	RootNode->setLocalOffset(testTransform.getOrigin());
	RootNode->setLocalRotation(testTransform.getRotation());

	BOOST_TEST((RootNode->getWorldTransform().getOrigin() == testTransform.getOrigin()), "Invalid world position");
	BOOST_TEST((RootNode->getLocalOffset() == testTransform.getOrigin()), "Invalid local position");
	BOOST_TEST((RootNode->getWorldTransform().getRotation() == testTransform.getRotation()), "Invalid world rotation");
	BOOST_TEST((RootNode->getLocalRotation() == testTransform.getRotation()), "Invalid local rotation");

	RootNode->resetTransform();

	BOOST_TEST((RootNode->getLocalTransform() == btTransform::getIdentity()), "Root node with non-identity local transform");
	BOOST_TEST((RootNode->getWorldTransform() == btTransform::getIdentity()), "Root node with non-identity world transform");

	RootNode->setWorldTransform(testTransform);

	BOOST_TEST((RootNode->getWorldTransform() == testTransform), "Root node with invalid world transform");

	RootNode->resetTransform();

	RootNode->setLocalTransform(testTransform);

	BOOST_TEST((RootNode->getLocalTransform() == testTransform), "Root node with invalid local transform");
}

BOOST_AUTO_TEST_CASE(NodeTransformPropagation)
{
	auto Scene = makeScene();
	auto RootNode = Scene->getRootNode();
	btTransform testTransform(btQuaternion(btVector3(0, 1, 0), SIMD_HALF_PI), btVector3(1, 0, 0));

	BOOST_TEST((RootNode != nullptr), "Scene without root node");

	RootNode->setLocalTransform(testTransform);
	auto ChildNode = RootNode->addChildNode();

	BOOST_TEST((ChildNode->getWorldTransform().getOrigin() == testTransform.getOrigin()), "Invalid world position");
	BOOST_TEST((ChildNode->getWorldTransform().getRotation() == testTransform.getRotation()), "Invalid world rotation");

	BOOST_TEST((ChildNode->getLocalTransform().getOrigin() == btVector3(0, 0, 0)), "Invalid local position");
	BOOST_TEST((ChildNode->getLocalTransform().getRotation() == btQuaternion::getIdentity()), "Invalid local rotation");

	BOOST_TEST((ChildNode->getTransformToOrigin() == testTransform.inverse()), "Invalid transform to origin");

	RootNode->resetTransform();

	BOOST_TEST((ChildNode->getWorldTransform().getOrigin() == btVector3(0, 0, 0)), "Invalid world position");
	BOOST_TEST((ChildNode->getWorldTransform().getRotation() == btQuaternion::getIdentity()), "Invalid world rotation");

	BOOST_TEST((ChildNode->getTransformToOrigin() == btTransform::getIdentity()), "Invalid transform to origin");
}

BOOST_AUTO_TEST_CASE(NodeActive)
{
	auto Scene = makeScene();
	auto RootNode = Scene->getRootNode();

	BOOST_TEST((RootNode != nullptr), "Scene without root node");

	auto ChildNode = RootNode->addChildNode();

	BOOST_TEST(ChildNode->isActive(), "Expected default values");

	ChildNode->setActive(false);

	BOOST_TEST(!ChildNode->isActive(), "Child node is active");
	BOOST_TEST(RootNode->isActive(), "Root node is not active");

	ChildNode->setActive(true);
	RootNode->setActive(false);

	BOOST_TEST(!ChildNode->isActive(), "Child node is active");
	BOOST_TEST(!RootNode->isActive(), "Root node is active");
}

BOOST_AUTO_TEST_CASE(NodeScene)
{
	std::shared_ptr<sg::Node> Node(new sg::Node(std::weak_ptr<sg::Scene>()));

	BOOST_TEST((Node->getScene() == nullptr), "Unexpected scene for node");
	BOOST_TEST((Node->getParentNode() == nullptr), "Unexpected parent node");

	auto Child = Node->addChildNode();

	BOOST_TEST((Child->getScene() == nullptr), "Unexpected scene for child");
	BOOST_TEST((Child->getParentNode() != nullptr), "Expected parent node");
}

BOOST_AUTO_TEST_CASE(NodeName)
{
	std::shared_ptr<sg::Node> Node(new sg::Node(std::weak_ptr<sg::Scene>()));

	Node->setName("Test Node");

	BOOST_TEST((Node->getName().compare("Test Node") == 0), "Unexpected name");
}

BOOST_AUTO_TEST_CASE(NodeTag)
{
	auto Scene = makeScene();
	auto RootNode = Scene->getRootNode()->shared_from_this();

	RootNode->setTag("test");

	BOOST_TEST((RootNode->getTag().compare("test") == 0), "Unexpected tag");

	auto range = Scene->findNodesByTag("test");
	BOOST_TEST((range.first != range.second), "Invalid range");
	auto nodeRef = range.first->second;
	BOOST_TEST((!(RootNode<nodeRef.lock()) && !(nodeRef.lock()<RootNode)), "Invalid reference");
}

BOOST_AUTO_TEST_SUITE_END()
