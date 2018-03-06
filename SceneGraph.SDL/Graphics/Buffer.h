#pragma once

#include <glad/glad.h>

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
    GLenum convertBufferFrequencyAndAccess(BufferFrequency frequency, BufferAccess access);

    template <typename DataType, typename GraphicsType, int bufferCount>
    class ArrayBuffer {
        // Disable buffer copy
        ArrayBuffer(const ArrayBuffer&) = delete;
        ArrayBuffer& operator= (const ArrayBuffer&) = delete;

        GLuint bufferId[bufferCount];

        std::unique_ptr<DataType[]> cpuBuffer[bufferCount];
        GraphicsType gpuBuffer[bufferCount];

    public:
        ///! Initializes an empty buffer
        explicit ArrayBuffer() {
            std::copy(bufferId, bufferId + BufferCount, 0);
        }

        void createBuffers() {
            this->freeBuffers();

            glGenBuffers(bufferCount, bufferId);
        }

        void freeBuffers() {
            glDeleteBuffers(bufferCount, bufferId);
        }

        ///! Returns the pointer to the internal buffer
        /// \remarks the pointer should never be deleted
        DataType* getCpuBuffer(int which) { return cpuBuffer[which].get(); }
    };

    template <typename D, typename G>
    class ArrayBuffer<D, G, 1> {
        // Disable buffer copy
        ArrayBuffer(const ArrayBuffer&) = delete;
        ArrayBuffer& operator= (const ArrayBuffer&) = delete;

        GLuint bufferId;

        std::unique_ptr<DataType[]> cpuBuffer;
        GraphicsType gpuBuffer;

    public:
        ///! Initializes an empty buffer
        explicit ArrayBuffer() {
            bufferId = 0;
        }

        ///! Copies the `contents` of size `size` to this buffer
        ArrayBuffer(DataType* contents, std::size_t size, BufferTarget target, BufferFrequency frequency, BufferAccess access)
        {
            bufferId = 0;
            this->createBuffer(contents, size);
        }

        void createBuffer(DataType* contents, std::size_t size) {
            cpuBuffer = new DataType[size];
            std::copy(contents, contents + size, cpuBuffer.get());

            if (bufferId != 0) {
                glDeleteBuffers(1, &bufferId);
            }

            glGenBuffers(1, &bufferId);
            glBindBuffer(convertBufferTarget(target), bufferId);
            glBufferData(convertBufferTarget(target), size, contents, convertBufferFrequencyAndAccess(frequency, access));
        }

        ///! Returns the pointer to the internal buffer
        /// \remarks the pointer should never be deleted
        DataType* getCpuBuffer() { return cpuBuffer.get(); }
    };

    template <typename D, typename G> typedef ArrayBuffer<D, G, 1> Buffer;

}
}
}
}
