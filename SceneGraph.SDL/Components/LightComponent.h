#pragma once

#include <Base/Component.h>

#include <glm/glm.hpp>

namespace kitsune::scenegraph::sdl::components {

class Light : public Component {
    KIT_SG_COMPONENT(kitsune::scenegraph::sdl::components::Light);

    glm::vec3 ambientColor;
    glm::vec3 diffuseColor;
    glm::vec3 specularColor;
    glm::vec3 direction;

public:
    Light()
        : ambientColor(1.0f, 1.0f, 1.0f),
          diffuseColor(1.0f, 1.0f, 1.0f),
          specularColor(1.0f, 1.0f, 1.0f),
          direction(0.0f, 0.0f, 0.0f)
    {}

    void setDirection(const glm::vec3 &direction)
    {
        this->direction = direction;
    }

    const glm::vec3& getDirection() const
    {
        return this->direction;
    }

    void setAmbientColor(const glm::vec3 &color)
    {
        this->ambientColor = color;
    }

    const glm::vec3& getAmbientColor()
    {
        return this->ambientColor;
    }

    void setDiffuseColor(const glm::vec3 &color)
    {
        this->diffuseColor = color;
    }

    const glm::vec3& getDiffuseColor()
    {
        return this->diffuseColor;
    }

    void setSpecularColor(const glm::vec3 &color)
    {
        this->specularColor = color;
    }

    const glm::vec3& getSpecularColor()
    {
        return this->specularColor;
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
