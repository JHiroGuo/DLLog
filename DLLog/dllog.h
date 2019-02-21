/////////////////////////////////////
// File name : dllog.h
// Describe : 支持日志分级格式化打印输出 模式输出 持久化 错误日志断言 自动化测试 跨平台
////////////////////////////////////

#ifndef _DLlog_H
#define _DLlog_H
#include <assert.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define DEBUG_LOG(str,...) DLlogFormat(log_level_debug,__LINE__,__FUNCTION__,__FILE__,str,##__VA_ARGS__)
#define INFO_LOG(str,...) DLlogFormat(log_level_info,__LINE__,__FUNCTION__,__FILE__,str,##__VA_ARGS__)
#define FUNC_RUN_LOG DLlogFormat(log_level_info,__LINE__,__FUNCTION__,__FILE__,"function is run")
#define WARNING_LOG(str,...) DLlogFormat(log_level_warning,__LINE__,__FUNCTION__,__FILE__,str,##__VA_ARGS__)
#define CONDITION_VALUE_IS_NULL DLlogFormat(log_level_null,__LINE__,__FUNCTION__,__FILE__,"condition value is null")
#define ERROR_LOG(str,...) DLlogFormat(log_level_error,__LINE__,__FUNCTION__,__FILE__,str,##__VA_ARGS__)
#define SYSERROR_LOG(str,...) DLlogFormat(log_level_syserror,__LINE__,__FUNCTION__,__FILE__,str,##__VA_ARGS__)
#define FATAL_LOG(str,...) DLlogFormat(log_level_fatal,__LINE__,__FUNCTION__,__FILE__,str,##__VA_ARGS__)

#define DL_TEST_COMPARE_VALUE_INT(real_value, expected_value) \
        if (expected_value == real_value){\
            DEBUG_LOG("variable: %s real value:%d expected value:%d",#real_value,real_value,expected_value);\
        }else{\
            ERROR_LOG("variable: %s real value:%d expected value:%d",#real_value,real_value,expected_value);\
        }\

#define DL_TEST_COMPARE_VALUE_STR(real_value, expected_value) \
        if (strcmp(real_value,expected_value) == 0){\
            DEBUG_LOG("variable: %s real value:%s expected value:%s",#real_value,real_value,expected_value);\
        }else{\
            ERROR_LOG("variable: %s real value:%s expected value:%s",#real_value,real_value,expected_value);\
        }\


#define DL_TEST_COMPARE_VALUE(real_value, expected_value,value_type) \
        if ("str" == value_type){\
            if (strcmp(real_value,expected_value) == 0){\
                DEBUG_LOG("variable: %s real value:%s expected value:%s",#real_value,real_value,expected_value);\
            }else{\
                ERROR_LOG("variable: %s real value:%s expected value:%s",#real_value,real_value,expected_value);\
            }\
        }else if("int"== value_type){\
            if (expected_value == real_value){\
                DEBUG_LOG("variable: %s real value:%d expected value:%d",#real_value,real_value,expected_value);\
            }else{\
                ERROR_LOG("variable: %s real value:%d expected value:%d",#real_value,real_value,expected_value);\
            }\
        }\

#define DL_GET_LIST_ENTRY(ptr,type,member) ((type *)((char *)(ptr) - (char *)(&(((type *)0)->member))))

#define DL_MARGINAL_TEST 0

typedef enum
{
    log_level_debug = 1,
    log_level_info,
    log_level_warning,
    log_level_null,
    log_level_error,
    log_level_syserror, //sys calls error,show system error description
    log_level_fatal,
} log_level_type;

typedef enum
{
    log_mode_release = 1,   //show run step log
    log_mode_debug,         //show log description info. log_level > warning then show detail
    log_mode_develop,       //show detail of the log info
}log_mode_type;

void DLlogFormat(log_level_type log_level, int line, const char* func, const char* file, const char* str,...);

int dlcpMkdir(const char* filePath);

void setIsShowLog(int isShowLog);
void setIsWriteLog(int isWriteLog);
void setLogPrintLevel(log_level_type logPrintLevel);
void setLogWriteLevel(log_level_type logWriteLevel);
void setLogFilePath(const char* logFilePath);
void setLogMode(log_mode_type logMode);
void setLogFileName(const char* logFileName);
void setIsPrintLineBreak(int isPrintLineBreak);
void setIsOpenAssert(int isOpenAssert);
void setOpenAssertLevel(log_level_type logOpenAssertLevel);

#endif

