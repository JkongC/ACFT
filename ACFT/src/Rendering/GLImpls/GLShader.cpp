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
import <filesystem>;
import Log;
import Shader;

#include "gldbg.h"

namespace GLImplementations
{
	GLShader::GLShader()
		: m_Path()
	{

	}
	
	GLShader::GLShader(const std::filesystem::path& filepath)
		: m_Path(filepath)
	{
		m_Identifier = CreateShader();
	}

	void GLShader::Bind() const
	{
		GLCall(glUseProgram(m_Identifier));
	}

	void GLShader::Unbind() const
	{
		GLCall(glUseProgram(0));
	}

	void GLShader::SetUniform1i(std::string_view name, int value)
	{
		GLCall(glUniform1i(GetUniformLocation(name), value));
	}

	void GLShader::SetUniform4f(std::string_view name, float v0, float v1, float v2, float v3)
	{
		GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
	}

	void GLShader::SetUniformMat4f(std::string_view name, const glm::mat4& matrix) {
		GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
	}

	void GLShader::SetUniformVec3f(std::string_view name, const glm::vec3& vec)
	{
		GLCall(glUniform3fv(GetUniformLocation(name), 1, &vec.x));
	}

	ShaderSources GLShader::ParseShader(const std::string& source)
	{
		std::istringstream src{ source };
		
		enum class ShaderType
		{
			NONE = -1, VERTEX = 0, FRAGMENT = 1
		};

		std::string line;
		std::stringstream ss[2];
		ShaderType type = ShaderType::NONE;
		while (std::getline(src, line)) {
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

	ShaderSources GLShader::ParseShader()
	{
		std::ifstream file(m_Path);
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

	unsigned int GLShader::CompileShader(unsigned int type, const std::string& source)
	{
		unsigned int ID = glCreateShader(type);
		const char* sc = source.c_str();
		GLCall(glShaderSource(ID, 1, &sc, 0));
		GLCall(glCompileShader(ID));

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

	unsigned int GLShader::CreateShader()
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

	unsigned int GLShader::CreateBasicShader()
	{
		static std::string basic_src = R"(
			#shader vertex
			#version 420 core

			layout(location = 0) in vec4 position;
			layout(location = 1) in vec2 texture_uv;

			out vec2 v_TexCoord;

			uniform mat4 mvp;

			void main()
			{
				gl_Position = mvp * position;
				v_TexCoord = texture_uv;
			}


			#shader fragment
			#version 420 core

			layout(location = 0) out vec4 color;

			in vec2 v_TexCoord;

			uniform sampler2D u_Texture;

			void main()
			{
				color = texture(u_Texture, v_TexCoord);
			}
		)";
		
		ShaderSources shadersources = ParseShader(basic_src);

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

	unsigned int GLShader::GetUniformLocation(std::string_view name) const
	{	
		if (auto it = m_UniformLocationCache.find(name);
			it != m_UniformLocationCache.end())
			return it->second;

		int location = glGetUniformLocation(m_Identifier, name.data());
		if (location == -1)
			ACFT_GL_LOG("Uniform '{}' doesn't exist.", name);

		m_UniformLocationCache.emplace(name, location);
		return location;
	}

	ACFT::RenderObjectIdentifier GLShader::GetIdentifier()
	{
		return m_Identifier;
	}
}