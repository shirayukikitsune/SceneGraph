#pragma once

#include "Buffer.h"
#include "Shader.h"
#include "Vertex.h"
#include "Base/Component.h"

#include <memory>
#include <type_traits>
#include <glad/glad.h>

namespace kitsune {
namespace scenegraph {
namespace sdl {
namespace graphics {

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
            if (shader) {
                shader->useProgram();
            } else {
                // Ensure no shader in this case
                glUseProgram(0);
            }

            glBindVertexArray(vertexArray);
            glDrawElements(GL_TRIANGLES, indexBuffer->get().size(), getDrawElementType(), nullptr);
        }

        void initialize() override {
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
        
    private:
        IndexBufferType indexBuffer;
        VertexBufferType vertexBuffer;

        GLuint vertexArray;

        Shader<VertexType>* shader;
    };

}
}
}
}
