module;

#include <string>
#include <unordered_map>

module Shader;

import Types;
import Config;
import Log;
import Renderer;

namespace ACFT
{
	Ref<Shader> Shader::Create(const std::filesystem::path& shader_path, ShaderLang language, ShaderType type)
	{
		Shader* shader = new Shader(shader_path, language, type);
		return Ref<Shader>(shader);
	}

	Ref<Shader> Shader::CreateBasic()
	{
		Shader* shader = new Shader();
		return Ref<Shader>(shader);
	}

	Shader::Shader(const std::filesystem::path& shader_path, ShaderLang language, ShaderType type)
		: m_Path(shader_path), m_Lang(language), m_Type(type)
	{
		m_Identifier = Renderer::Get()->MakeShader(m_Path, m_Lang, m_Type);
	}

	Shader::Shader()
		: m_Path()
	{
		switch (Config::GetRenderAPI())
		{
		case RenderAPI::OpenGL:
			m_Lang = ShaderLang::GLSL;
			m_Type = ShaderType::glsl_mixed;
			m_Identifier = Renderer::Get()->MakeBasicShader();
			break;
		default:
			break;
		}
	}

	RenderObjectIdentifier Shader::GetIdentifier() const
	{
		return m_Identifier;
	}

	void ShaderLib::Init()
	{
		if (ShaderLib::s_Initialized)
		{
			ACFT_LOG_WARN("ShaderLib is already initialized!");
			return;
		}
		
		ShaderLib::s_Shaders.try_emplace("basic", Shader::CreateBasic());
		
		for (const auto& file : std::filesystem::recursive_directory_iterator(Config::GetShaderPath()))
		{
			ShaderLib::s_Shaders.try_emplace(file.path().stem().string(), Shader::Create(file.path()));
		}

		ShaderLib::s_Initialized = true;
	}

	Ref<Shader> ShaderLib::GetShader(std::string_view shader_name)
	{
		auto& shaders = ShaderLib::s_Shaders;
		if (auto it = shaders.find(shader_name);
			it != shaders.end())
		{
			return it->second;
		}
		else
		{
			ACFT_LOG_ERROR("Trying to get an unexisted shader \"{}\"!", shader_name);
			return nullptr;
		}
	}

	Ref<Shader> ShaderLib::GetBasicShader()
	{
		return ShaderLib::GetShader("basic");
	}
}
