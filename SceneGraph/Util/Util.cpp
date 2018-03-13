#include "Util.h"

namespace kitsune {
namespace scenegraph {
namespace util {
    
unsigned int addFlag(unsigned int flags, unsigned int flag)
{
    return flags | flag;
}

unsigned int removeFlag(unsigned int flags, unsigned int flag)
{
    return flags & ~flag;
}

std::string getExtension(const std::string &filename)
{
    auto extPos = filename.find_last_of(".");
    if (extPos == std::string::npos) {
        return std::string();
    }

    return filename.substr(extPos + 1);
}

}
}
}
