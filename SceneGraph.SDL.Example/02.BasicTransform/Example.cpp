#include "Example.h"

#include <Base/Node.h>
#include <Base/Scene.h>
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
    _Application = Application;
    _Application->setWindowTitle("Scenegraph Example 02 - Basic Transform");
}

void ExampleApplication::onInitialized()
{
    _Application->getGraphics()->setClearColor(1.0f, 0, 0, 0);
    _Application->getGraphics()->setVerticalSync(true);

    // All of these will make the application gracefully end:
    //  - Send a SIGTERM
    //  - Close the main window
    //  - Press the ESCAPE key
    _Application->getEventHandler()->addHandler(sg::events::application::Quit, [this](void*) {
        this->Run = false;
    });
    _Application->getEventHandler()->addHandler(sg::events::window::Close, [this](void*) {
        this->Run = false;
    });
    _Application->getEventHandler()->addHandler(sg::events::input::KeyUp, [this](void* data) {
        SDL_KeyboardEvent *evData = reinterpret_cast<SDL_KeyboardEvent*>(data);
        if (evData->keysym.sym == SDLK_ESCAPE) {
            this->Run = false;
        }
    });

    // Setup the basic scene, with physics support
    std::shared_ptr<sg::Scene> Scene(new sg::Scene);

    Scene->initialize();

    // Add a cube to the scene
    auto Node = Scene->getRootNode()->addChildNode();
    Node->setName("Cube");
    Node->setTag("Cube");
    // Scale the cube by 2.0
    auto transform = Node->getLocalTransform();
    transform.setBasis(transform.getBasis().scaled(btVector3(2.0f, 2.0f, 2.0f)));
    Node->setLocalTransform(transform);
    sg::sdl::prefabs::CreateCube(Node, false);

    // Add a new cube as a child of the first cube
    Node = Node->addChildNode();
    Node->setName("Cube 2");
    Node->setTag("Cube");
    sg::sdl::prefabs::CreateCube(Node, false);

    auto Shader = Node->createComponent<sdlg::Shader<sdlg::vertex::PositionNormalUVVertex>>();
    Shader->append("example02.vert", sdlg::ShaderType::Vertex);
    Shader->append("example02.frag", sdlg::ShaderType::Fragment);
    Shader->linkShaders();

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

    // We rotate the cubes a bit every frame
    auto Nodes = CurrentScene->findNodesByTag("Cube");
    for (auto i = Nodes.first; i != Nodes.second; ++i) {
        if (auto Node = i->second.lock()) {
            auto step = Node->getName().compare("Cube") == 0 ? 0.03f : 0.015f;
            btQuaternion rotation = Node->getLocalRotation();
            float X, Y, Z;
            rotation.getEulerZYX(Z, Y, X);
            Z = -SIMD_PI / 18.0f;
            Y += step;
            rotation.setEuler(Y, X, Z);
            Node->setLocalRotation(rotation);
        }
    }

    // With the first cube local rotation, the second cube will orbit around the first while also rotating itself

    if (f < 500) {
        a = sg::util::Easing<sg::util::EasingFunction::CubicIn>()(f / 500.0f);
    } else if (f < 1000) {
        a = sg::util::Easing<sg::util::EasingFunction::CubicOut>()((1000 - f) / 500.0f);
    }

    _Application->getGraphics()->setClearColor(1.0f, a, a, a);

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
