#include "Log.h"


std::shared_ptr<spdlog::logger> Log::s_Logger;
std::shared_ptr<spdlog::logger> Log::s_VKLogger;
void Log::Init()
{
	spdlog::set_pattern("%^[%H:%M:%S] [%n] %v%$");
	spdlog::set_level(spdlog::level::trace);
	s_Logger = spdlog::stdout_color_mt("Main");
	s_VKLogger = spdlog::stdout_color_mt("Vulkan");
}