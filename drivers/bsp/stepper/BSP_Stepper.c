#include "BSP_Stepper.h"

static uint8_t s_is_initialized;
static uint8_t s_is_enabled;
static uint8_t s_is_running;

static void BSP_Stepper_ConfigureStepPin(uint32_t mode)
{
    GPIO_InitTypeDef gpio = {0};

    gpio.Pin = BSP_STEPPER_STEP_GPIO_PIN;
    gpio.Mode = mode;
    gpio.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(BSP_STEPPER_STEP_GPIO_PORT, &gpio);
}

static ret_t BSP_Stepper_IsFrequencyValid(uint32_t frequency_hz)
{
    uint32_t period_counts;

    if ((frequency_hz == 0u) || (frequency_hz > (BSP_STEPPER_TIMER_CLOCK_HZ / 2u)))
    {
        return RET_NG;
    }

    period_counts = BSP_STEPPER_TIMER_CLOCK_HZ / frequency_hz;

    /* TIM4 is a 16-bit timer and a 50 % waveform needs at least two counts. */
    if ((period_counts < 2u) || (period_counts > 65536u))
    {
        return RET_NG;
    }

    return RET_OK;
}

ret_t BSP_Stepper_Init(void)
{
    GPIO_InitTypeDef gpio = {0};

    BSP_STEPPER_STEP_GPIO_CLK_ENABLE();
    BSP_STEPPER_DIR_GPIO_CLK_ENABLE();
    BSP_STEPPER_ENABLE_GPIO_CLK_ENABLE();
    BSP_STEPPER_STEP_TIMER_CLK_ENABLE();

    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_HIGH;

    gpio.Pin = BSP_STEPPER_DIR_GPIO_PIN;
    HAL_GPIO_Init(BSP_STEPPER_DIR_GPIO_PORT, &gpio);

    gpio.Pin = BSP_STEPPER_ENABLE_GPIO_PIN;
    HAL_GPIO_Init(BSP_STEPPER_ENABLE_GPIO_PORT, &gpio);

    BSP_Stepper_ConfigureStepPin(GPIO_MODE_OUTPUT_PP);
    HAL_GPIO_WritePin(BSP_STEPPER_STEP_GPIO_PORT, BSP_STEPPER_STEP_GPIO_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(BSP_STEPPER_DIR_GPIO_PORT, BSP_STEPPER_DIR_GPIO_PIN, BSP_STEPPER_DIR_FORWARD_STATE);
    HAL_GPIO_WritePin(BSP_STEPPER_ENABLE_GPIO_PORT, BSP_STEPPER_ENABLE_GPIO_PIN,
                      BSP_STEPPER_ENABLE_INACTIVE_STATE);

    /*
     * Configure TIM4 CH1 directly because the current CubeMX project does not
     * include the HAL TIM module.  The timer, not the CPU, generates STEP.
     */
    BSP_STEPPER_STEP_TIMER->CR1 = 0u;
    BSP_STEPPER_STEP_TIMER->PSC = 0u;
    BSP_STEPPER_STEP_TIMER->ARR = (BSP_STEPPER_TIMER_CLOCK_HZ / BSP_STEPPER_DEFAULT_FREQUENCY_HZ) - 1u;
    BSP_STEPPER_STEP_TIMER->CCR1 = (BSP_STEPPER_TIMER_CLOCK_HZ / BSP_STEPPER_DEFAULT_FREQUENCY_HZ) / 2u;
    BSP_STEPPER_STEP_TIMER->CCMR1 = TIM_CCMR1_OC1PE | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1;
    BSP_STEPPER_STEP_TIMER->CCER = 0u;
    BSP_STEPPER_STEP_TIMER->EGR = TIM_EGR_UG;
    BSP_STEPPER_STEP_TIMER->CR1 = TIM_CR1_ARPE;

    s_is_initialized = 1u;
    s_is_enabled = 0u;
    s_is_running = 0u;

    return RET_OK;
}

ret_t BSP_Stepper_Enable(void)
{
    if (s_is_initialized == 0u)
    {
        return RET_NG;
    }

    HAL_GPIO_WritePin(BSP_STEPPER_ENABLE_GPIO_PORT, BSP_STEPPER_ENABLE_GPIO_PIN,
                      BSP_STEPPER_ENABLE_ACTIVE_STATE);
    s_is_enabled = 1u;

    return RET_OK;
}

ret_t BSP_Stepper_Disable(void)
{
    if (s_is_initialized == 0u)
    {
        return RET_NG;
    }

    (void)BSP_Stepper_Stop();
    HAL_GPIO_WritePin(BSP_STEPPER_ENABLE_GPIO_PORT, BSP_STEPPER_ENABLE_GPIO_PIN,
                      BSP_STEPPER_ENABLE_INACTIVE_STATE);
    s_is_enabled = 0u;

    return RET_OK;
}

ret_t BSP_Stepper_SetDirection(BSP_Stepper_Direction direction)
{
    if ((s_is_initialized == 0u) || (s_is_running != 0u))
    {
        return RET_NG;
    }

    if (direction == BSP_STEPPER_FORWARD)
    {
        HAL_GPIO_WritePin(BSP_STEPPER_DIR_GPIO_PORT, BSP_STEPPER_DIR_GPIO_PIN,
                          BSP_STEPPER_DIR_FORWARD_STATE);
    }
    else if (direction == BSP_STEPPER_BACKWARD)
    {
        HAL_GPIO_WritePin(BSP_STEPPER_DIR_GPIO_PORT, BSP_STEPPER_DIR_GPIO_PIN,
                          BSP_STEPPER_DIR_BACKWARD_STATE);
    }
    else
    {
        return RET_NG;
    }

    return RET_OK;
}

ret_t BSP_Stepper_SetFrequency(uint32_t frequency_hz)
{
    uint32_t period_counts;

    if ((s_is_initialized == 0u) || (s_is_running != 0u) ||
        (BSP_Stepper_IsFrequencyValid(frequency_hz) != RET_OK))
    {
        return RET_NG;
    }

    period_counts = BSP_STEPPER_TIMER_CLOCK_HZ / frequency_hz;
    BSP_STEPPER_STEP_TIMER->ARR = period_counts - 1u;
    BSP_STEPPER_STEP_TIMER->CCR1 = period_counts / 2u;
    BSP_STEPPER_STEP_TIMER->EGR = TIM_EGR_UG;

    return RET_OK;
}

ret_t BSP_Stepper_Start(void)
{
    if ((s_is_initialized == 0u) || (s_is_enabled == 0u) || (s_is_running != 0u))
    {
        return RET_NG;
    }

    BSP_Stepper_ConfigureStepPin(GPIO_MODE_AF_PP);
    BSP_STEPPER_STEP_TIMER->CNT = 0u;
    BSP_STEPPER_STEP_TIMER->CCER |= TIM_CCER_CC1E;
    BSP_STEPPER_STEP_TIMER->CR1 |= TIM_CR1_CEN;
    s_is_running = 1u;

    return RET_OK;
}

ret_t BSP_Stepper_Stop(void)
{
    if (s_is_initialized == 0u)
    {
        return RET_NG;
    }

    BSP_STEPPER_STEP_TIMER->CR1 &= ~TIM_CR1_CEN;
    BSP_STEPPER_STEP_TIMER->CCER &= ~TIM_CCER_CC1E;
    BSP_Stepper_ConfigureStepPin(GPIO_MODE_OUTPUT_PP);
    HAL_GPIO_WritePin(BSP_STEPPER_STEP_GPIO_PORT, BSP_STEPPER_STEP_GPIO_PIN, GPIO_PIN_RESET);
    s_is_running = 0u;

    return RET_OK;
}
