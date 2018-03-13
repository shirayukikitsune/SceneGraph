#pragma once

#include <string>

namespace kitsune {
namespace scenegraph {
namespace util {

    unsigned int addFlag(unsigned int flags, unsigned int flag);
    unsigned int removeFlag(unsigned int flags, unsigned int flag);

    std::string getExtension(const std::string &filename);

}
}
}
