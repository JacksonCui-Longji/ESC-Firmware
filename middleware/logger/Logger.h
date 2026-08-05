#ifndef LOGGER_H
#define LOGGER_H

#include <stdint.h>

typedef enum LogLevel_t
{
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_MAX
}LogLevel;

extern void Logger_Init(void);

extern void Logger_Print(
        LogLevel level,
        const char *module,
        const char *file,
        uint32_t line,
        const char *fmt,
        ...);

#endif // LOGGER_H
