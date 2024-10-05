#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

//-------------------------------------------------
// Log
//-------------------------------------------------

/**
 * @brief Provides logging functionality using the spdlog library.
 */
class Log
{
public:
    //-------------------------------------------------
    // Init
    //-------------------------------------------------

    static void Init()
    {
        spdlog::set_pattern("%^[%T] %n: %v%$");

        m_logger = spdlog::stdout_color_mt("Shooter");

#ifdef FPS_DEBUG_BUILD
        m_logger->set_level(spdlog::level::trace);
#else
        m_logger->set_level(spdlog::level::info);
#endif
    }

    //-------------------------------------------------
    // Getter
    //-------------------------------------------------

    static std::shared_ptr<spdlog::logger>& GetLogger()
    {
        return m_logger;
    }

protected:

private:
    //-------------------------------------------------
    // Member
    //-------------------------------------------------

    inline static std::shared_ptr<spdlog::logger> m_logger;
};

#define FPS_LOG_DEBUG(...)    Log::GetLogger()->debug(__VA_ARGS__)
#define FPS_LOG_TRACE(...)    Log::GetLogger()->trace(__VA_ARGS__)
#define FPS_LOG_INFO(...)     Log::GetLogger()->info(__VA_ARGS__)
#define FPS_LOG_WARN(...)     Log::GetLogger()->warn(__VA_ARGS__)
#define FPS_LOG_ERROR(...)    Log::GetLogger()->error(__VA_ARGS__)
#define FPS_LOG_CRITICAL(...) Log::GetLogger()->critical(__VA_ARGS__)
