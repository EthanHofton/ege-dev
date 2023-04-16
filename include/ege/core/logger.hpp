#ifndef __EGE_LOGGER_HPP__
#define __EGE_LOGGER_HPP__

#include <util/logger.hpp>
#include <util/logger_console_sink.hpp>

namespace ege {

class ege_logger {
private:

    inline ege_logger()
    { m_logger.addSink<util::logger_console_sink>("ege_console_logger"); m_logger.logInfo("Logger initialized"); }

public:

    inline ~ege_logger() { m_logger.logInfo("Logger destroyed"); }

    inline static ege_logger& get() {
        static ege_logger instance;
        return instance;
    }

    ege_logger(ege_logger const&) = delete;
    void operator=(ege_logger const&) = delete;

    /**
    * @brief logger instance
    */
    util::logger m_logger;
};

// logger macros
#define EGE_TRACE(...)    ege::ege_logger::get().m_logger.logTrace(__VA_ARGS__)
#define EGE_DEBUG(...)    ege::ege_logger::get().m_logger.logDebug(__VA_ARGS__)
#define EGE_INFO(...)     ege::ege_logger::get().m_logger.logInfo(__VA_ARGS__)
#define EGE_WARN(...)     ege::ege_logger::get().m_logger.logWarning(__VA_ARGS__)
#define EGE_ERROR(...)    ege::ege_logger::get().m_logger.logError(__VA_ARGS__)
#define EGE_CRITICAL(...) ege::ege_logger::get().m_logger.logCritical(__VA_ARGS__)

}

#endif // __EGE_LOGGER_HPP__
