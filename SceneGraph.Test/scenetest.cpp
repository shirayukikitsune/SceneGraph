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
			std::unique_ptr<sg::Scene> Scene(new sg::Scene);
			sg::Scene::updateCallback::auto_remover_type updateEvent = Scene->addUpdateEvent([&updated](float DeltaTime) {
				Assert::AreEqual(100.0f, DeltaTime, 0.0f, L"Invalid DeltaTime");
				updated = true;
			});

			Scene->update(100.0f);

			Assert::IsTrue(updated, L"Update event not called");
		}
		
		TEST_METHOD(ScenePreUpdateEvent)
		{
			bool updated = false;
			std::unique_ptr<sg::Scene> Scene(new sg::Scene);
			sg::Scene::updateCallback::auto_remover_type updateEvent = Scene->addPreUpdateEvent([&updated](float DeltaTime) {
				Assert::AreEqual(100.0f, DeltaTime, 0.0f, L"Invalid DeltaTime");
				updated = true;
			});

			Scene->update(100.0f);

			Assert::IsTrue(updated, L"Pre-update event not called");
		}
	};

}
