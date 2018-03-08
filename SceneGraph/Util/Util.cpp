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

}
}
}
