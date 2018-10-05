#include "Util.h"

namespace kitsune::scenegraph::util {

    std::string getExtension(const std::string &filename) {
        auto extPos = filename.find_last_of('.');
        if (extPos == std::string::npos) {
            return std::string();
        }

        return filename.substr(extPos + 1);
    }

}
