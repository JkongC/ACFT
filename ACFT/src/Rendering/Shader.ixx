module;

#include "Platform.h"

export module Shader;

import <filesystem>;
import <unordered_map>;

import Types;

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
		friend class ShaderLib;
		Shader(const std::filesystem::path& shader_path, ShaderLang language = ShaderLang::GLSL, ShaderType type = ShaderType::glsl_mixed);
		Shader();
		
		static Ref<Shader> CreateBasic();

		std::filesystem::path m_Path;
		ShaderLang m_Lang;
		ShaderType m_Type;
		RenderObjectIdentifier m_Identifier;
	};

	export class ShaderLib
	{
	public:
		static void Init();
		static Ref<Shader> GetShader(std::string_view shader_name);
		static Ref<Shader> GetBasicShader();

	private:
		ShaderLib() = default;
		ShaderLib(const ShaderLib&) = delete;
		ShaderLib(ShaderLib&&) = delete;
		ShaderLib& operator=(const ShaderLib&) = delete;

	private:
		static inline bool s_Initialized = false;
		static inline std::unordered_map<std::string, Ref<Shader>, StringHashFunc, EqualFunc> s_Shaders;
	};
}