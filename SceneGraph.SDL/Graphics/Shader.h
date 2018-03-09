#pragma once

#include "../Components/CameraComponent.h"
#include "Vertex.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SDL2/SDL.h>

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <deque>
#include <Components/SceneEventComponent.h>

namespace kitsune {
namespace scenegraph {
namespace sdl {
namespace graphics {

enum struct ShaderType : GLenum {
	Compute = GL_COMPUTE_SHADER,
	Vertex = GL_VERTEX_SHADER,
	TesselateControl = GL_TESS_CONTROL_SHADER,
	TesselateEvaluation = GL_TESS_EVALUATION_SHADER,
	Geometry = GL_GEOMETRY_SHADER,
	Fragment = GL_FRAGMENT_SHADER
};

template <class VertexType>
class Shader : public SceneEventComponent
{
	KIT_SG_COMPONENT(kitsune::scenegraph::sdl::graphics::Shader)
	static_assert(std::is_base_of<vertex::BaseVertex, VertexType>::value, "VertexType must inherit from BaseVertex");

public:
	std::string readFile(const char* file)
	{
		// Open file
		std::ifstream t(file);

		// Read file into buffer
		std::stringstream buffer;
		buffer << t.rdbuf();

		// Make a std::string and fill it with the contents of buffer
		std::string fileContent = buffer.str();

		return fileContent;
	}

	void useProgram()
	{
		// Load the shader into the rendering pipeline
		glUseProgram(shaderProgram);
	}

	void initialize() override
	{
		// Generate our shader. This is similar to glGenBuffers() and glGenVertexArray(), except that this returns the ID
		shaderProgram = glCreateProgram();

		// Bind the location of our attributes
		VertexType::bindShaderProgram(shaderProgram);
	}

	bool append(const std::string &filename, ShaderType type) {
		// Read file as std::string
		std::string str = readFile(filename.c_str());

		// c_str() gives us a const char*, but we need a non-const one
		int32_t size = str.length();

		// Create an shader handle
		GLuint shader = glCreateShader((GLenum)type);

		auto code = str.c_str();

		// Send the vertex shader source code to OpenGL
		glShaderSource(shader, 1, &code, &size);

		// Compile the vertex shader
		glCompileShader(shader);

		int wasCompiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &wasCompiled);

		if (wasCompiled == 0) {
			printShaderCompilationErrorInfo(shader);
			return false;
		}

		shaders.emplace_back(shader);

		glAttachShader(shaderProgram, shader);
		return true;
	}

	bool linkShaders()
	{
		// Link. At this point, our shaders will be inspected/optimized and the binary code generated
		// The binary code will then be uploaded to the GPU
		glLinkProgram(shaderProgram);

		// Verify that the linking succeeded
		int isLinked;
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, (int *)&isLinked);

		if (isLinked == false)
			printShaderLinkingError(shaderProgram);

		MatrixID = glGetUniformLocation(shaderProgram, "MVP");

		return isLinked != 0;
	}

	void printShaderLinkingError(int32_t shaderId)
	{
		std::cout << "=======================================\n";
		std::cout << "Shader linking failed : " << std::endl;

		// Find length of shader info log
		int maxLength;
		glGetProgramiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

		std::cout << "Info Length : " << maxLength << std::endl;

		// Get shader info log
		char* shaderProgramInfoLog = new char[maxLength];
		glGetProgramInfoLog(shaderProgram, maxLength, &maxLength, shaderProgramInfoLog);

		std::cout << "Linker error message : " << shaderProgramInfoLog << std::endl;

		/* Handle the error in an appropriate way such as displaying a message or writing to a log file. */
		/* In this simple program, we'll just leave */
		delete[] shaderProgramInfoLog;
		return;
	}

	// If something went wrong whil compiling the shaders, we'll use this function to find the error
	void printShaderCompilationErrorInfo(int32_t shaderId)
	{
		std::cout << "=======================================\n";
		std::cout << "Shader compilation failed : " << std::endl;

		// Find length of shader info log
		int maxLength;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

		// Get shader info log
		char* shaderInfoLog = new char[maxLength];
		glGetShaderInfoLog(shaderId, maxLength, &maxLength, shaderInfoLog );

		// Print shader info log
		std::cout << "\tError info : " << shaderInfoLog << std::endl;

		std::cout << "=======================================\n\n";
		delete[] shaderInfoLog;
	}

	void cleanUp()
	{
		/* Cleanup all the things we bound and allocated */
		glUseProgram(0);

		for (auto & shader : shaders) {
			glDetachShader(shaderProgram, shader);
		}

		glDeleteProgram(shaderProgram);

		for (auto & shader : shaders) {
			glDeleteShader(shader);
		}
	}

	void onPreUpdate(float DeltaTime) override {
		glm::mat4 model;
		auto node = getNode();
		auto transform = node->getWorldTransform();
	    transform.getOpenGLMatrix(glm::value_ptr(model));

		auto camera = node->getScene()->getActiveCamera()->template getComponent<components::Camera>();

		glm::mat4 mvp = camera->getProjection() * camera->getView() * model;

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
	}

	GLuint MatrixID;

	// The handle to our shader program
	GLuint shaderProgram;

	// The handles to each added shader
	std::deque<GLuint> shaders;

};

}
}
}
}
