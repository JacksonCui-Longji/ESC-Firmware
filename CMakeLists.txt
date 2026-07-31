cmake_minimum_required(VERSION 3.22)

project(ESC_Seat_Firmware
    VERSION 0.1.0
    LANGUAGES C ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_subdirectory(common)
add_subdirectory(platform/stm32)
add_subdirectory(drivers)
add_subdirectory(middleware)
add_subdirectory(protocol)
add_subdirectory(services)
add_subdirectory(application)