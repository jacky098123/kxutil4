/*
 * we use KXHLOG as prefix, it means kuxun hotel log
 * usage:
 *     1. KXHLOG_INIT(log_confg_file_name)
 *     2. KHXLOG_DEBUG(log_name, xxxx)
 */

#ifndef __KXUTIL3_KXLOG_HPP__
#define __KXUTIL3_KXLOG_HPP__

#include "log4cplus/logger.h"
#include "log4cplus/configurator.h"

namespace kxutil4 {
#define KXHLOG_BUF_SIZE 1024*2

#ifndef DISABLE_KXHLOG
#define KXHLOG_INIT(config_name) log4cplus::PropertyConfigurator::doConfigure(config_name);

#define KXHLOG_TRACE(log_name,msg) do { \
	log4cplus::Logger logger = log4cplus::Logger::getInstance(log_name);\
	LOG4CPLUS_TRACE(logger, __func__ << ": " << msg); } while(0)

#define KXHLOG_DEBUG(log_name,msg) do { \
    log4cplus::Logger logger = log4cplus::Logger::getInstance(log_name);\
	LOG4CPLUS_DEBUG(logger, __func__ << ": " << msg); } while(0)

#define KXHLOG_INFO(log_name,msg) do { \
	log4cplus::Logger logger = log4cplus::Logger::getInstance(log_name);\
	LOG4CPLUS_INFO(logger, __func__ << ": " << msg); } while(0)

#define KXHLOG_WARN(log_name,msg) do { \
	log4cplus::Logger logger = log4cplus::Logger::getInstance(log_name);\
	LOG4CPLUS_WARN(logger, __func__ << ": " << msg); } while(0)

#define KXHLOG_ERROR(log_name,msg) do { \
	log4cplus::Logger logger = log4cplus::Logger::getInstance(log_name);\
	LOG4CPLUS_ERROR(logger, __func__ << ": " << msg); } while(0)

#define KXHLOG_FATAL(log_name,msg) do { \
	log4cplus::Logger logger = log4cplus::Logger::getInstance(log_name);\
	LOG4CPLUS_FATAL(logger, __func__ << ": " << msg); } while(0)

#else

#define KXHLOG_INIT(config_name) 
#define KXHLOG_TRACE(log_name,msg)
#define KXHLOG_DEBUG(log_name,msg)
#define KXHLOG_INFO(log_name,msg)
#define KXHLOG_WARN(log_name,msg)
#define KXHLOG_ERROR(log_name,msg)
#define KXHLOG_FATAL(log_name,msg)

#endif

#define KXHLOG_TRACE_FMT(logger_name,fmt,...) do { \
	char _KXHLOG_BUF_[KXHLOG_BUF_SIZE]; \
	snprintf(_KXHLOG_BUF_,KXHLOG_BUF_SIZE,fmt,__VA_ARGS__); \
	KXHLOG_TRACE(logger_name, _KXHLOG_BUF_); } while(0)

#define KXHLOG_DEBUG_FMT(logger_name,fmt,...) do { \
	char _KXHLOG_BUF_[KXHLOG_BUF_SIZE]; \
	snprintf(_KXHLOG_BUF_,KXHLOG_BUF_SIZE,fmt,__VA_ARGS__); \
	KXHLOG_DEBUG(logger_name, _KXHLOG_BUF_); } while(0)

#define KXHLOG_INFO_FMT(logger_name,fmt,...) do { \
	char _KXHLOG_BUF_[KXHLOG_BUF_SIZE]; \
	snprintf(_KXHLOG_BUF_,KXHLOG_BUF_SIZE,fmt,__VA_ARGS__); \
	KXHLOG_INFO(logger_name, _KXHLOG_BUF_); } while(0)

#define KXHLOG_WARN_FMT(logger_name,fmt,...) do { \
	char _KXHLOG_BUF_[KXHLOG_BUF_SIZE]; \
	snprintf(_KXHLOG_BUF_,KXHLOG_BUF_SIZE,fmt,__VA_ARGS__); \
	KXHLOG_WARN(logger_name, _KXHLOG_BUF_); } while(0)

#define KXHLOG_ERROR_FMT(logger_name,fmt,...) do { \
	char _KXHLOG_BUF_[KXHLOG_BUF_SIZE]; \
	snprintf(_KXHLOG_BUF_,KXHLOG_BUF_SIZE,fmt,__VA_ARGS__); \
	KXHLOG_ERROR(logger_name, _KXHLOG_BUF_); } while(0)

#define KXHLOG_FATAL_FMT(logger_name,fmt,...) do { \
	char _KXHLOG_BUF_[KXHLOG_BUF_SIZE]; \
	snprintf(_KXHLOG_BUF_,KXHLOG_BUF_SIZE,fmt,__VA_ARGS__); \
	KXHLOG_FATAL(logger_name, _KXHLOG_BUF_); } while(0)

}; // namespace kxutil4
#endif // __KXUTIL3_KXLOG_HPP__
