#pragma once

#include <string>

namespace kitsune::scenegraph::util {

    template <class T>
    T addFlag(T flags, T flag) {
        return flags | flag;
    }

    template <class T>
    T removeFlag(T flags, T flag) {
        return flags & ~flag;
    }

    std::string getExtension(const std::string &filename);

}
