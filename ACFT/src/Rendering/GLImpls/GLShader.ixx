module;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

export module Renderer:GLShader;

import <string>;
import <unordered_map>;

namespace GLImplementations
{
	struct ShaderSources {
		std::string vertex;
		std::string fragment;
	};

	class Shader
	{
	public:
		Shader(const std::string& filepath);

		void Bind() const;
		void Unbind() const;

		inline unsigned int GetID() const { return m_ShaderID; }

		unsigned int GetUniformLocation(const std::string& name) const;

		void SetUniform1i(const std::string& name, int value);
		void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
		void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
		void SetUniformVec3f(const std::string& name, const glm::vec3& vec);
	private:
		ShaderSources ParseShader();
		unsigned int CompileShader(unsigned int type, const std::string& source);
		unsigned int CreateShader();

		unsigned int m_ShaderID;
		std::string m_FilePath;
		mutable std::unordered_map<std::string, int> m_UniformLocationCache;
	};
}