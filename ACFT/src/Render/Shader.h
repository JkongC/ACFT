#ifndef ACFT_SHADER_H_
#define ACFT_SHADER_H_

namespace ACFT
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

		void SetUniform1i(const std::string& name, int value);
		void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
		void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
	private:
		ShaderSources ParseShader();
		unsigned int CompileShader(unsigned int type, const std::string& source);
		unsigned int CreateShader();
		unsigned int GetUniformLocation(const std::string& name) const;


		unsigned int shader_id;
		std::string file_path;
		mutable std::unordered_map<std::string, int> uniform_location_cache;
	};
}

#endif