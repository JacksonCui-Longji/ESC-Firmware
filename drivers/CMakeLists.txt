add_library(Drivers STATIC)

target_sources(Drivers PRIVATE
    bsp/common/BSP_Delay.c
    bsp/common/BSP_Time.c
    bsp/led/BSP_LED.c
    bsp/stepper/BSP_Stepper.c

    hal/uart/stm32/BSP_UART.c
)


target_include_directories(Drivers PUBLIC
    bsp/common
    bsp/led
    bsp/stepper
    
    hal/uart
    ${PROJECT_SOURCE_DIR}/platform/board
)

target_link_libraries(Drivers PUBLIC
    Common
    STM32_HAL
)