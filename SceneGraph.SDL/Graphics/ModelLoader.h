#pragma once

#include "Model.h"

#include <string>
#include <istream>

namespace kitsune::scenegraph::sdl::graphics {

/**
 * @brief Interface for all classes that load models from files
 */
class ModelLoader {
public:
    virtual void load(std::shared_ptr<Node> node,
            const std::string& filename,
            BufferAccess indexBufferAccess,
            BufferFrequency indexBufferFrequency,
            BufferAccess vertexBufferAccess,
            BufferFrequency vertexBufferFrequency) = 0;

    virtual void load(std::shared_ptr<Node> node,
            std::istream& stream,
            BufferAccess indexBufferAccess,
            BufferFrequency indexBufferFrequency,
            BufferAccess vertexBufferAccess,
            BufferFrequency vertexBufferFrequency) = 0;
};

}
