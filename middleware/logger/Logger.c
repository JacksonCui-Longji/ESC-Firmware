#include "Logger.h"
#include "BSP_UART.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "SystemTime.h"
#include <inttypes.h>

#define LOGGER_MSG_BUFFER_SIZE      (0x80u)
#define LOGGER_OUTPUT_BUFFER_SIZE   (0x100u)

static const char *LogLevel_String[LOG_LEVEL_MAX] =
{
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
};

static LogLevel g_cfg_runtime_log_level = LOG_LEVEL_DEBUG;

// get current outputing file name.
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
    g_cfg_runtime_log_level = LOG_LEVEL_ERROR;
}

void Logger_Print(
        LogLevel level,
        const char *module,
        const char *file,
        uint32_t line,
        const char *fmt,
        ...)
{

    if(level < g_cfg_runtime_log_level)
    {
        return;
    }

    char buffer[LOGGER_MSG_BUFFER_SIZE];

    va_list args;

    va_start(args, fmt);
    vsnprintf(
        buffer,
        sizeof(buffer),
        fmt,
        args
    );
    va_end(args);

    char log_buffer[LOGGER_OUTPUT_BUFFER_SIZE];
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

void Logger_SetRuntimeLogLevel(LogLevel level)
{
    if(level >= LOG_LEVEL_MAX)
    {
        return;
    }
    else
    {
        g_cfg_runtime_log_level = level;
    }
}