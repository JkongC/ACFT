module;

#include "Platform.h"

export module Shader;

import Types;
export import <filesystem>;

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
		static Ref<Shader> Create(const std::filesystem::path& shader_path, ShaderLang language = ShaderLang::GLSL, ShaderType type = ShaderType::glsl_mixed);

		RenderObjectIdentifier GetIdentifier() const;

	protected:
		Shader(const std::filesystem::path& shader_path, ShaderLang language = ShaderLang::GLSL, ShaderType type = ShaderType::glsl_mixed);

		std::filesystem::path m_Path;
		ShaderLang m_Lang;
		ShaderType m_Type{ShaderType::glsl_mixed};
		RenderObjectIdentifier m_Identifier;
	};
}