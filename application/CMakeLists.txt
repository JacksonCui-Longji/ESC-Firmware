add_library(Application STATIC)

target_include_directories(Drivers PUBLIC
    display
    motor
    seat
    system
)

target_sources(Application PRIVATE
    system/AppSystem.c
    motor/AppMotorControl.c
    seat/AppSeatController.c
)

target_link_libraries(Application
    Services
    Drivers
    Common
)