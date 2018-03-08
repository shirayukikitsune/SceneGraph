#include "Example.h"

#include <Base/Node.h>
#include <Base/PhysicsScene.h>
#include <Components/CollisionShapeComponent.h>
#include <Components/RigidBodyComponent.h>
#include <Util/Easing.h>

#include <Graphics/Buffer.h>
#include <Graphics/Material.h>
#include <Graphics/Shader.h>
#include <Prefabs/Prefabs.h>

using kitsune::scenegraph::sdl::Application;
using kitsune::scenegraph::sdl::ExampleApplication;
namespace sg = kitsune::scenegraph;
namespace sdlg = kitsune::scenegraph::sdl::graphics;

namespace kitsune {
namespace scenegraph {
namespace sdl {
kitsune::scenegraph::sdl::Bootstrap * AppBootstrap = new ExampleApplication;
}
}
}

ExampleApplication::ExampleApplication()
{
    Run = true;
    currentTime = 0;
}

void ExampleApplication::onInitializing(sg::sdl::Application * Application)
{
    this->Application = Application;
    Application->setWindowTitle("Scenegraph Example");
}

void ExampleApplication::onInitialized()
{
    Application->getGraphics()->setClearColor(1.0f, 0, 0, 0);
    Application->getGraphics()->setVerticalSync(true);

    // All of these will make the application gracefully end:
    //  - Send a SIGTERM
    //  - Close the main window
    //  - Press the ESCAPE key
    Application->getEventHandler()->addHandler(sg::events::application::Quit, [this](void*) {
        this->Run = false;
    });
    Application->getEventHandler()->addHandler(sg::events::window::Close, [this](void*) {
        this->Run = false;
    });
    Application->getEventHandler()->addHandler(sg::events::input::KeyUp, [this](void* data) {
        SDL_KeyboardEvent *evData = reinterpret_cast<SDL_KeyboardEvent*>(data);
        if (evData->keysym.sym == SDLK_ESCAPE) {
            this->Run = false;
        }
    });

    // Setup the basic scene, with physics support
    std::shared_ptr<sg::PhysicsScene> Scene(new sg::PhysicsScene);

	Scene->setGravity(btVector3(0, -10.0f, 0));
	Scene->setFps(60.0f);
    Scene->initialize();

    // Add a plane to the scene
    auto Node = Scene->getRootNode()->addChildNode();
    Node->setName("Plane");
    sg::sdl::prefabs::CreatePlane(Node);

    auto Shader = Node->createComponent<sdlg::Shader<sdlg::vertex::PositionNormalUVVertex>>();
    Shader->append("example01.vert", sdlg::ShaderType::Vertex);
    Shader->append("example01.frag", sdlg::ShaderType::Fragment);
    Shader->linkShaders();

    // Now add a box
    Node = Scene->getRootNode()->addChildNode();
    Node->setLocalOffset(btVector3(0, 5.0f, 0));

    auto CollisionShape = Node->createComponent<sg::CollisionShapeComponent>();
    CollisionShape->setDimentions(btVector3(1, 1, 1));
    CollisionShape->setShape(sg::CollisionShapeComponent::ShapeFormat::Box);

    auto RigidBody = Node->createComponent<sg::RigidBodyComponent>(1.0f, sg::RigidBodyComponent::RigidBodyType::Dynamic);
    RigidBody->setRestitution(0.5f);

    CurrentScene = Scene;
}

void ExampleApplication::onTerminating()
{
    CurrentScene.reset();
}

void ExampleApplication::onUpdate(const std::chrono::milliseconds & frameTime)
{
    currentTime += frameTime.count();
    int f = (int)currentTime % 2000;
    float a = 0.0f;

    if (f < 500) {
        a = sg::util::Easing<sg::util::EasingFunction::CubicIn>()(f / 500.0f);
    } else if (f < 1000) {
        a = sg::util::Easing<sg::util::EasingFunction::CubicOut>()((1000 - f) / 500.0f);
    }

    Application->getGraphics()->setClearColor(1.0f, a, a, a);

    CurrentScene->update(frameTime.count());
}

void ExampleApplication::onRender()
{
    CurrentScene->render();
}

bool ExampleApplication::shouldRun()
{
    return Run;
}

int ExampleApplication::returnCode()
{
    return 0;
}

void ExampleApplication::getWindowDimensions(int &width, int &height)
{
    // Create a window covering all available desktop size on display 0.
    SDL_Rect bounds;
    if (SDL_GetDisplayUsableBounds(0, &bounds) == 0) {
        width = bounds.w;
        height = bounds.h;
    } else {
        width = 800;
        height = 600;
    }
}
