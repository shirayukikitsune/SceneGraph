#include "CameraComponent.h"

#include <Base/Node.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using kitsune::scenegraph::sdl::components::Camera;

Camera::Camera(float fov, float aspectRatio, float near, float far) {
    projection = glm::perspective(glm::radians(fov), aspectRatio, near, far);
    lookDirection = glm::vec3(0, 1, 0);
}

void Camera::setLookDirection(btVector3 direction) {
    lookDirection = glm::vec3(direction.x(), direction.y(), direction.z());
}

glm::mat4 Camera::getView() {
    glm::mat4 view;
    auto node = getNode();
    auto transform = node->getWorldTransform();

    glm::vec3 currentPosition(transform.getOrigin().x(), transform.getOrigin().y(), transform.getOrigin().z());
    glm::vec3 nextPosition = currentPosition + lookDirection;

    return glm::lookAt(currentPosition, nextPosition, glm::vec3(0, 1, 0));
}
