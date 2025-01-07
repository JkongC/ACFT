#include "stdafx.h"

#include "Logger.h"

namespace ACFT
{
	std::shared_ptr<spdlog::logger> Logger::acft_logger;
	std::shared_ptr<spdlog::logger> Logger::gl_logger;
	
	void Logger::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		acft_logger = spdlog::stdout_color_mt("AnotherCraft");
		gl_logger = spdlog::stdout_color_mt("OpenGL");

		spdlog::set_level(spdlog::level::trace);
	}
}