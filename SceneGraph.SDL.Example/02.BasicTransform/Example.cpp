#include "Example.h"

#include <Base/Node.h>
#include <Base/Scene.h>
#include <Components/CameraComponent.h>
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

namespace kitsune::scenegraph::sdl {
    std::unique_ptr<Bootstrap> GetApplicationInstance() {
        return std::make_unique<ExampleApplication>();
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
        auto evData = reinterpret_cast<SDL_KeyboardEvent*>(data);
        if (evData->keysym.sym == SDLK_ESCAPE) {
            this->Run = false;
        }
    });

    // Setup the basic scene, with physics support
    std::shared_ptr<sg::Scene> Scene(new sg::Scene);

    Scene->initialize();

    // Add a camera to the scene
    Camera = Scene->getRootNode()->addChildNode();
    Camera->setLocalOffset(glm::vec3(5.0f, 0, -5.0f));
    Camera->setName("Camera");
    Camera->setTag("Camera");
    Scene->setActiveCamera(Camera);
    auto CameraComponent = Camera->createComponent<sg::sdl::components::Camera>(45.0f, AspectRatio, 0.5f, 1000.0f);
    CameraComponent->setLookDirection(glm::vec3(-5.0f , 0, 5.0f));

    // Add a cube to the scene
    auto Node = Scene->getRootNode()->addChildNode();
    Node->setName("Cube");
    Node->setTag("Cube");
    // Scale the cube by 2.0
    sg::sdl::prefabs::CreateCube(Node, false);

    auto Shader = Node->createComponent<sdlg::Shader<sdlg::vertex::PositionNormalUVVertex>>();
    Shader->append("example02.vert", sdlg::ShaderType::Vertex);
    Shader->append("example02.frag", sdlg::ShaderType::Fragment);
    Shader->registerEvents();
    Shader->linkShaders();

    Node = Scene->getRootNode()->addChildNode();
    Node->setName("Moon");
    Node->setTag("Cube");
    // Add a new cube as a child of the first cube
    Node = Node->addChildNode();
    Node->setName("Cube 2");
    Node->setTag("Cube");
    sg::sdl::prefabs::CreateCube(Node, false);
    Node->setLocalOffset(glm::vec3(0.0f, 2.0f, 0.0f));
    Node->setLocalScale(glm::vec3(0.25f));

    Shader = Node->createComponent<sdlg::Shader<sdlg::vertex::PositionNormalUVVertex>>();
    Shader->append("example02.vert", sdlg::ShaderType::Vertex);
    Shader->append("example02.frag", sdlg::ShaderType::Fragment);
    Shader->registerEvents();
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

    // We rotate the cubes a bit every frame
    auto Nodes = CurrentScene->findNodesByTag("Cube");
    for (auto i = Nodes.first; i != Nodes.second; ++i) {
        if (auto Node = i->second.lock()) {
            auto step = Node->getName() == "Cube" ? 0.01f : Node->getName() == "Moon" ? 0.02f : 0.002f;
            auto rotation = Node->getLocalRotation();
            auto angles = glm::eulerAngles(rotation);
            angles.z += step;
            rotation = glm::quat(angles);
            Node->setLocalRotation(rotation);
        }
    }

    // With the first cube local rotation, the second cube will orbit around the first while also rotating itself

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

    AspectRatio = (float)width / (float)height;
}
