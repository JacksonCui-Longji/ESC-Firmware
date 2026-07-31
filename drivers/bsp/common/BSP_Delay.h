#ifndef BSP_DELAY_H
#define BSP_DELAY_H


#include "esc_types.h"


/**
 * @brief Initialize BSP delay module
 */
void BSP_Delay_Init(void);


/**
 * @brief Delay milliseconds
 *
 * @param ms delay time
 */
void BSP_Delay_ms(uint32_t ms);



#endif