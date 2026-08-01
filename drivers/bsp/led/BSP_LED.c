#include "BSP_LED.h"
#include "esc_seat_board.h"

ret_t BSP_LED_Init()
{

}

ret_t BSP_LED_Deinit()
{

}

ret_t BSP_LED_State_Set(BSP_LED_Color led, BSP_LED_State state)
{
    GPIO_PinState hal_state = GPIO_PIN_RESET;

    if(BSP_LED_OFF == state)
    {
        hal_state = GPIO_PIN_SET;
    }
    else if(BSP_LED_ON == state)
    {
        hal_state = GPIO_PIN_RESET;
    }
    else
    {
        return RET_NG;
    }

    switch(led)
    {
        case BSP_LED_COLOR_GREEN:
        {
            HAL_GPIO_WritePin(BOARD_LED_GREEN_GPIO_PORT, BOARD_LED_GREEN_GPIO_PIN, hal_state);
            break;
        }
        case BSP_LED_COLOR_BLUE:
        {
            HAL_GPIO_WritePin(BOARD_LED_BLUE_GPIO_PORT, BOARD_LED_BLUE_GPIO_PIN, hal_state);
            break;
        }
        case BSP_LED_COLOR_RED:
        {
            HAL_GPIO_WritePin(BOARD_LED_RED_GPIO_PORT, BOARD_LED_RED_GPIO_PIN, hal_state);
            break;
        }
        default:
        {
            return RET_NG;
        }
    }
    return RET_OK;
}

BSP_LED_State BSP_LED_State_Get(BSP_LED_Color led)
{
    GPIO_PinState state = GPIO_PIN_RESET;
    BSP_LED_State ret_state = BSP_LED_INVALID;

    switch(led)
    {
        case BSP_LED_COLOR_GREEN:
        {
            ret_state = HAL_GPIO_ReadPin(BOARD_LED_GREEN_GPIO_PORT, BOARD_LED_GREEN_GPIO_PIN);
            break;
        }
        case BSP_LED_COLOR_BLUE:
        {
            ret_state = HAL_GPIO_ReadPin(BOARD_LED_BLUE_GPIO_PORT, BOARD_LED_BLUE_GPIO_PIN);
            break;
        }
        case BSP_LED_COLOR_RED:
        {
            ret_state = HAL_GPIO_ReadPin(BOARD_LED_RED_GPIO_PORT, BOARD_LED_RED_GPIO_PIN);
            break;
        }
        default:
        {
            return BSP_LED_INVALID;
        }
    }

    if(GPIO_PIN_RESET == state)
    {
        ret_state = BSP_LED_OFF;
    }
    else if(GPIO_PIN_SET == state)
    {
        ret_state = BSP_LED_ON;
    }
    else
    {
        ret_state = BSP_LED_INVALID;
    }

    return ret_state;
}

ret_t BSP_LED_Toggle(BSP_LED_Color led)
{
    switch(led)
    {
        case BSP_LED_COLOR_GREEN:
        {
            HAL_GPIO_TogglePin(BOARD_LED_GREEN_GPIO_PORT, BOARD_LED_GREEN_GPIO_PIN);
            break;
        }
        case BSP_LED_COLOR_BLUE:
        {
            HAL_GPIO_TogglePin(BOARD_LED_BLUE_GPIO_PORT, BOARD_LED_BLUE_GPIO_PIN);
            break;
        }
        case BSP_LED_COLOR_RED:
        {
            HAL_GPIO_TogglePin(BOARD_LED_RED_GPIO_PORT, BOARD_LED_RED_GPIO_PIN);
            break;
        }
        default:
        {
            return RET_NG;
        }
    }
    
    return RET_OK;
}
