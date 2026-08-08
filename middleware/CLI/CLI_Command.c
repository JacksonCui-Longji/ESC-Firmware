#include "CLI_Command.h"
#include "BSP_UART.h"
#include "Logger.h"
#include <string.h>

static const char LOG_PARA_ERROR[] = "Input parameter error! eg: log set level debug\r\n";

void CLI_Help(int argc, char *argv[])
{
    const uint8_t help_info[] = 
    "1. use help checking help information.\r\n"
    "2. use log set level debug/info/warn/error to set runtime log level\r\n"
    "3. use log get level to check runtime log level.\r\n";

    BSP_UART_Send(help_info, strlen(help_info));

}

void CLI_Log(int argc, char *argv[])
{
    if(argc <= 0 || argv == NULL)
    {
        return;
    }

    /*
     * command:
     *
     * log set level debug
     *
     * argv[0] = set
     * argv[1] = level
     * argv[2] = debug
     *
     */
    if(strcmp(argv[0], "set") == 0)
    {
        if(argc < 3)
        {
            BSP_UART_Send((uint8_t *)LOG_PARA_ERROR, strlen(LOG_PARA_ERROR));
            return;
        }

        if(strcmp(argv[1], "level") == 0)
        {
            if(strcmp(argv[2], "debug") == 0)
            {
                Logger_SetRuntimeLogLevel(LOG_LEVEL_DEBUG);
            }
            else if(strcmp(argv[2], "info") == 0)
            {
                Logger_SetRuntimeLogLevel(LOG_LEVEL_INFO);
            }
            else if(strcmp(argv[2], "warn") == 0)
            {
                Logger_SetRuntimeLogLevel(LOG_LEVEL_WARN);
            }
            else if(strcmp(argv[2], "error") == 0)
            {
                Logger_SetRuntimeLogLevel(LOG_LEVEL_ERROR);
            }
            else
            {
                BSP_UART_Send((uint8_t *)LOG_PARA_ERROR, strlen(LOG_PARA_ERROR));
            }
        }
        else
        {
            BSP_UART_Send((uint8_t *)LOG_PARA_ERROR, strlen(LOG_PARA_ERROR));
        }

    }
    else if(strcmp(argv[0], "get") == 0)
    {
        if(argc < 2)
        {
            BSP_UART_Send((uint8_t *)LOG_PARA_ERROR, strlen(LOG_PARA_ERROR));
            return;
        }

        if(strcmp(argv[1], "level") == 0)
        {
            LogLevel level = Logger_GetRuntimeLogLevel();
            switch(level)
            {
                case LOG_LEVEL_DEBUG:
                {
                    BSP_UART_Send((uint8_t *)"Runtime log level debug\r\n", strlen("Runtime log level debug\r\n"));
                    break;
                }
                case LOG_LEVEL_INFO:
                {
                    BSP_UART_Send((uint8_t *)"Runtime log level info\r\n", strlen("Runtime log level info\r\n"));
                    break;
                }
                case LOG_LEVEL_WARN:
                {
                    BSP_UART_Send((uint8_t *)"Runtime log level warn\r\n", strlen("Runtime log level warn\r\n"));
                    break;
                }
                case LOG_LEVEL_ERROR:
                {
                    BSP_UART_Send((uint8_t *)"Runtime log level error\r\n", strlen("Runtime log level error\r\n"));
                    break;
                }
                default:
                {
                    BSP_UART_Send((uint8_t *)"Runtime log level get failed\r\n", strlen("Runtime log level get failed\r\n"));
                    break;
                }
            }
        }
        else
        {
            BSP_UART_Send((uint8_t *)LOG_PARA_ERROR, strlen(LOG_PARA_ERROR));
        }
    }
    else
    {
        BSP_UART_Send((uint8_t *)LOG_PARA_ERROR, strlen(LOG_PARA_ERROR));
    }
}
