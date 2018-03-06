#include "Buffer.h"

using namespace kitsune::scenegraph::sdl::graphics;

GLenum convertBufferTarget(BufferTarget target) {
    switch (target) {
        case BufferTarget::Vertex:
            return GL_ARRAY_BUFFER;
        case BufferTarget::Index:
            return GL_ELEMENT_ARRAY_BUFFER;
        case BufferTarget::Uniform:
#if defined GL_UNIFORM_BUFFER
            return GL_UNIFORM_BUFFER;
#elif defined GL_UNIFORM_BUFFER_EXT
            return GL_UNIFORM_BUFFER_EXT;
#endif
    }

    return 0;
}
