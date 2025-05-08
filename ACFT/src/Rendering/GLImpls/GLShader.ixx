module;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <unordered_map>
#include <functional>

export module Renderer:GLShader;

import <filesystem>;

import Types;
import Shader;

namespace GLImplementations
{
	export struct ShaderSources {
		std::string vertex;
		std::string fragment;
	};

	export class GLShader
	{
	public:
		GLShader(const std::filesystem::path& path);
		GLShader(GLShader&&) = default;

		void Bind() const;
		void Unbind() const;

		unsigned int GetUniformLocation(std::string_view name) const;

		void SetUniform1i(std::string_view name, int value);
		void SetUniform4f(std::string_view name, float v0, float v1, float v2, float v3);
		void SetUniformMat4f(std::string_view name, const glm::mat4& matrix);
		void SetUniformVec3f(std::string_view name, const glm::vec3& vec);

		ACFT::RenderObjectIdentifier GetIdentifier();

	private:
		ShaderSources ParseShader();
		unsigned int CompileShader(unsigned int type, const std::string& source);
		unsigned int CreateShader();

	private:
		std::filesystem::path m_Path;
		unsigned int m_Identifier;
		mutable std::unordered_map<std::string, int, StringHashFunc, EqualFunc> m_UniformLocationCache;
	};
}