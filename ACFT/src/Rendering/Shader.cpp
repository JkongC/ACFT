module;

#include <string>
#include <unordered_map>

module Shader;

import Types;
import Config;
import Renderer;

namespace ACFT
{
	Ref<Shader> Shader::Create(const std::filesystem::path& shader_path, ShaderLang language, ShaderType type)
	{
		Shader* shader = new Shader(shader_path, language, type);
		return Ref<Shader>(shader);
	}

	Shader::Shader(const std::filesystem::path& shader_path, ShaderLang language, ShaderType type)
		: m_Path(shader_path), m_Lang(language), m_Type(type)
	{
		m_Identifier = Renderer::GetRenderer().MakeShader(m_Path, m_Lang, m_Type);
	}

	RenderObjectIdentifier Shader::GetIdentifier() const
	{
		return m_Identifier;
	}
}
