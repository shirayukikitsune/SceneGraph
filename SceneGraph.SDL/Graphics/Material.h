#pragma once

#include "Buffer.h"
#include "Shader.h"
#include "Vertex.h"
#include "Base/Component.h"

#include <memory>
#include <type_traits>
#include <glad/glad.h>

namespace kitsune::scenegraph::sdl::graphics {

    template <class VertexType, class IndexType = unsigned int>
    class Material : public Component
    {
        KIT_SG_COMPONENT(kitsune::scenegraph::sdl::graphics::Material)
        static_assert(std::is_base_of<vertex::BaseVertex, VertexType>::value, "VertexType must inherit from BaseVertex");

        template <class A = IndexType>
        constexpr typename std::enable_if<std::is_same<A, unsigned int>::value, GLenum>::type getDrawElementType() {
            return GL_UNSIGNED_INT;
        }

        template <class A = IndexType>
        constexpr typename std::enable_if<std::is_same<A, unsigned short>::value, GLenum>::type getDrawElementType() {
            return GL_UNSIGNED_SHORT;
        }

        template <class A = IndexType>
        constexpr typename std::enable_if<std::is_same<A, unsigned char>::value, GLenum>::type getDrawElementType() {
            return GL_UNSIGNED_BYTE;
        }

    public:
        typedef std::unique_ptr<ArrayBuffer<IndexType, 1>> IndexBufferType;
        typedef std::unique_ptr<ArrayBuffer<VertexType, 1>> VertexBufferType;

        Material() : ambientColor(1.0f), diffuseColor(1.0f), specularColor(1.0f) {
            shininess = 1.0f;
        }

        void setIndexBuffer(IndexBufferType && buffer) {
            indexBuffer = std::move(buffer);
        }

        void setVertexBuffer(VertexBufferType && buffer) {
            vertexBuffer = std::move(buffer);
        }

        void beginSetup() {
            glGenVertexArrays(1, &vertexArray);
            glBindVertexArray(vertexArray);
        }

        void endSetup() {
            VertexType::bindAttributes();
        }

        void render() override {
            if (!shader) {
                // Don't render if there is no shader attached
                return;
            }
            shader->useProgram();

            glBindVertexArray(vertexArray);
            glDrawElements(GL_TRIANGLES, (GLsizei)indexBuffer->get().size(), getDrawElementType(), nullptr);
        }

        void initialize() override {
            // This allow hotswapping shaders
            shader = getNode()->template getComponent<Shader<VertexType>>();
            getNode()->addComponentAddedEvent([this](Component *component) {
                if (component->getComponentNameHash() == Shader<VertexType>::componentNameHash) {
                    this->shader = reinterpret_cast<Shader<VertexType>*>(component);
                }
            });
            getNode()->addComponentRemovedEvent([this](Component *component) {
                if (component->getComponentNameHash() == Shader<VertexType>::componentNameHash) {
                    this->shader = nullptr;
                }
            });
        }

        void setAmbientColor(glm::vec4 color) { ambientColor = color; }
        glm::vec4 getAmbientColor() { return ambientColor; }

        void setDiffuseColor(glm::vec4 color) { diffuseColor = color; }
        glm::vec4 getDiffuseColor() { return diffuseColor; }

        void setSpecularColor(glm::vec4 color) { specularColor = color; }
        glm::vec4 getSpecularColor() { return specularColor; }

        void setShininess(float value) { shininess = value; }
        float getShininess() { return shininess; }

    private:
        IndexBufferType indexBuffer;
        VertexBufferType vertexBuffer;

        GLuint vertexArray;

        Shader<VertexType>* shader;

        glm::vec4 ambientColor;
        glm::vec4 diffuseColor;
        glm::vec4 specularColor;
        float shininess;
    };

}
