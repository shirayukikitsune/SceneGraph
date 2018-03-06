#pragma once

#include "Buffer.h"
#include "Vertex.h"
#include "Base/Component.h"

#include <memory>
#include <type_traits>

namespace kitsune {
namespace scenegraph {
namespace graphics {

    /*template <typename VertexType>
    class Material
        : public Component
    {
        static_assert(std::is_base_of<vertex::BaseVertex, VertexType>::value, "VertexType must inherit from BaseVertex");
        KIT_SG_COMPONENT(kitsune::scenegraph::graphics::Material)

    public:
        void setIndexBuffer(std::unique_ptr<IndexBuffer> buffer) {
            indexBuffer = buffer;
        }

        void setVertexBuffer(std::unique_ptr<Buffer<VertexType>> buffer) {
            vertexBuffer = buffer;
        }
        
    private:
        std::unique_ptr<IndexBuffer> indexBuffer;
        std::unique_ptr<Buffer<VertexType>> vertexBuffer;
    };*/

}
}
}
