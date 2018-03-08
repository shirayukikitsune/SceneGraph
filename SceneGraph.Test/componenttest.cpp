#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE kitsune::scenegraph::Component
#include <boost/test/unit_test.hpp>

#include "Base/Component.h"
#include "Base/Node.h"
#include "Components/SceneEventComponent.h"

namespace sg = kitsune::scenegraph;

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

	virtual void onPreUpdate(float DeltaTime) override {
		updated = 1;

		sg::SceneEventComponent::onPreUpdate(DeltaTime);
	}

	virtual void onPostUpdate(float DeltaTime) override {
		updated = 2;

		sg::SceneEventComponent::onPostUpdate(DeltaTime);
	}

	virtual uint32_t getAttachedEvents() override { return (uint32_t)AttachedEvents::PreUpdate | (uint32_t)AttachedEvents::PostUpdate; }
};

BOOST_AUTO_TEST_SUITE(Component)

BOOST_AUTO_TEST_CASE(ComponentIsActive)
{
	sg::Component *Component = new sg::Component();

	BOOST_TEST(Component->isLocalActive(), "Component is not locally active");
	BOOST_TEST(Component->isActive(), "Component is not active");

	Component->setActive(false);

	BOOST_TEST(!Component->isLocalActive(), "Component is locally active");
	BOOST_TEST(!Component->isActive(), "Component is active");
}

BOOST_AUTO_TEST_CASE(NodeComponentIsActive)
{
	std::shared_ptr<sg::Node> Node(new sg::Node());
	auto Component = Node->createComponent<sg::Component>();

	BOOST_TEST((Component != nullptr), "Failed to create component");

	BOOST_TEST(Node->isActive(), "Node is not active");
	BOOST_TEST(Component->isActive(), "Component is not active");

	Node->setActive(false);

	BOOST_TEST(!Component->isActive(), "Component is active");
	BOOST_TEST(Component->isLocalActive(), "Component is not locally active");

	Node->setActive(true);
	Component->setActive(false);

	BOOST_TEST(!Component->isActive(), "Component is active");
	BOOST_TEST(!Component->isLocalActive(), "Component is locally active");
}

BOOST_AUTO_TEST_CASE(NodeHasComponent)
{
	std::shared_ptr<sg::Scene> Scene(new sg::Scene);
	Scene->initialize();

	auto Node = Scene->getRootNode()->addChildNode();
	sg::Component *Component = new sg::Component();

	Node->addComponent(Component);

	BOOST_TEST(Node->hasComponent<sg::Component>(), "Component not found");
}

BOOST_AUTO_TEST_CASE(NodeDoesNotHaveComponent)
{
	std::shared_ptr<sg::Scene> Scene(new sg::Scene);
	Scene->initialize();

	auto Node = Scene->getRootNode()->addChildNode();
	sg::Component *Component = new sg::Component();

	Node->addComponent(Component);

	BOOST_TEST(!Node->hasComponent<InvalidComponent>(), "Component found");
}

BOOST_AUTO_TEST_CASE(NodeGetComponent)
{
	std::shared_ptr<sg::Scene> Scene(new sg::Scene);
	Scene->initialize();

	auto Node = Scene->getRootNode()->addChildNode();
	sg::Component *Component = new sg::Component();

	Node->addComponent(Component);

	BOOST_TEST((Node->getComponent<sg::Component>() != nullptr), "Component not found");
}

BOOST_AUTO_TEST_CASE(ComponentHasNodeAndScene)
{
	sg::Component *Component = new sg::Component();

	BOOST_TEST((Component->getScene() == nullptr), "Component belongs to a scene");
	BOOST_TEST((Component->getNode() == nullptr), "Component belongs to a node");

	delete Component;

	std::shared_ptr<sg::Scene> Scene(new sg::Scene);
	Scene->initialize();

	auto Node = Scene->getRootNode();
	Component = Node->createComponent<sg::Component>();

	BOOST_TEST((Component->getScene() != nullptr), "Component doesn't belongs to a scene");
	BOOST_TEST((Component->getNode() != nullptr), "Component doesn't belongs to a node");
}

BOOST_AUTO_TEST_CASE(NodeGetInvalidComponent)
{
	std::shared_ptr<sg::Scene> Scene(new sg::Scene);
	Scene->initialize();

	auto Node = Scene->getRootNode()->addChildNode();
	sg::Component *Component = new sg::Component();

	Node->addComponent(Component);

	BOOST_TEST((Node->getComponent<InvalidComponent>() == nullptr), "Component found");
}

BOOST_AUTO_TEST_CASE(SceneEventComponent)
{
	std::shared_ptr<sg::Scene> Scene(new sg::Scene);
	Scene->initialize();

	auto Node = Scene->getRootNode()->addChildNode();
	auto Component = Node->createComponent<TestEventComponent>();

	Component->registerEvents();

	Scene->update(100.0f);

	BOOST_TEST(Component->updated == 2, "Update events not called");
}

BOOST_AUTO_TEST_SUITE_END()
