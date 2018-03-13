#pragma once

#include "Material.h"
#include "Shader.h"

#include <array>
#include <SOIL.h>
#include <glm/gtc/matrix_inverse.hpp>

namespace kitsune {
namespace scenegraph {
namespace sdl {
namespace graphics {

template <class VertexType>
class DiffuseShader : public Shader<VertexType>
{
    // FIXME: using DiffuseShader here will prevent material from capturing this...
    KIT_SG_COMPONENT(kitsune::scenegraph::sdl::graphics::Shader)

public:
    enum struct TextureIndex {
        Ambient,
        Diffuse,
        SpecularColor,
        SpecularHighlight,
        Bump,
        Count
    };

    DiffuseShader()
    {
        std::memset(textures.data(), 0, sizeof(GLuint) * (int)TextureIndex::Count);
        std::memset(textureUniforms.data(), 0, sizeof(GLint) * (int)TextureIndex::Count);
        ambientColor = diffuseColor = specularColor = 0;
    }

    virtual void initialize() override
    {
        Shader<VertexType>::initialize();
    }

    virtual void cleanUp() override
    {
        Shader<VertexType>::cleanUp();

        glDeleteTextures((GLsizei)TextureIndex::Count, textures.data());
    }

    void loadTexture(TextureIndex where, const std::string &name)
    {
        textures[(int)where] = SOIL_load_OGL_texture(name.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
    }

    void onPreUpdate(float DeltaTime) override {
        auto node = getNode();
        auto model = node->getWorldTransform();

        auto camera = node->getScene()->getActiveCamera()->template getComponent<components::Camera>();

        modelViewMatrix = camera->getView() * model;
        projectionMatrix = camera->getProjection();
        normalMatrix = glm::inverseTranspose(glm::mat3(modelViewMatrix));
    }

protected:
    void setUniforms() override
    {
        for (int i = 0; i < (int)TextureIndex::Count; ++i) {
            glUniform1i(textureUniforms[i], i);

            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, textures[i]);
        }

        auto node = getNode();
        // We don't really care about index type here, so no problem if types differ
        Material<VertexType>* material = node->getComponent<Material<VertexType>>();

        glUniform4fv(ambientColor, 1, &material->getAmbientColor()[0]);
        glUniform4fv(diffuseColor, 1, &material->getDiffuseColor()[0]);
        glUniform4fv(specularColor, 1, &material->getSpecularColor()[0]);

        glUniformMatrix4fv(modelView, 1, GL_FALSE, &modelViewMatrix[0][0]);
        glUniformMatrix4fv(projection, 1, GL_FALSE, &projectionMatrix[0][0]);
        glUniformMatrix4fv(normal, 1, GL_FALSE, &normalMatrix[0][0]);
    }

    void getUniforms() override
    {
        textureUniforms[(int)TextureIndex::Ambient] = glGetUniformLocation(shaderProgram, "ambientTexture");
        textureUniforms[(int)TextureIndex::Diffuse] = glGetUniformLocation(shaderProgram, "diffuseTexture");
        textureUniforms[(int)TextureIndex::SpecularColor] = glGetUniformLocation(shaderProgram, "specularColorTexture");
        textureUniforms[(int)TextureIndex::SpecularHighlight] = glGetUniformLocation(shaderProgram, "specularHighlightTexture");
        textureUniforms[(int)TextureIndex::Bump] = glGetUniformLocation(shaderProgram, "bumpTexture");

        ambientColor = glGetUniformLocation(shaderProgram, "ambientColor");
        diffuseColor = glGetUniformLocation(shaderProgram, "diffuseColor");
        specularColor = glGetUniformLocation(shaderProgram, "specularColor");

        modelView = glGetUniformLocation(shaderProgram, "modelView");
        projection = glGetUniformLocation(shaderProgram, "projection");
        normal = glGetUniformLocation(shaderProgram, "normal");
    }

private:
    std::array<GLuint, (int)TextureIndex::Count> textures;
    std::array<GLint, (int)TextureIndex::Count> textureUniforms;

    // Material parameters
    GLint ambientColor;
    GLint diffuseColor;
    GLint specularColor;

    GLint modelView;
    GLint projection;
    GLint normal;

    glm::mat4 modelViewMatrix;
    glm::mat4 projectionMatrix;
    glm::mat4 normalMatrix;
};

}
}
}
}
