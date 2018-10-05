#pragma once

#include <Base/Component.h>

#include <glm/glm.hpp>

namespace kitsune::scenegraph::sdl::components {

class Light : public Component {
    KIT_SG_COMPONENT(kitsune::scenegraph::sdl::components::Light);

    glm::vec3 color;
    glm::vec3 direction;

public:
    Light()
        : color(1.0f), direction(0.0f)
    {}

    void setDirection(const glm::vec3 &direction)
    {
        this->direction = direction;
    }

    const glm::vec3& getDirection() const
    {
        return this->direction;
    }

    void setColor(const glm::vec3 &color)
    {
        this->color = color;
    }

    const glm::vec3& getColor()
    {
        return this->color;
    }

protected:
    void onNodeSet() override
    {
        auto node = this->getNode();

        if (node) {
            node->setTag("__light");
        }
    }
};

}
