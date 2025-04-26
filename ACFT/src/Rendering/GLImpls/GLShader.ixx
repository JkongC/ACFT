module;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <unordered_map>

export module Renderer:GLShader;

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

		unsigned int GetUniformLocation(const std::string& name) const;

		void SetUniform1i(const std::string& name, int value);
		void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
		void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
		void SetUniformVec3f(const std::string& name, const glm::vec3& vec);

		ACFT::RenderObjectIdentifier GetIdentifier();

	private:
		ShaderSources ParseShader();
		unsigned int CompileShader(unsigned int type, const std::string& source);
		unsigned int CreateShader();

	private:
		std::filesystem::path m_Path;
		unsigned int m_Identifier;
		mutable std::unordered_map<std::string, int> m_UniformLocationCache;
	};
}