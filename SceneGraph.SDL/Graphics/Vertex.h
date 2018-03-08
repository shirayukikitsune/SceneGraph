#pragma once

#include <LinearMath/btVector3.h>
#include <glad/glad.h>

namespace kitsune {
namespace scenegraph {
namespace sdl {
namespace graphics {
namespace vertex {

    struct BaseVertex {
        static void bindAttributes() {};
        static void bindShaderProgram(GLuint shaderProgram) {};
    };

    struct PositionVertex : public BaseVertex
    {
        // Bullet vector3 is 16-byte aligned already, we don't need padding
        btVector3 position;

        static void bindAttributes() {
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PositionVertex), reinterpret_cast<void*>(offsetof(PositionVertex, position)));
            glEnableVertexAttribArray(0);
        }

        static void bindShaderProgram(GLuint shaderProgram) {
            glBindAttribLocation(shaderProgram, 0, "in_Position");
        }
    };

    struct PositionNormalVertex : public BaseVertex
    {
        btVector3 position;
        btVector3 normal;

        static void bindAttributes() {
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PositionNormalVertex), reinterpret_cast<void*>(offsetof(PositionNormalVertex, position)));
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(PositionNormalVertex), reinterpret_cast<void*>(offsetof(PositionNormalVertex, normal)));
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
        }

        static void bindShaderProgram(GLuint shaderProgram) {
            glBindAttribLocation(shaderProgram, 0, "in_Position");
            glBindAttribLocation(shaderProgram, 1, "in_Normal");
        }
    };

    struct PositionNormalColorVertex : public BaseVertex
    {
        PositionNormalColorVertex() {}
        PositionNormalColorVertex(btVector3 _p, btVector3 _n, btVector4 _c) : position(_p), normal(_n), color(_c) {}

        btVector3 position;
        btVector3 normal;
        btVector4 color;

        static void bindAttributes() {
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PositionNormalColorVertex), reinterpret_cast<void*>(offsetof(PositionNormalColorVertex, position)));
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(PositionNormalColorVertex), reinterpret_cast<void*>(offsetof(PositionNormalColorVertex, normal)));
            glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(PositionNormalColorVertex), reinterpret_cast<void*>(offsetof(PositionNormalColorVertex, color)));
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);
        }

        static void bindShaderProgram(GLuint shaderProgram) {
            glBindAttribLocation(shaderProgram, 0, "in_Position");
            glBindAttribLocation(shaderProgram, 1, "in_Normal");
            glBindAttribLocation(shaderProgram, 2, "in_Color");
        }
    };

    struct PositionNormalUVVertex : public BaseVertex
    {
        PositionNormalUVVertex() {}
        PositionNormalUVVertex(btVector3 _p, btVector3 _n, btVector4 _uv) : position(_p), normal(_n), uv(_uv) {}

        btVector3 position;
        btVector3 normal;
        // We use a vector4 for storage, but z and w aren't passed to GLSL
        btVector4 uv;

        static void bindAttributes() {
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PositionNormalUVVertex), reinterpret_cast<void*>(offsetof(PositionNormalUVVertex, position)));
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(PositionNormalUVVertex), reinterpret_cast<void*>(offsetof(PositionNormalUVVertex, normal)));
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(PositionNormalUVVertex), reinterpret_cast<void*>(offsetof(PositionNormalUVVertex, uv)));
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);
        }

        static void bindShaderProgram(GLuint shaderProgram) {
            glBindAttribLocation(shaderProgram, 0, "in_Position");
            glBindAttribLocation(shaderProgram, 1, "in_Normal");
            glBindAttribLocation(shaderProgram, 2, "in_UV");
        }
    };

}
}
}
}
}
