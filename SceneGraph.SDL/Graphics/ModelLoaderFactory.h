#pragma once

#include "ModelLoader.h"

#include <istream>
#include <map>
#include <string>

namespace kitsune::scenegraph::sdl::graphics {

/**
 * @brief Factory that instantiates {@see ModelLoader}s
 */
    class ModelLoaderFactory {
    public:
        static void registerLoader(const std::string &extension, ModelLoader *loader);

        static void registerDefaultLoaders();

        static void
        load(std::shared_ptr<Node> node, const std::string &file, std::ios_base::openmode mode = std::ios_base::in);

        static void load(std::shared_ptr<Node> node, const std::string &type, std::istream &stream);

    private:
        static std::map<std::string, ModelLoader *> loaders;
    };

}
