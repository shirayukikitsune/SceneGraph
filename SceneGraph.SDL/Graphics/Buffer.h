#pragma once

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

namespace kitsune {
namespace scenegraph {
namespace sdl {
namespace graphics {

    enum struct BufferFrequency {
        ///! The buffer contents will be used a few times and modified only once
        Stream,
        ///! The buffer contents will be used many times and modified only once
        Static,
        ///! The buffer contents will be used and modified several times
        Dynamic
    };

    enum struct BufferAccess {
        ///! The buffer is modified by the application and read by the graphics stack
        Draw,
        ///! The buffer is modified by the graphics stack and read by the application
        Read,
        ///! The buffer gives read/write access to the graphics stack
        Copy
    };

    // Names adapted from OpenGL
    enum struct BufferTarget {
        Vertex,
        IndirectDispatch,
        Command,
        Index,
        PixelPack,
        PixelUnpack,
        Query,
        ShaderStorage,
        Texture,
        TransformFeedback,
        Uniform
    };

    GLenum convertBufferTarget(BufferTarget target);

    template <typename DataType, typename GraphicsType>
    class Buffer {
        // Disable buffer copy
        Buffer(const Buffer&) = delete;
        Buffer& operator= (const Buffer&) = delete;

        GLuint bufferId;

        std::unique_ptr<DataType[]> cpuBuffer;
        GraphicsType gpuBuffer;

    public:
        ///! Initializes an empty buffer
        explicit Buffer() {
            cpuBuffer = nullptr;
        }

        ///! Copies the `contents` of size `size` to this buffer
        Buffer(DataType* contents, std::size_t size, BufferTarget target, BufferFrequency frequency, BufferAccess access)
        {
            this->createBuffer(contents, size);
            glBindBuffer(convertBufferTarget(target), bufferId);
            glBufferData(convertBufferTarget(target), size, contents, GL_STREAM_DRAW);
        }

        void createBuffer(DataType* contents, std::size_t size) {
            cpuBuffer = new DataType[size];
            std::copy(contents, contents + size, cpuBuffer.get());

            glGenBuffers(1, &bufferId);
        }

        ///! Returns the pointer to the internal buffer
        /// \remarks the pointer should never be deleted
        DataType* getCpuBuffer() { return cpuBuffer.get(); }

    };

}
}
}
}
