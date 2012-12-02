#include "simple_log.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <iostream>

using namespace std;
namespace kxutil4 {
    SimpleLog*  SimpleLog::instance_ = NULL;

    SimpleLog* SimpleLog::Instance(void) {
        if(instance_ == NULL) {
            instance_ = new SimpleLog();
        }
        return instance_;
    }

    int SimpleLog::Init(const string& logFile,long lMaxLogSize,int  iMaxLogNum) {
        if(lMaxLogSize < 1 || iMaxLogNum < 1) {
            cerr<<"fail to init SimpleLog."<<endl;
            return -1;
        }
        m_lLogSize = lMaxLogSize;
        m_iLogNum = iMaxLogNum;
        m_logfile = logFile;
        return 0;
    }

    void SimpleLog::SetTraceInfo(long lLine,const char* sFile) {
        m_lLine = lLine;
        if(sFile!=0) 
            m_trace_file = sFile;
    }

    int SimpleLog::ShiftFiles(char *sErrMsg, int msg_len) {
        struct stat stStat;
        char sLogFileName[1024];
        char sNewLogFileName[1024];

        if(stat(m_logfile.c_str(), &stStat) < 0) {
            if (sErrMsg != NULL) {
                snprintf(sErrMsg, msg_len, "Fail to get file status");
            }
            return -1;
        }

        if (stStat.st_size < m_lLogSize) {
            return 0;
        }

        sprintf(sLogFileName,"%s_%d", m_logfile.c_str(), m_iLogNum-1);
        if (access(sLogFileName, F_OK) == 0) {
            if (remove(sLogFileName) < 0 ) {
                if (sErrMsg != NULL) {
                    snprintf(sErrMsg, msg_len, "Fail to get file status");
                }
                return -1;
            }
        }

        for(int i = m_iLogNum-2; i >= 0; i--) {
            if (i == 0)
                sprintf(sLogFileName,"%s", m_logfile.c_str());
            else
                sprintf(sLogFileName,"%s_%d", m_logfile.c_str(), i);

            if (access(sLogFileName, F_OK) == 0) {
                sprintf(sNewLogFileName,"%s_%d", m_logfile.c_str(), i+1);
                if (rename(sLogFileName,sNewLogFileName) < 0 ) {
                    if (sErrMsg != NULL) {
                        snprintf(sErrMsg, msg_len, "Fail to get file status");
                    }
                    return -1;
                }
            }
        }
        return 0;
    }

    void SimpleLog::WriteTrace(const char*sFormat,...) {
        va_list ap;
        int     ret=0;
        char buf[SIMPLE_LOG_BUFFER_SIZE];
        char time_buf[SIMPLE_LOG_BUFFER_SIZE];

        va_start(ap,sFormat);    
        memset(buf, 0x00, sizeof(buf));
        ret =  snprintf(buf, sizeof(buf), "[%s]\t[%s:%ld] ", 
                GetCurDateTimeStr(time_buf), m_trace_file.c_str(),m_lLine);
        ret += vsnprintf(buf+ret, sizeof(buf)-ret, sFormat,ap);
        va_end(ap);

        WriteLog("%s",buf);
    }

    void SimpleLog::WriteLog(const char *sFormat, ...) {
        va_list ap;
        FILE*   fp;

        if ( (fp=fopen(m_logfile.c_str(), "a+")) == NULL ) {
            return ;
        }

        va_start(ap, sFormat);
        vfprintf(fp, sFormat, ap);
        fwrite("\n", 1, 1, fp);
        va_end(ap);	
        fclose(fp);
    }

    char *SimpleLog::GetCurDateTimeStr(char *buf) {
        time_t	iCurTime;
        time(&iCurTime);
        struct tm *curr = localtime(&iCurTime);

        if (curr->tm_year > 50) {
            sprintf(buf,   "%04d-%02d-%02d %02d:%02d:%02d", 
                    curr->tm_year+1900, curr->tm_mon+1, curr->tm_mday,
                    curr->tm_hour, curr->tm_min, curr->tm_sec);
        }
        else {
            sprintf(buf,   "%04d-%02d-%02d %02d:%02d:%02d", 
                    curr->tm_year+2000, curr->tm_mon+1, curr->tm_mday,
                    curr->tm_hour, curr->tm_min, curr->tm_sec);
        }
        return buf;
    }
} // namespace kxutil4
