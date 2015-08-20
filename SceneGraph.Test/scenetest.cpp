#include "stdafx.h"
#include "CppUnitTest.h"

#include "../SceneGraph/Component.h"
#include "../SceneGraph/Node.h"
#include "../SceneGraph/Scene.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace sg = kitsune::scenegraph;

namespace SceneGraphTest
{

	TEST_CLASS(SceneTest)
	{
	public:
		TEST_METHOD(SceneUpdateEvent)
		{
			bool updated = false;
			std::shared_ptr<sg::Scene> Scene(new sg::Scene);
			Scene->initialize();
			auto updateEvent = Scene->addUpdateEvent([&updated](float DeltaTime) {
				Assert::AreEqual(100.0f, DeltaTime, 0.0f, L"Invalid DeltaTime");
				updated = true;
			});

			Scene->update(100.0f);

			Assert::IsTrue(updated, L"Update event was not called");
		}
		
		TEST_METHOD(ScenePreUpdateEvent)
		{
			bool updated = false;
			std::shared_ptr<sg::Scene> Scene(new sg::Scene);
			Scene->initialize();
			auto preUpdateEvent = Scene->addPreUpdateEvent([&updated](float DeltaTime) {
				Assert::AreEqual(100.0f, DeltaTime, 0.0f, L"Invalid DeltaTime");
				updated = true;
			});

			Scene->update(100.0f);

			Assert::IsTrue(updated, L"Pre-update event was not called");
		}

		TEST_METHOD(SceneUpdateOrder)
		{
			unsigned int updated = 0;
			std::shared_ptr<sg::Scene> Scene(new sg::Scene);
			Scene->initialize();
			auto preUpdateEvent = Scene->addPreUpdateEvent([&updated](float DeltaTime) {
				Assert::AreEqual(100.0f, DeltaTime, 0.0f, L"Invalid DeltaTime");
				updated = 1;
			});
			auto updateEvent = Scene->addUpdateEvent([&updated](float DeltaTime) {
				Assert::AreEqual(100.0f, DeltaTime, 0.0f, L"Invalid DeltaTime");
				Assert::IsTrue(updated == 1, L"Invalid update order (update ran before pre-update)");
				updated = 2;
			});


			Scene->update(100.0f);

			Assert::IsTrue(updated == 2, L"Update event was not called");
		}

		TEST_METHOD(SceneHasRootNode)
		{
			std::shared_ptr<sg::Scene> Scene(new sg::Scene);
			Scene->initialize();
			
			Assert::IsNotNull(Scene->getRootNode(), L"Scene without root node");
		}
	};

}
