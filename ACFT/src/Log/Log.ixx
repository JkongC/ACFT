module;

#define SPDLOG_COMPILED_LIB
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

export module Log;

import <memory>;

namespace ACFT
{
	export enum LogPriority
	{
		trace = 0,
		info,
		warn,
		error,
		fatal
	};

	export class Logger
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetACFTLogger() { return acft_logger; }
		inline static std::shared_ptr<spdlog::logger>& GetGLLogger() { return gl_logger; }

	private:
		inline static std::shared_ptr<spdlog::logger> acft_logger;
		inline static std::shared_ptr<spdlog::logger> gl_logger;
	};
}

export template<typename ...Args>
void ACFT_LOG_ERROR(spdlog::format_string_t<Args...> fmt, Args&&... args)
{
	::ACFT::Logger::GetACFTLogger()->error(fmt, std::forward<Args>(args)...);
}

export template<typename ...Args>
void ACFT_LOG_WARN(spdlog::format_string_t<Args...> fmt, Args&&... args)
{
	::ACFT::Logger::GetACFTLogger()->warn(fmt, std::forward<Args>(args)...);
}

export template<typename ...Args>
void ACFT_LOG_INFO(spdlog::format_string_t<Args...> fmt, Args&&... args)
{
	::ACFT::Logger::GetACFTLogger()->info(fmt, std::forward<Args>(args)...);
}

export template<typename ...Args>
void ACFT_LOG_TRACE(spdlog::format_string_t<Args...> fmt, Args&&... args)
{
	::ACFT::Logger::GetACFTLogger()->trace(fmt, std::forward<Args>(args)...);
}

export template<typename ...Args>
void ACFT_LOG_FATAL(spdlog::format_string_t<Args...> fmt, Args&&... args)
{
	::ACFT::Logger::GetACFTLogger()->error(fmt, std::forward<Args>(args)...);
}

export template<typename ...Args>
void ACFT_GL_LOG(spdlog::format_string_t<Args...> fmt, Args&&... args)
{
	::ACFT::Logger::GetGLLogger()->warn(fmt, std::forward<Args>(args)...);
}