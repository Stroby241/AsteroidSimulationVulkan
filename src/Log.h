#pragma once
#include<memory>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

class Log
{
public:
	static void Init();

	inline static std::shared_ptr<spdlog::logger>& GetLogger() { return s_Logger; }
	inline static std::shared_ptr<spdlog::logger>& GetVKLogger() { return s_VKLogger; }

private:
	static std::shared_ptr<spdlog::logger> s_Logger;
	static std::shared_ptr<spdlog::logger> s_VKLogger;
};

#define VK_LOG_ERROR(...) ::Log::GetVKLogger()->error(__VA_ARGS__)
#define VK_LOG_WARN(...) ::Log::GetVKLogger()->warn(__VA_ARGS__)
#define VK_LOG_INFO(...) ::Log::GetVKLogger()->info(__VA_ARGS__)
#define VK_LOG_TRACE(...) ::Log::GetVKLogger()->trace(__VA_ARGS__)


#define LOG_ERROR(...) ::Log::GetLogger()->error(__VA_ARGS__)
#define LOG_WARN(...) ::Log::GetLogger()->warn(__VA_ARGS__)
#define LOG_INFO(...) ::Log::GetLogger()->info(__VA_ARGS__)
#define LOG_TRACE(...) ::Log::GetLogger()->trace(__VA_ARGS__)
