#include "ModelLoader.h"

#include <fstream>

namespace kitsune {
namespace scenegraph {
namespace sdl {
namespace graphics {
namespace implementations {

class OBJModelLoader : public ModelLoader {
public:
    void load(std::shared_ptr<Node> node, const std::string& filename) override {
        std::ifstream filestream(filename, std::ios::in);

        load(node, filestream);

        filestream.close();
    }

    void load(std::shared_ptr<Node> node, std::istream& stream) override {
        auto model = node->createComponent<Model>();
    }
};

}
}
}
}
}
