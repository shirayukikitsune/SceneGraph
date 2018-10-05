#pragma once

#include <glad/glad.h>
#include <array>
#include <vector>

namespace kitsune::scenegraph::sdl::graphics {

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

    template <typename DataType, int bufferCount>
    class ArrayBuffer {
        std::array<GLuint, bufferCount> bufferId;

        std::array<std::vector<DataType>, bufferCount> cpuBuffer;

    public:
        // Disable buffer copy
        ArrayBuffer(const ArrayBuffer&) = delete;
        ArrayBuffer& operator= (const ArrayBuffer&) = delete;

        ///! Initializes an empty buffer
        explicit ArrayBuffer() {
            bufferId.fill(0);
        }

        void createBuffers() {
            this->freeBuffers();

            glGenBuffers(bufferCount, bufferId);
        }

        void assignBufferData(int where, DataType* contents, std::size_t length, BufferTarget target, BufferFrequency frequency, BufferAccess access) {
            cpuBuffer[where].resize(length);
            std::copy(contents, contents + length, cpuBuffer[where].begin());
            glBindBuffer(convertBufferTarget(target), bufferId[where]);
            glBufferData(convertBufferTarget(target), length * sizeof(DataType), contents, convertBufferFrequencyAndAccess(frequency, access));
        }

        void freeBuffers() {
            glDeleteBuffers(bufferCount, bufferId);
        }

        ///! Returns the pointer to the internal buffer
        /// \remarks the pointer should never be deleted
        const std::vector<DataType>& getCpuBuffer(int which) const { return cpuBuffer[which]; }
    };

    template <typename DataType>
    class ArrayBuffer<DataType, 1> {
        GLuint bufferId;

        std::vector<DataType> cpuBuffer;

        BufferTarget target;

    public:
        // Disable buffer copy
        ArrayBuffer(const ArrayBuffer&) = delete;
        ArrayBuffer& operator= (const ArrayBuffer&) = delete;

        ///! Initializes an empty buffer
        explicit ArrayBuffer(BufferTarget target)
            : target(target)
        {
            bufferId = 0;
        }

        ///! Copies the `contents` of length `length` to this buffer
        ArrayBuffer(DataType* contents, std::size_t length, BufferTarget target, BufferFrequency frequency, BufferAccess access)
            : target(target)
        {
            bufferId = 0;
            this->createBuffer(contents, length, frequency, access);
        }

        void create(DataType* contents, std::size_t length, BufferFrequency frequency, BufferAccess access) {
            cpuBuffer.resize(length);
            std::copy(contents, contents + length, cpuBuffer.begin());

            if (bufferId != 0) {
                glDeleteBuffers(1, &bufferId);
            }

            glGenBuffers(1, &bufferId);
            glBindBuffer(convertBufferTarget(target), bufferId);
            glBufferData(convertBufferTarget(target), length * sizeof(DataType), contents, convertBufferFrequencyAndAccess(frequency, access));
        }

        void bind() {
            glBindBuffer(convertBufferTarget(target), bufferId);
        }

        ///! Returns the pointer to the internal buffer
        /// \remarks the pointer should never be deleted
        const std::vector<DataType>& get() const { return cpuBuffer; }

        ///! Returns the id of the GPU buffer
        GLuint getGpuBuffer() { return bufferId; }
    };

}
