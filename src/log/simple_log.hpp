#ifndef __SIMPLE_LOG_H__
#define __SIMPLE_LOG_H__

#include <stdarg.h>
#include <string>

namespace kxutil4 {

#define SIMPLE_LOG_INIT(X) do { \
    SimpleLog *magic__ = SimpleLog::Instance(); \
    magic__->Init(X); } while(0);

#define SIMPLE_LOG_TRACE(X) do { \
    SimpleLog *magic__ = SimpleLog::Instance(); \
    magic__->SetTraceInfo(__LINE__,__FILE__); \
    magic__->WriteTrace(X); }while(0);

#define SIMPLE_LOG_TRACE_FMT(X, ...) do { \
    SimpleLog *magic__ = SimpleLog::Instance(); \
    magic__->SetTraceInfo(__LINE__,__FILE__); \
    magic__->WriteTrace(X, __VA_ARGS__); }while(0);


    class SimpleLog {
#define SIMPLE_LOG_BUFFER_SIZE  4*1024
        public:
            virtual ~SimpleLog() {}
            static SimpleLog* Instance(void);

            int Init(const std::string& file,long  logsize = 100*1024*1024, int lognum = 5);

            /** 
             * write log with __FILE__, __LINE__, call WriteLog
             */
            void WriteTrace(const char* sFormat,...);

            /**
             * simple write to disk
             */
            void WriteLog(const char* sFormat,...);
            void SetTraceInfo(long lLine,const char*sFile);

        private:
            SimpleLog() {}

            int ShiftFiles(char *sErrMsg, int msg_len);

            char*  GetCurDateTimeStr(char *buf);

            std::string	m_logfile;
            long        	m_lLine;
            std::string 	m_trace_file;
            long 		m_lLogSize;
            int  		m_iLogNum;

            static SimpleLog *instance_;
    };
} // namespace kxutil4
#endif // __SIMPLE_LOG_H__
