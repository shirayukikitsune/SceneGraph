#include "ModelLoaderFactory.h"

#include "Util/Util.h"

#include <fstream>
#include <stdexcept>

#include "OBJModelLoader.ipp"

using kitsune::scenegraph::sdl::graphics::ModelLoaderFactory;
namespace sg = kitsune::scenegraph;
namespace sdlg = kitsune::scenegraph::sdl::graphics;

std::map<std::string, sdlg::ModelLoader*> ModelLoaderFactory::loaders;

void ModelLoaderFactory::registerLoader(const std::string &extension, sdlg::ModelLoader *loader)
{
    ModelLoaderFactory::loaders[extension] = loader;
}

void ModelLoaderFactory::registerDefaultLoaders()
{
    registerLoader("obj", new sdlg::implementations::OBJModelLoader);
}

void ModelLoaderFactory::load(std::shared_ptr<sg::Node> node, const std::string &file, std::ios_base::openmode mode)
{
    auto extension = kitsune::scenegraph::util::getExtension(file);

    std::ifstream filestream(file, mode);

    load(node, extension, filestream);

    filestream.close();
}

void ModelLoaderFactory::load(std::shared_ptr<sg::Node> node, const std::string &extension, std::istream &stream)
{
    auto loader = ModelLoaderFactory::loaders.find(extension);

    if (loader == ModelLoaderFactory::loaders.end()) {
        throw std::domain_error::domain_error("unregistered loader");
    }

    return loader->second->load(node, stream);
}
