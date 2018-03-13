#pragma once

#include "Model.h"

#include <string>
#include <istream>

namespace kitsune {
namespace scenegraph {
namespace sdl {
namespace graphics {

/**
 * @brief Interface for all classes that load models from files
 */
class ModelLoader {
public:
    virtual void load(std::shared_ptr<Node> node, const std::string& filename) = 0;

    virtual void load(std::shared_ptr<Node> node, std::istream& stream) = 0;
};

}
}
}
}
