#pragma once

#include <LinearMath/btVector3.h>

namespace kitsune {
namespace scenegraph {
namespace graphics {
namespace vertex {

    struct BaseVertex {};

    struct PositionVertex : public BaseVertex
    {
        btVector3 position;
    };

    struct PositionNormalVertex : public BaseVertex
    {
        btVector3 position;
        btVector3 normal;
    };

    struct PositionNormalColorVertex : public BaseVertex
    {
        btVector3 position;
        btVector3 normal;
        btVector3 color;
    };

}
}
}
}
