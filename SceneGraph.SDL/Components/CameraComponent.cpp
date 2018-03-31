#include "CameraComponent.h"

#include <Base/Node.h>
#include <Base/Scene.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using kitsune::scenegraph::sdl::components::Camera;

Camera::Camera(float fov, float aspectRatio, float near, float far) {
    projection = glm::perspectiveRH(glm::radians(fov), aspectRatio, near, far);
    lookDirection = glm::vec3(0, 1, 0);
}

void Camera::setLookDirection(glm::vec3 direction) {
    lookDirection = direction;
}

glm::mat4 Camera::getView() {
    glm::mat4 view;
    auto node = getNode();
    auto transform = node->getWorldTransform();

    glm::vec3 currentPosition(transform[3]);
    glm::vec3 nextPosition = currentPosition + lookDirection;

    return glm::lookAtRH(currentPosition, nextPosition, glm::vec3(0, 1, 0));
}

void Camera::onNodeSet() {
    auto Scene = this->getScene();

    if (!Scene) {
        return;
    }

    if (!Scene->getActiveCamera()) {
        Scene->setActiveCamera(getNode());
    }
}
