#ifndef LOGGER_MACRO_H
#define LOGGER_MACRO_H


#include "Logger.h"


#ifndef LOG_MODULE
#error "Please define LOG_MODULE before using logger"
#endif


#define LOG_DEBUG(fmt, ...) \
    Logger_Print( \
        LOG_LEVEL_DEBUG, \
        LOG_MODULE, \
        __FILE__, \
        __LINE__, \
        fmt, \
        ##__VA_ARGS__)


#define LOG_INFO(fmt, ...) \
    Logger_Print( \
        LOG_LEVEL_INFO, \
        LOG_MODULE, \
        __FILE__, \
        __LINE__, \
        fmt, \
        ##__VA_ARGS__)

#define LOG_WARN(fmt, ...) \
    Logger_Print( \
        LOG_LEVEL_WARN, \
        LOG_MODULE, \
        __FILE__, \
        __LINE__, \
        fmt, \
        ##__VA_ARGS__)

#define LOG_ERROR(fmt, ...) \
    Logger_Print( \
        LOG_LEVEL_ERROR, \
        LOG_MODULE, \
        __FILE__, \
        __LINE__, \
        fmt, \
        ##__VA_ARGS__)
#endif