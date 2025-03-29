export module Shader;

import <filesystem>;

namespace ACFT
{
	export enum class ShaderLang
	{
		GLSL
	};

	export enum class ShaderType
	{
		glsl_mixed, glsl_vertex, glsl_fragment, glsl_compute
	};
	
	export class Shader
	{
	public:
		Shader(std::filesystem::path shader_path, ShaderLang language = ShaderLang::GLSL, ShaderType type = ShaderType::glsl_mixed)
			: m_Path(shader_path), m_Lang(language), m_Type(type)
		{}

	private:
		std::filesystem::path m_Path;
		ShaderLang m_Lang;
		ShaderType m_Type{ShaderType::glsl_mixed};
	};
}