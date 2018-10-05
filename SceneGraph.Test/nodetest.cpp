#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE kitsune::scenegraph::Node
#include <boost/test/unit_test.hpp>
#include <memory>
#include <iostream>

#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Base/Component.h"
#include "Base/Node.h"
#include "Base/Scene.h"
#include "Util/Constants.h"

namespace sg = kitsune::scenegraph;

std::shared_ptr<sg::Scene> makeScene() {
	std::shared_ptr<sg::Scene> Scene(new sg::Scene);
	Scene->initialize();

	return Scene;
}

bool mat4Compare(glm::mat4 &a, glm::mat4 &b, double epsilon) {
	for (int i = 0; i < 16; ++i) {
		if (fabs(a[i / 4][i % 4] - b[i / 4][i % 4]) > epsilon) {
			std::cout << "i: " << i << "; a: " << a[i / 4][i % 4] << "; b: " << b[i / 4][i % 4] << std::endl;
			return false;
		}
	}

	return true;
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
	glm::vec3 translation(1.0f, 0, 0);
	glm::quat rotation(0, 1.0f, 0, kitsune::scenegraph::util::PI_2);
	glm::mat4 testTransform = glm::translate(glm::mat4(1.0f), translation) * glm::toMat4(rotation);

	BOOST_TEST((RootNode != nullptr), "Scene without root node");

	RootNode->setLocalOffset(translation);
	RootNode->setLocalRotation(rotation);

	BOOST_TEST((glm::vec3(RootNode->getWorldTransform()[3]) == translation), "Invalid world position");
	BOOST_TEST((RootNode->getLocalOffset() == translation), "Invalid local position");
	BOOST_TEST((glm::toQuat(RootNode->getWorldTransform()) == rotation), "Invalid world rotation");
	BOOST_TEST((RootNode->getLocalRotation() == rotation), "Invalid local rotation");

	RootNode->resetTransform();

	BOOST_TEST((RootNode->getLocalTransform() == glm::mat4(1.0f)), "Root node with non-identity local transform");
	BOOST_TEST((RootNode->getWorldTransform() == glm::mat4(1.0f)), "Root node with non-identity world transform");

	RootNode->setWorldTransform(testTransform);

	glm::mat4 m = RootNode->getWorldTransform();
	BOOST_TEST((mat4Compare(m, testTransform, 1e-5)), "Root node with invalid world transform");

	RootNode->resetTransform();

	RootNode->setLocalTransform(testTransform);

	m = RootNode->getLocalTransform();
	BOOST_TEST((mat4Compare(m, testTransform, 1e-5)), "Root node with invalid local transform");
}

BOOST_AUTO_TEST_CASE(NodeTransformPropagation)
{
	auto Scene = makeScene();
	auto RootNode = Scene->getRootNode();
	glm::vec3 translation(1.0f, 0, 0);
	glm::quat rotation = glm::angleAxis(kitsune::scenegraph::util::PI_2, glm::vec3(0, 1.0f, 0));
	glm::mat4 testTransform = glm::translate(glm::mat4(1.0f), translation) * glm::toMat4(rotation);

	BOOST_TEST((RootNode != nullptr), "Scene without root node");

	auto ChildNode = RootNode->addChildNode();
	auto SecondChild = ChildNode->addChildNode();

	ChildNode->setLocalTransform(testTransform);

	BOOST_TEST((glm::vec3(SecondChild->getWorldTransform()[3]) == translation), "Invalid world position");
	glm::quat worldRotation = glm::toQuat(SecondChild->getWorldTransform());
	BOOST_TEST((fabsf(rotation.x - worldRotation.x) <= 1e-5f), "Invalid world rotation");
	BOOST_TEST((fabsf(rotation.y - worldRotation.y) <= 1e-5f), "Invalid world rotation");
	BOOST_TEST((fabsf(rotation.z - worldRotation.z) <= 1e-5f), "Invalid world rotation");
	BOOST_TEST((fabsf(rotation.w - worldRotation.w) <= 1e-5f), "Invalid world rotation");

	BOOST_TEST((SecondChild->getLocalTransform() == glm::mat4(1.0f)), "Invalid local transform");

	glm::mat4 testInverse = glm::affineInverse(testTransform);
	glm::mat4 toOrigin = SecondChild->getTransformToOrigin();
	BOOST_TEST((mat4Compare(toOrigin, testInverse, 1e-5)), "Invalid transform to origin");

	ChildNode->resetTransform();

	BOOST_TEST((SecondChild->getWorldTransform() == glm::mat4(1.0f)), "Invalid world rotation");

	BOOST_TEST((SecondChild->getTransformToOrigin() == glm::mat4(1.0f)), "Invalid transform to origin");
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
