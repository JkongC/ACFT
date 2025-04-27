module;

#define SPDLOG_COMPILED_LIB
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

module Log;

namespace ACFT
{
	void Logger::Init()
	{
		spdlog::set_pattern("%^[%T] [%n] %v%$");
		acft_logger = spdlog::stdout_color_mt("AnotherCraft");
		gl_logger = spdlog::stdout_color_mt("OpenGL");

		spdlog::set_level(spdlog::level::trace);
	}
}