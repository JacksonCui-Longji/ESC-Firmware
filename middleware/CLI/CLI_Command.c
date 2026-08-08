#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "CLI_Command.h"
#include "BSP_UART.h"
#include "Logger.h"
#include "CRC32.h"

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

void CLI_CRC32Calculate(int argc, char *argv[])
{
    uint8_t data_cache[CLI_CRC32_DATA_MAX_SIZE];
    uint8_t output_buffer[CLI_CRC32_DATA_MAX_SIZE];
    size_t data_len = 0;
    int output_len = 0;
    uint32_t crc = 0;

    memset(data_cache, 0x00, sizeof(data_cache));
    memset(output_buffer, 0x00, sizeof(output_buffer));

    if ((argc <= 0) || (argv == NULL))
    {
        BSP_UART_Send(
            (uint8_t *)"Usage: crc32 <string | hex bytes>\r\n",
            strlen("Usage: crc32 <string | hex bytes>\r\n")
        );
        return;
    }

    /*
     * ========================================
     * ASCII string mode
     *
     * crc32 123456789
     * ========================================
     */
    if (argc == 1)
    {
        data_len = strlen(argv[0]);

        if (data_len > sizeof(data_cache))
        {
            BSP_UART_Send(
                (uint8_t *)"Input data overflow\r\n",
                strlen("Input data overflow\r\n")
            );
            return;
        }

        memcpy(data_cache, argv[0], data_len);
    }

    /*
     * ========================================
     * HEX byte mode
     *
     * crc32 01 02 03 04 05
     * ========================================
     */
    else
    {
        for (int i = 0; i < argc; i++)
        {
            char *endptr = NULL;
            unsigned long value;

            errno = 0;

            value = strtoul(argv[i], &endptr, 16);

            if ((endptr == argv[i]) ||
                (*endptr != '\0') ||
                (errno == ERANGE) ||
                (value > 0xFF))
            {
                BSP_UART_Send(
                    (uint8_t *)"Invalid hex byte\r\n",
                    strlen("Invalid hex byte\r\n")
                );
                return;
            }

            if (data_len >= sizeof(data_cache))
            {
                BSP_UART_Send(
                    (uint8_t *)"Input data overflow\r\n",
                    strlen("Input data overflow\r\n")
                );
                return;
            }

            data_cache[data_len++] = (uint8_t)value;
        }
    }

    /*
     * Debug information
     */
    output_len = snprintf(
        (char *)output_buffer,
        sizeof(output_buffer),
        "data_len: %u\r\n",
        data_len
    );

    BSP_UART_Send(output_buffer, output_len);
    /*
     * CRC32
     */
    crc = u32CrcIeee8023(data_cache, data_len);
    output_len = snprintf(
        (char *)output_buffer,
        sizeof(output_buffer),
        "CRC: %08X\r\n",
        crc
    );

    BSP_UART_Send(output_buffer, output_len);
}
