#include "Logger.h"
#include "BSP_UART.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "SystemTime.h"
#include <inttypes.h>

static const char *LogLevel_String[LOG_LEVEL_MAX] =
{
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
};

static const char *Logger_GetFileName(const char *path)
{
    const char *name1 = strrchr(path, '/');
    const char *name2 = strrchr(path, '\\');

    if(name1 && name2)
    {
        return (name1 > name2) ? name1 + 1 : name2 + 1;
    }

    if(name1)
    {
        return name1 + 1;
    }

    if(name2)
    {
        return name2 + 1;
    }

    return path;
}

void Logger_Init(void)
{

}

void Logger_Print(
        LogLevel level,
        const char *module,
        const char *file,
        uint32_t line,
        const char *fmt,
        ...)
{
    char buffer[128];

    va_list args;

    va_start(args, fmt);
    vsnprintf(
        buffer,
        sizeof(buffer),
        fmt,
        args
    );
    va_end(args);

    char log_buffer[256];
    uint32_t systemtime = SystemTime_GetMs();

    if(LOG_LEVEL_MAX <= level)
    {
        snprintf(
            log_buffer,
            sizeof(log_buffer),
            "[T=%" PRIu32 "ms][%s][%s][%s:%d] %s:%d\r\n",
            systemtime,
            LogLevel_String[LOG_LEVEL_ERROR],
            module,
            Logger_GetFileName(file),
            line,
            "Log level oversize.",
            level
        );
    }
    else
    {
        snprintf(
            log_buffer,
            sizeof(log_buffer),
            "[T=%" PRIu32 "ms][%s][%s][%s:%d] %s\r\n",
            systemtime,
            LogLevel_String[level],
            module,
            Logger_GetFileName(file),
            line,
            buffer
        );
    }

    BSP_UART_Send((uint8_t *)log_buffer, strlen(log_buffer));
}

