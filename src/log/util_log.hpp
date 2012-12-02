#ifndef __UTILLOG_H__
#define __UTILLOG_H__

#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

namespace kxutil4 {
class UtilLog {
public:
    enum UtilLogLevel {
        UTIL_LEVEL_TRACE = 1,
        UTIL_LEVEL_DEBUG,
        UTIL_LEVEL_INFO,
        UTIL_LEVEL_WARN,
        UTIL_LEVEL_ERROR,
        UTIL_LEVEL_FATAL,
        UTIL_LEVEL_NOLOG
    };

    UtilLog(UtilLogLevel level=UTIL_LEVEL_ERROR): log_level_(level) {}

    virtual ~UtilLog() {}

    void LogMessage(const char* message, const char* func="") {
        if (strlen(func) == 0)
            cout << message << endl;
        else
            cout << "[" << func << "] " << message << endl;
    }

    UtilLogLevel get_log_level() {
        return log_level_;
    }

    void set_log_level(UtilLogLevel level) {
        log_level_ = level;
    }

    void set_log_level_trace() {
        log_level_ = UTIL_LEVEL_TRACE;
    }

    void set_log_level_debug() {
        log_level_ = UTIL_LEVEL_DEBUG;
    }

    void set_log_level_info() {
        log_level_ = UTIL_LEVEL_INFO;
    }

    void set_log_level_warn() {
        log_level_ = UTIL_LEVEL_WARN;
    }

    void set_log_level_error() {
        log_level_ = UTIL_LEVEL_ERROR;
    }

    void set_log_level_fatal() {
        log_level_ = UTIL_LEVEL_FATAL;
    }

    bool IsEnabledFor(UtilLogLevel level) {
        if (log_level_ <= level)
            return true;
        return false;
    }

    bool IsTraceEnabled() {
        return IsEnabledFor(UtilLog::UTIL_LEVEL_TRACE);
    }

    bool IsDebugEnabled() {
        return IsEnabledFor(UtilLog::UTIL_LEVEL_DEBUG);
    }

    bool IsInfoEnabled() {
        return IsEnabledFor(UtilLog::UTIL_LEVEL_INFO);
    }

    bool IsWarnEnabled() {
        return IsEnabledFor(UtilLog::UTIL_LEVEL_WARN);
    }

    bool IsErrorEnabled() {
        return IsEnabledFor(UtilLog::UTIL_LEVEL_ERROR);
    }

    bool IsFatalEnabled() {
        return IsEnabledFor(UtilLog::UTIL_LEVEL_FATAL);
    }

protected:
    UtilLogLevel log_level_;
};

#define UTIL_LOG_TRACE(msg) do {\
    if (UtilLog::IsTraceEnabled()) \
            UtilLog::LogMessage(msg,__func__); } while (0)

#define UTIL_LOG_DEBUG(msg) do {\
    if (UtilLog::IsDebugEnabled()) \
            UtilLog::LogMessage(msg,__func__); } while (0)

#define UTIL_LOG_INFO(msg) do {\
    if (UtilLog::IsInfoEnabled()) \
            UtilLog::LogMessage(msg,__func__); } while (0)

#define UTIL_LOG_WARN(msg) do {\
    if (UtilLog::IsWarnEnabled()) \
            UtilLog::LogMessage(msg,__func__); } while (0)

#define UTIL_LOG_ERROR(msg) do {\
    if (UtilLog::IsErrorEnabled()) \
            UtilLog::LogMessage(msg,__func__); } while (0)

#define UTIL_LOG_FATAL(msg) do {\
    if (UtilLog::IsFatalEnabled()) \
            UtilLog::LogMessage(msg,__func__); } while (0)


#define UTIL_LOG_BUF_LENGTH 2048

#define UTIL_LOG_TRACE_FMT(fmt, ...) do {\
    if (UtilLog::IsTraceEnabled()) { \
    char _UTIL_LOG_BUF_[UTIL_LOG_BUF_LENGTH]; \
        snprintf(_UTIL_LOG_BUF_, UTIL_LOG_BUF_LENGTH, fmt, __VA_ARGS__); \
            UTIL_LOG_TRACE(_UTIL_LOG_BUF_); } } while (0)

#define UTIL_LOG_DEBUG_FMT(fmt, ...) do {\
    if (UtilLog::IsDebugEnabled()) { \
    char _UTIL_LOG_BUF_[UTIL_LOG_BUF_LENGTH]; \
        snprintf(_UTIL_LOG_BUF_, UTIL_LOG_BUF_LENGTH, fmt, __VA_ARGS__); \
            UTIL_LOG_DEBUG(_UTIL_LOG_BUF_); } } while (0)

#define UTIL_LOG_INFO_FMT(fmt, ...) do {\
    if (UtilLog::IsInfoEnabled()) { \
    char _UTIL_LOG_BUF_[UTIL_LOG_BUF_LENGTH]; \
        snprintf(_UTIL_LOG_BUF_, UTIL_LOG_BUF_LENGTH, fmt, __VA_ARGS__); \
            UTIL_LOG_INFO(_UTIL_LOG_BUF_); } } while (0)

#define UTIL_LOG_WARN_FMT(fmt, ...) do {\
    if (UtilLog::IsWarnEnabled()) { \
    char _UTIL_LOG_BUF_[UTIL_LOG_BUF_LENGTH]; \
        snprintf(_UTIL_LOG_BUF_, UTIL_LOG_BUF_LENGTH, fmt, __VA_ARGS__); \
            UTIL_LOG_WARN(_UTIL_LOG_BUF_); } } while (0)

#define UTIL_LOG_ERROR_FMT(fmt, ...) do {\
    if (UtilLog::IsErrorEnabled()) { \
    char _UTIL_LOG_BUF_[UTIL_LOG_BUF_LENGTH]; \
        snprintf(_UTIL_LOG_BUF_, UTIL_LOG_BUF_LENGTH, fmt, __VA_ARGS__); \
            UTIL_LOG_ERROR(_UTIL_LOG_BUF_); } } while (0)

#define UTIL_LOG_FATAL_FMT(fmt, ...) do {\
    if (UtilLog::IsFatalEnabled()) { \
    char _UTIL_LOG_BUF_[UTIL_LOG_BUF_LENGTH]; \
        snprintf(_UTIL_LOG_BUF_, UTIL_LOG_BUF_LENGTH, fmt, __VA_ARGS__); \
            UTIL_LOG_FATAL(_UTIL_LOG_BUF_); } } while (0)

} // namespace kxutil4
#endif // __UTILLOG_H__
