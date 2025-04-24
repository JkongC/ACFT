module Shader;

import Types;
import Config;
import :GLShader;

namespace ACFT
{
	Ref<Shader> Shader::Create(const std::filesystem::path& shader_path, ShaderLang language, ShaderType type)
	{
		switch (Config::GetRenderAPI())
		{
		case RenderAPI::OpenGL:

		default:
			break;
		}
	}

	Shader::Shader(const std::filesystem::path& shader_path, ShaderLang language, ShaderType type)
		: m_Path(shader_path), m_Lang(language), m_Type(type)
	{

	}
}
