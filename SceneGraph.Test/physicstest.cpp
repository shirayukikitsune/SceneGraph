#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE kitsune::scenegraph::PhysicsScene
#include <boost/test/unit_test.hpp>

#include "Base/Node.h"
#include "Base/PhysicsScene.h"
#include "Components/CollisionShapeComponent.h"
#include "Components/RigidBodyComponent.h"

namespace sg = kitsune::scenegraph;
namespace utf = boost::unit_test;

std::shared_ptr<sg::PhysicsScene> makeScene() {
	std::shared_ptr<sg::PhysicsScene> Scene(new sg::PhysicsScene);
	Scene->initialize();

	return Scene;
}

BOOST_AUTO_TEST_SUITE(PhysicsScene)

BOOST_AUTO_TEST_CASE(PhysicsUpdateEvent, * utf::tolerance(0.0001))
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

	BOOST_TEST(RigidBody->getLinearVelocity().x() == 0.0f, "X velocity is not zero");
	BOOST_TEST(RigidBody->getLinearVelocity().y() == -10.0f, "Unexpected Y velocity");
	BOOST_TEST(RigidBody->getLinearVelocity().z() == 0.0f, "Z velocity is not zero");

	BOOST_TEST(Node->getWorldTransform().getOrigin().x() == 0.0f, "X position is not zero");
	BOOST_TEST(Node->getWorldTransform().getOrigin().y() == -5.0f, "Unexpected Y position");
	BOOST_TEST(Node->getWorldTransform().getOrigin().z() == 0.0f, "Z position is not zero");
}

BOOST_AUTO_TEST_CASE(PhysicsCollision, * utf::tolerance(0.0001))
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

	BOOST_TEST(BoxNode->getWorldTransform().getOrigin().y() == 1.0f, "Unexpected Y position");
}

BOOST_AUTO_TEST_SUITE_END()
