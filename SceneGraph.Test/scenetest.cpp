#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE kitsune::scenegraph::Scene
#include <boost/test/unit_test.hpp>

#include "Base/Component.h"
#include "Base/Node.h"
#include "Base/Scene.h"

namespace sg = kitsune::scenegraph;
namespace utf = boost::unit_test;

BOOST_AUTO_TEST_SUITE(Component)

BOOST_AUTO_TEST_CASE(SceneUpdateEvent, * utf::tolerance(0.0001))
{
	bool updated = false;
	std::shared_ptr<sg::Scene> Scene(new sg::Scene);
	Scene->initialize();
	auto updateEvent = Scene->addUpdateEvent([&updated](float DeltaTime) {
		BOOST_TEST(DeltaTime == 100.0f, "Invalid DeltaTime");
		updated = true;
	});

	Scene->update(100.0f);

	BOOST_TEST(updated, "Update event was not called");
}

BOOST_AUTO_TEST_CASE(ScenePreUpdateEvent, * utf::tolerance(0.0001))
{
	bool updated = false;
	std::shared_ptr<sg::Scene> Scene(new sg::Scene);
	Scene->initialize();
	auto preUpdateEvent = Scene->addPreUpdateEvent([&updated](float DeltaTime) {
		BOOST_TEST(DeltaTime == 100.0f, "Invalid DeltaTime");
		updated = true;
	});

	Scene->update(100.0f);

	BOOST_TEST(updated, "Pre-update event was not called");
}

BOOST_AUTO_TEST_CASE(SceneUpdateOrder, * utf::tolerance(0.0001))
{
	unsigned int updated = 0;
	std::shared_ptr<sg::Scene> Scene(new sg::Scene);
	Scene->initialize();
	auto preUpdateEvent = Scene->addPreUpdateEvent([&updated](float DeltaTime) {
		BOOST_TEST(DeltaTime == 100.0f, "Invalid DeltaTime");
		updated = 1;
	});
	auto updateEvent = Scene->addUpdateEvent([&updated](float DeltaTime) {
		BOOST_TEST(DeltaTime == 100.0f, "Invalid DeltaTime");
		BOOST_TEST(updated == 1, "Invalid update order (update ran before pre-update)");
		updated = 2;
	});


	Scene->update(100.0f);

	BOOST_TEST(updated == 2, "Update event was not called");
}

BOOST_AUTO_TEST_CASE(SceneHasRootNode)
{
	std::shared_ptr<sg::Scene> Scene(new sg::Scene);
	Scene->initialize();
	
	BOOST_TEST((Scene->getRootNode() != nullptr), "Scene without root node");
}

BOOST_AUTO_TEST_SUITE_END()
