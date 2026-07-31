add_library(STM32_HAL INTERFACE)


target_include_directories(STM32_HAL INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/../esc_seat_fw/Core/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/../esc_seat_fw/Drivers/STM32F1xx_HAL_Driver/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/../esc_seat_fw/Drivers/CMSIS/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/../esc_seat_fw/Drivers/CMSIS/Device/ST/STM32F1xx/Include
)

target_compile_definitions(STM32_HAL INTERFACE
    STM32F103xE
    USE_HAL_DRIVER
)
