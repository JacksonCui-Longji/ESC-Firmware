#include "CLI.h"
#include "CLI_Command.h"
#include "BSP_UART.h"
#include <string.h>

#define CLI_MAX_ARGC   (8u)

typedef void (*CLI_Handler)(int argc, char *argv[]);

typedef struct
{
    const char *command;
    CLI_Handler handler;
}CLI_CommandTable_t;

/*
 * command handlers
 */
extern void CLI_Log(int argc, char *argv[]);

/*
 * Root command table
 *
 * argv[0]
 *
 * log
 * led
 * can
 * uds
 *
 */
static const CLI_CommandTable_t g_cli_command_table[] =
{
    {"log", CLI_Log},

};

#define CLI_COMMAND_NUM (sizeof(g_cli_command_table) / sizeof(g_cli_command_table[0]))

static int CLI_Parse(char *line, char *argv[])
{
    int argc = 0;
    while(*line)
    {
        while(*line == ' ')
        {
            line++;
        }

        if(*line == '\0')
        {
            break;
        }

        if(argc >= CLI_MAX_ARGC)
        {
            break;
        }

        argv[argc++] = line;

        while((*line != ' ') && (*line != '\0'))
        {
            line++;
        }

        if(*line)
        {
            *line = '\0';
            line++;
        }
    }
    return argc;
}

void CLI_Execute(int argc, char *argv[])
{
    if(argc <= 0 || argv == NULL)
    {
        return;
    }

    for(uint8_t i = 0; i < CLI_COMMAND_NUM; i++)
    {

        if(strcmp(argv[0], g_cli_command_table[i].command) == 0)
        {
            g_cli_command_table[i].handler(argc - 1, &argv[1]);
            return;
        }
    }
    BSP_UART_Send((uint8_t *)"Unknown command\r\n", strlen("Unknown command\r\n"));
}

static void CLI_ProcessLine(char *line)
{
    int argc;
    char *argv[CLI_MAX_ARGC];

    argc = CLI_Parse(line, argv);
    CLI_Execute(argc, argv);
}

void CLI_MainFunction(void)
{

    static uint8_t command_buffer[BSP_UART_RX_BUFFER_MAX];
    static uint16_t index = 0;
    uint8_t data;

    while(BSP_UART_RxPop(&data) == 0)
    {
        if((data == '\r') || (data == '\n'))
        {
            if(index > 0)
            {
                command_buffer[index] = '\0';
                uint8_t newline[] = "\r\n";
                BSP_UART_Send(command_buffer, strlen(command_buffer));
                BSP_UART_Send(newline, 2);
                CLI_ProcessLine((char *)command_buffer);
                memset(command_buffer, 0x00u, sizeof(command_buffer));
                index = 0;
            }
        }
        else
        {

            if(index < BSP_UART_RX_BUFFER_MAX - 1)
            {
                command_buffer[index++] = data;
            }
            else
            {
                index = 0;
                memset(command_buffer, 0x00u, sizeof(command_buffer));
            }
        }
    }
}