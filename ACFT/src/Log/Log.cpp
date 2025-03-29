module;

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

module Log;

namespace ACFT
{
	void Logger::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		acft_logger = spdlog::stdout_color_mt("AnotherCraft");
		gl_logger = spdlog::stdout_color_mt("OpenGL");

		spdlog::set_level(spdlog::level::trace);
	}
}

template<typename ...Args>
void ACFT_LOG_ERROR(spdlog::format_string_t<Args...> fmt, Args&&... args)
{
	::ACFT::Logger::GetACFTLogger()->error(fmt, std::forward<Args>(args)...);
}

template<typename ...Args>
void ACFT_LOG_WARN(spdlog::format_string_t<Args...> fmt, Args&&... args)
{
	::ACFT::Logger::GetACFTLogger()->warn(fmt, std::forward<Args>(args)...);
}

template<typename ...Args>
void ACFT_LOG_INFO(spdlog::format_string_t<Args...> fmt, Args&&... args)
{
	::ACFT::Logger::GetACFTLogger()->info(fmt, std::forward<Args>(args)...);
}

template<typename ...Args>
void ACFT_LOG_TRACE(spdlog::format_string_t<Args...> fmt, Args&&... args)
{
	::ACFT::Logger::GetACFTLogger()->trace(fmt, std::forward<Args>(args)...);
}

template<typename ...Args>
void ACFT_LOG_FATAL(spdlog::format_string_t<Args...> fmt, Args&&... args)
{
	::ACFT::Logger::GetACFTLogger()->error(fmt, std::forward<Args>(args)...);
}

template<typename ...Args>
void ACFT_GL_LOG(spdlog::format_string_t<Args...> fmt, Args&&... args)
{
	::ACFT::Logger::GetGLLogger()->warn(fmt, std::forward<Args>(args)...);
}