#pragma once

#include "shaders.h"

GLuint
shaders::load_shaders(
    const std::string_view& vertexShaderPath,
    const std::string_view& fragmentShaderPath
) const noexcept {

	GLint result = GL_FALSE;
	int32_t logSize = 0;

	// load and read the vertex shader code from the file
	std::string vShaderCode;
	std::ifstream vShaderStream(vertexShaderPath.data(), std::ios::in);
	if (vShaderStream.is_open()) {
		std::ostringstream os;
		os << vShaderStream.rdbuf();
		vShaderCode = os.str();
		vShaderStream.close();
	} else {
		std::cerr << "[load_shader] Unable to open file " << vertexShaderPath << "\n";
		return 0;
	}

	// load and read the fragment shader code from the file
	std::string fShaderCode;
	std::ifstream fShaderStream(fragmentShaderPath.data(), std::ios::in);
	if (fShaderStream.is_open()) {
		std::ostringstream os;
		os << fShaderStream.rdbuf();
		fShaderCode = os.str();
		fShaderStream.close();
	} else {
		std::cerr << "[load_shader] Unable to open file " << fShaderCode << "\n";
		return 0;
	}

	// Compile vertex shader
	std::cout << "Compiling vertex shader " << vertexShaderPath << "\n";
    GLuint vShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLchar const* vShaderSourcePtr = vShaderCode.c_str();
	glShaderSource(vShaderID, 1, &vShaderSourcePtr, NULL);
	glCompileShader(vShaderID);

	// check info log
	glGetShaderiv(vShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vShaderID, GL_INFO_LOG_LENGTH, &logSize);
	if (logSize > 0) {
		std::vector<uint8_t> vShaderErrLog(logSize + 1);
		glGetShaderInfoLog(vShaderID, logSize, NULL, (GLchar*)vShaderErrLog.data());
		std::cerr << "[load_shader] " << vShaderErrLog.data() << "\n";
	}

	// Compile frag shader
	std::cout << "Compiling fragment shader " << fragmentShaderPath << "\n";
	GLuint fShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	GLchar const* fShaderSourcePtr = fShaderCode.c_str();
	glShaderSource(fShaderID, 1, &fShaderSourcePtr, NULL);
	glCompileShader(fShaderID);

	// check info log
	glGetShaderiv(fShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fShaderID, GL_INFO_LOG_LENGTH, &logSize);
	if (logSize > 0) {
		std::vector<uint8_t> fShaderErrLog(logSize + 1);
		glGetShaderInfoLog(fShaderID, logSize, NULL, (GLchar*)fShaderErrLog.data());
		std::cerr << "[load_shader] " << fShaderErrLog.data() << "\n";
	}

	// link the program
	std::cout << "Linking the shaders to the current running program..\n";
	GLuint pid = glCreateProgram();
	glAttachShader(pid, vShaderID);
	glAttachShader(pid, fShaderID);
	glLinkProgram(pid);

	// Check the program
	glGetProgramiv(pid, GL_LINK_STATUS, &result);
	glGetProgramiv(pid, GL_INFO_LOG_LENGTH, &logSize);
	if (logSize > 0) {
		std::vector<uint8_t> programErrorLog(logSize + 1);
		glGetProgramInfoLog(pid, logSize, NULL, (GLchar*)programErrorLog[0]);
		std::cerr << "[load_shader] " << programErrorLog.data() << "\n";
	}

	glDetachShader(pid, vShaderID);
	glDetachShader(pid, fShaderID);

	glDeleteShader(vShaderID);
	glDeleteShader(fShaderID);

	return pid;
}