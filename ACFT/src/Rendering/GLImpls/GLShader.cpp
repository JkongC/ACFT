module;

#include <glew.h>
#include <glfw3.h>
#include <malloc.h>
#include <sstream>
#include <fstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

module Renderer:GLShader;

import <string>;
import Log;

#include "gldbg.h"

namespace GLImplementations
{
	Shader::Shader(const std::string& filepath)
		: m_FilePath(filepath), m_ShaderID(0)
	{
		m_ShaderID = CreateShader();
	}

	void Shader::Bind() const
	{
		GLCall(glUseProgram(m_ShaderID));
	}

	void Shader::Unbind() const
	{
		GLCall(glUseProgram(0));
	}

	void Shader::SetUniform1i(const std::string& name, int value)
	{
		GLCall(glUniform1i(GetUniformLocation(name), value));
	}

	void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
	{
		GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
	}

	void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix) {
		GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
	}

	void Shader::SetUniformVec3f(const std::string& name, const glm::vec3& vec)
	{
		GLCall(glUniform3fv(GetUniformLocation(name), 1, &vec.x));
	}

	ShaderSources Shader::ParseShader()
	{
		std::ifstream file(m_FilePath);
		if (!file.is_open()) {
			ACFT_LOG_ERROR("Failed to open shader file!");
		}

		enum class ShaderType
		{
			NONE = -1, VERTEX = 0, FRAGMENT = 1
		};

		std::string line;
		std::stringstream ss[2];
		ShaderType type = ShaderType::NONE;
		while (std::getline(file, line)) {
			if (line.find("#shader") != std::string::npos) {
				if (line.find("vertex") != std::string::npos) {
					type = ShaderType::VERTEX;
				}
				else if (line.find("fragment") != std::string::npos) {
					type = ShaderType::FRAGMENT;
				}
			}
			else {
				ss[static_cast<int>(type)] << line << "\n";
			}
		}

		return { ss[0].str(), ss[1].str() };
	}

	unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
	{
		unsigned int ID = glCreateShader(type);
		const char* sc = source.c_str();
		glShaderSource(ID, 1, &sc, 0);
		glCompileShader(ID);

		int result;
		glGetShaderiv(ID, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE) {
			int length;
			glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &length);
			char* message = (char*)alloca(length * sizeof(char));
			glGetShaderInfoLog(ID, length, &length, message);

			ACFT_GL_LOG("{}", message);

			glDeleteShader(ID);

			return 0;
		}

		return ID;
	}

	unsigned int Shader::CreateShader()
	{
		ShaderSources shadersources = ParseShader();

		unsigned int program = glCreateProgram();
		unsigned int vertex = CompileShader(GL_VERTEX_SHADER, shadersources.vertex);
		unsigned int fragment = CompileShader(GL_FRAGMENT_SHADER, shadersources.fragment);

		glAttachShader(program, vertex);
		glAttachShader(program, fragment);
		glLinkProgram(program);
		glValidateProgram(program);

		glDeleteShader(vertex);
		glDeleteShader(fragment);

		return program;
	}

	unsigned int Shader::GetUniformLocation(const std::string& name) const
	{
		if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
			return m_UniformLocationCache[name];

		int location = glGetUniformLocation(m_ShaderID, name.c_str());
		if (location == -1)
			ACFT_GL_LOG("Uniform '{}' doesn't exist.", name);

		m_UniformLocationCache[name] = location;
		return location;
	}
}