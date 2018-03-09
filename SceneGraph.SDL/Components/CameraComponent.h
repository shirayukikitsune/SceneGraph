#pragma once

#include <Base/Component.h>

#include <glm/glm.hpp>

namespace kitsune {
namespace scenegraph {
namespace sdl {
namespace components {

class Camera : public Component {
    KIT_SG_COMPONENT(kitsune::scenegraph::sdl::components::Camera);

    glm::mat4 projection;
    glm::vec3 lookDirection;

public:
    Camera(float fov, float aspectRatio, float near, float far);

    void setLookDirection(glm::vec3 direction);

    glm::mat4 getView();
    glm::mat4 getProjection() { return projection; };
};

}
}
}
}