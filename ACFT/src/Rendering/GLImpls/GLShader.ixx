module;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

export module Shader:GLShader;

import <string>;
import <unordered_map>;

import Types;
import Shader;

namespace GLImplementations
{
	struct ShaderSources {
		std::string vertex;
		std::string fragment;
	};

	export class GLShader : public ACFT::Shader
	{
	public:
		GLShader(const std::filesystem::path& path, ACFT::ShaderLang language, ACFT::ShaderType type);

		void Bind() const;
		void Unbind() const;

		unsigned int GetUniformLocation(const std::string& name) const;

		void SetUniform1i(const std::string& name, int value);
		void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
		void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
		void SetUniformVec3f(const std::string& name, const glm::vec3& vec);

		virtual ACFT::RenderObjectIdentifier GetIdentifier() override;

		virtual void Use() override;
		virtual void Unuse() override;

	private:
		ShaderSources ParseShader();
		unsigned int CompileShader(unsigned int type, const std::string& source);
		unsigned int CreateShader();

	private:
		unsigned int m_Identifier;
		mutable std::unordered_map<std::string, int> m_UniformLocationCache;
	};
}