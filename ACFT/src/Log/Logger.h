#ifndef ACFT_LOGGER_H_
#define ACFT_LOGGER_H_

namespace ACFT
{
	enum LogPriority
	{
		trace = 0,
		info,
		warn,
		error,
		fatal
	};
	
	class Logger
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetACFTLogger() { return acft_logger; }
		inline static std::shared_ptr<spdlog::logger>& GetGLLogger() { return gl_logger; }

	private:
		static std::shared_ptr<spdlog::logger> acft_logger;
		static std::shared_ptr<spdlog::logger> gl_logger;
	};
}

//Client log macros
#define ACFT_LOG_FATAL(...)		::ACFT::Logger::GetACFTLogger()->fatal(__VA_ARGS__)
#define ACFT_LOG_ERROR(...)		::ACFT::Logger::GetACFTLogger()->error(__VA_ARGS__)
#define ACFT_LOG_WARN(...)		::ACFT::Logger::GetACFTLogger()->warn(__VA_ARGS__)
#define ACFT_LOG_INFO(...)		::ACFT::Logger::GetACFTLogger()->info(__VA_ARGS__)
#define ACFT_LOG_TRACE(...)		::ACFT::Logger::GetACFTLogger()->trace(__VA_ARGS__)

#define ACFT_GL_LOG(...)		::ACFT::Logger::GetGLLogger()->warn(__VA_ARGS__)

#endif
