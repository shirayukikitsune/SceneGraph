#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE kitsune::scenegraph::PhysicsScene
#include <boost/test/unit_test.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Base/Node.h"
#include "Base/PhysicsScene.h"
#include "Components/CollisionShapeComponent.h"
#include "Components/RigidBodyComponent.h"

namespace sg = kitsune::scenegraph;
namespace utf = boost::unit_test;

std::shared_ptr<sg::PhysicsScene> makeScene() {
	std::shared_ptr<sg::PhysicsScene> Scene(new sg::PhysicsScene);

	Scene->setGravity(btVector3(0, -10.0f, 0));
	Scene->setFps(1200.0f);

	Scene->initialize();

	return Scene;
}

BOOST_AUTO_TEST_SUITE(PhysicsScene)

BOOST_AUTO_TEST_CASE(PhysicsFreeFall, * utf::tolerance(0.005f))
{
	auto Scene = makeScene();

	auto Node = Scene->getRootNode()->addChildNode();
	Node->setLocalOffset(glm::vec3(0, 5.0f, 0));

	auto CollisionShape = Node->createComponent<sg::CollisionShapeComponent>();
	CollisionShape->setDimensions(btVector3(1, 1, 1));
	CollisionShape->setShape(sg::CollisionShapeComponent::ShapeFormat::Box);

	auto RigidBody = Node->createComponent<sg::RigidBodyComponent>(1.0f, sg::RigidBodyComponent::RigidBodyType::Dynamic);

	for (int i = 0; i < 1200; ++i)
		Scene->update(1.0f / 1200.0f);

	BOOST_TEST(RigidBody->getLinearVelocity().x() == 0.0f, "X velocity is zero (" << RigidBody->getLinearVelocity().x() << ")");
	BOOST_TEST(RigidBody->getLinearVelocity().y() == -10.0f, "Y velocity is -10 (" << RigidBody->getLinearVelocity().y() << ")");
	BOOST_TEST(RigidBody->getLinearVelocity().z() == 0.0f, "Z velocity is zero (" << RigidBody->getLinearVelocity().z() << ")");

	glm::vec3 translation(Node->getWorldTransform()[3]);
	BOOST_TEST(translation.x == 0.0f, "X position is zero (" << translation.x << ")");
	BOOST_TEST(translation.y == 0.0f, "Y position is zero (" << translation.y << ")");
	BOOST_TEST(translation.z == 0.0f, "Z position is zero (" << translation.z << ")");
}

BOOST_AUTO_TEST_CASE(PhysicsCollision, * utf::tolerance(0.005f))
{
	auto Scene = makeScene();

	auto PlaneNode = Scene->getRootNode()->addChildNode();
	PlaneNode->setWorldTransform(glm::mat4(1.0f));
	auto CollisionShape = PlaneNode->createComponent<sg::CollisionShapeComponent>();
	CollisionShape->setDimensions(btVector3(0, 1, 0)); // +y = up
	CollisionShape->setPlaneConstant(0.0f);
	CollisionShape->setShape(sg::CollisionShapeComponent::ShapeFormat::Plane);
	auto RigidBody = PlaneNode->createComponent<sg::RigidBodyComponent>(0.0f, sg::RigidBodyComponent::RigidBodyType::Static);

	auto BoxNode = Scene->getRootNode()->addChildNode();
	BoxNode->setWorldTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0, 50.0f, 0)));
	CollisionShape = BoxNode->createComponent<sg::CollisionShapeComponent>();
	CollisionShape->setDimensions(btVector3(1, 1, 1)); // x=-1:1; y=49:51; z=-1:1
	CollisionShape->setShape(sg::CollisionShapeComponent::ShapeFormat::Box);
	RigidBody = BoxNode->createComponent<sg::RigidBodyComponent>(1.0f, sg::RigidBodyComponent::RigidBodyType::Dynamic);
	
	for (int i = 0; i < 6000; ++i)
		Scene->update(1.0f / 1200.0f);

	// The box should have hit the plane with its lower face (box's y should go from 0 to 2), so the center of mass position is 1
	glm::vec3 translation(BoxNode->getWorldTransform()[3]);
	BOOST_TEST(translation.y == 1.0f, "Y position is not 1 (" << translation.y << ")");
}

BOOST_AUTO_TEST_SUITE_END()
