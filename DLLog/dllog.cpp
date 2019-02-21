/////////////////////////////////////
// File name : dllog.c
// Describe : not set
////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <stdarg.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include "dllog.h"

//DLlog default config begin
#define IS_SHOW_LOG 1
#define IS_WRITE_LOG 1
#define DL_LOG_MODE log_mode_develop
#define LOG_FILE_PATH "./dllog/"
#define LOG_FILE_NAME "dllog"
#define LOG_PRINT_LEVEL log_level_debug
#define LOG_WRITE_LEVEL log_level_warning
#define IS_PRINT_LINE_BREAK 0
#define LOG_OPEN_ASSERT_LEVEL log_level_warning
#define IS_OPEN_ASSERT 0
//DLlog default config end

#define DL_LOG(str) printf("LEVEL:error     FILE:%s     FUNC:%s     LINE:%d     DESCRIPTION:%s", __FILE__,__FUNCTION__,__LINE__,str)

static int globalIsShowLog = IS_SHOW_LOG;
static int globalIsWriteLog = IS_WRITE_LOG;
static log_mode_type globalLogMode = DL_LOG_MODE;
static char* globalLogFilePath = LOG_FILE_PATH;
static char* globalLogFileName = LOG_FILE_NAME;
static log_level_type globalLogPrintLevel = LOG_PRINT_LEVEL;
static log_level_type globalLogWriteLevel = LOG_WRITE_LEVEL;
static log_level_type globalOpenAssertLevel = LOG_OPEN_ASSERT_LEVEL;
static int globalIsPrintLineBreak = IS_PRINT_LINE_BREAK;
static int globalIsOpenAssert = IS_OPEN_ASSERT;

static void _FreeMemory(void** ptr){
    if (*ptr == NULL){
        return;
    }else{
        free(*ptr);
        *ptr = NULL;
    }
}

void setIsShowLog(int isShowLog){
    globalIsShowLog = isShowLog;
}

static int _getIsShowLog(){
    return globalIsShowLog;
}

void setIsWriteLog(int isWriteLog){
    globalIsWriteLog = isWriteLog;
}

static int _getIsWriteLog(){
    return globalIsWriteLog;
}

void setLogPrintLevel(log_level_type logLevel){
    globalLogPrintLevel = logLevel;
}

static log_level_type _getLogPrintLevel(){
    return globalLogPrintLevel;
}

void setLogWriteLevel(log_level_type logLevel){
    globalLogWriteLevel = logLevel;
}

static log_level_type _getLogWriteLevel(){
    return globalLogWriteLevel;
}

void setOpenAssertLevel(log_level_type logLevel){
    globalOpenAssertLevel = logLevel;
}

static log_level_type _getOpenAssertLevel(){
    return globalOpenAssertLevel;
}

void setLogFilePath(const char* filePath){
    globalLogFilePath = (char*)filePath;
}

static char* _getLogFilePath(){
    return globalLogFilePath;
}

void setLogMode(log_mode_type logMode){
    globalLogMode = logMode;
}

static log_mode_type _getLogMode(){
    return globalLogMode;
}

void setLogFileName(const char* fileName){
    globalLogFileName = (char*)fileName;
}

static char* _getLogFileName(){
    return globalLogFileName;
}

void setIsPrintLineBreak(int isPrintLineBreak){
    globalIsPrintLineBreak = isPrintLineBreak;
}

static int _getIsPrintLineBreak(){
    return globalIsPrintLineBreak;
}

void setIsOpenAssert(int isOpenAssert){
    globalIsOpenAssert = isOpenAssert;
}

static int _getIsOpenAssert(){
    return globalIsOpenAssert;
}

static char* _getLogLevelStr(int log_level){
    char* log_level_str;
    switch (log_level){
        case log_level_debug:
            log_level_str = "debug";
            break;
        case log_level_info:
            log_level_str = "info";
            break;
        case log_level_warning:
            log_level_str = "warning";
            break;
        case log_level_null:
            log_level_str = "null";
            break;
        case log_level_error:
            log_level_str = "error";
            break;
        case log_level_syserror:
            log_level_str = "syserror";
            break;
        case log_level_fatal:
            log_level_str = "fatal";
            break;
        default:
            log_level_str = "unknown";
            DL_LOG("undifined log_level");
            }
    return log_level_str;
}

static char* _mallocBuffer(int size){
    char* bufferPointer = (char*)malloc(size);
    if (bufferPointer == NULL)
        DL_LOG("malloc fail\n");
    return bufferPointer;
}

static char* _mallocStrFormat(const char* str,...){
    va_list argptr;
    int needreadcount;
    int strbuffersize = 1024;
    char* strbuffer = _mallocBuffer(strbuffersize);
    va_start(argptr, str);
    needreadcount = vsnprintf(strbuffer,strbuffersize,str, argptr);
    va_end(argptr);

    if (needreadcount >= strbuffersize){
        DL_LOG("re malloc");
        _FreeMemory((void**)&strbuffer);
        int size = needreadcount + 1;
        char* strbufferNeed = _mallocBuffer(size);
        va_start(argptr, str);
        vsnprintf(strbufferNeed,size,str, argptr);
        va_end(argptr);
        return strbufferNeed;
    }
    return strbuffer;
}

static char* _getCurrentDay(){
    time_t timeStamp;
    time(&timeStamp);
    int dayFormatSize = 11;
    struct tm* timeInfo = localtime(&timeStamp);
    int currentYear = timeInfo->tm_year + 1900;
    int currentMon = timeInfo->tm_mon + 1;
    char* dayFormat = _mallocStrFormat("%d-%d-%d",currentYear,currentMon,timeInfo->tm_mday);
    return dayFormat;
}

static void _createFilePath(const char* filePath){
    DIR* dirptr = NULL;
    struct dirent* entry;
    dirptr = opendir(filePath);
    if (dirptr == NULL){
        int status = dlcpMkdir(filePath);
        if (status == -1){
            DL_LOG("");
            printf("dir:%s create dir fail errno:%d errordesc:%s \n", filePath,errno,strerror(errno));
        }
    }else{
        closedir(dirptr);
    }
}

static char* _getLogFileFullPath(){
    char* dayFormat = _getCurrentDay();
    char* filePath = _getLogFilePath();
    char* fileName = _getLogFileName();
    _createFilePath(filePath);
    char* logFileFullPath = _mallocStrFormat("./%s%s%s.dllog",filePath,fileName,dayFormat);
    _FreeMemory((void**)&dayFormat);
    return logFileFullPath;
}

static void _writeToFile(const char* text){
    FILE* filePointer;
    char* logFileFullPath = _getLogFileFullPath();
    char* openStyle = "a";
    filePointer = fopen(logFileFullPath,openStyle);
    fputs(text,filePointer);
    fclose(filePointer);
    _FreeMemory((void**)&logFileFullPath);
}

static void _writeLogToFile(int log_level, const char* str, int line, const char* func, const char* file, const char* log_level_str){
    char* fileCache = _mallocStrFormat("LEVEL:%-10sLINE:%-10dFILE:%s   FUNC:%s   DESCRIPTION:%s\n",log_level_str, line, file, func, str);
    _writeToFile(fileCache);
    _FreeMemory((void**)&fileCache);
}

static void _dealLogModeRelease(int log_level, const char* str, int line, const char* func, const char* file, const char* log_level_str){
    printf("%s\n",str);
}

static void _dealLogModeDevelop(int log_level, const char* str, int line, const char* func, const char* file, const char* log_level_str){
    if (_getIsPrintLineBreak()){
        printf("LEVEL:%s\nFILE:%s\nFUNC:%s\nLINE:%d\nDESCRIPTION:%s\n", log_level_str, file, func, line, str);
    }else{
        printf("LEVEL:%-10sFILE:%s   FUNC:%s     LINE:%d     DESCRIPTION:%s\n", log_level_str, file, func, line, str);
    }
    //Flush STREAM,Force a refresh to the console
    fflush(stdout);
}

static void _dealLogModeDebug(int log_level, const char* str, int line, const char* func, const char* file, const char* log_level_str){
    if (log_level > log_level_warning){
        _dealLogModeDevelop(log_level, str, line, func, file, log_level_str);
    }else{
        _dealLogModeRelease(log_level, str, line, func, file, log_level_str);
    }
}

static void _checkLogMode(int log_level, const char* str, int line, const char* func, const char* file, const char* log_level_str){
    int log_mode = _getLogMode();
    switch (log_mode){
        case log_mode_release:
            _dealLogModeRelease(log_level, str, line, func, file, log_level_str);
            break;
        case log_mode_debug:
            _dealLogModeDebug(log_level, str, line, func, file, log_level_str);
            break;
        case log_mode_develop:
            _dealLogModeDevelop(log_level, str, line, func, file, log_level_str);
            break;
        default:
            DL_LOG("unkown log mode");
        }
}

static int _checkIsSystemError(log_level_type log_level, const char** str){
    int isSysError = 0;
    if (log_level == log_level_syserror){
        char* syserrorStr = _mallocStrFormat("%s   system errno:%d system error description:%s", *str, errno, strerror(errno));
        *str = syserrorStr;
        isSysError = 1;
    }
    return isSysError;
}

static void __FreeMemorySysErrorStr(const char* str, int isSysError){
    if (isSysError)
        _FreeMemory((void**)&str);
}

static void _checkLogLevelShowBell(int log_level){
    if (log_level > log_level_warning)
        printf("\007\007\007\007\007\007");
}

static void _checkIsAssertAbort(int log_level){
    if (_getLogMode() == log_mode_develop)
        if (log_level > _getOpenAssertLevel())
            if (_getIsOpenAssert())
                assert(0);
}

static void _DLlog(log_level_type log_level, int line, const char* func, const char* file,const char* str){
    char* log_level_str = _getLogLevelStr(log_level);

    if (_getIsShowLog()){
        if (log_level >= _getLogPrintLevel()){
            int isSysError = _checkIsSystemError(log_level, &str);
            _checkLogLevelShowBell(log_level);
            _checkLogMode(log_level, str, line, func, file, log_level_str);
            __FreeMemorySysErrorStr(str, isSysError);
        }
    }

    if (_getIsWriteLog()){
        if (log_level >= _getLogWriteLevel()){
            _writeLogToFile(log_level, str, line, func, file, log_level_str);
        }
    }

    _checkIsAssertAbort(log_level);
}

void DLlogFormat(log_level_type log_level, int line, const char* func, const char* file,const char* str,...){
    va_list argptr;
    int needreadcount;
    int strbuffersize = 1024;
    char* strbuffer = _mallocBuffer(strbuffersize);
    va_start(argptr, str);
    needreadcount = vsnprintf(strbuffer, strbuffersize, str, argptr);
    va_end(argptr);
    if (needreadcount >= strbuffersize){
        printf("re malloc Func:%s Line:%d\n", __FUNCTION__, __LINE__);
        _FreeMemory((void**)&strbuffer);
        int size = needreadcount + 1;
        char* strbufferNeed = _mallocBuffer(size);
        va_start(argptr, str);
        vsnprintf(strbufferNeed, size, str, argptr);
        va_end(argptr);
        str = strbufferNeed;
    }else{
        str = strbuffer;
    }

    _DLlog(log_level, line, func, file, str);
    _FreeMemory((void**)&str);
}

int dlcpMkdir(const char* filePath){
    //windows
    #ifdef __WIN32
        int status = mkdir(filePath);
        return status;
    #endif

    //linux
    #ifdef __linux__
        int status = mkdir(filePath,S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH);
        return status;
    #endif

    //mac

    #ifdef __APPLE__
        int status = mkdir(filePath,S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH);
        return status;
    #endif
    //#ifdef __llvm__
}
