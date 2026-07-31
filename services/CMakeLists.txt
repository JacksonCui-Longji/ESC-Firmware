add_library(Services STATIC)

target_sources(Services PRIVATE
    communication/ServiceComm.c
    motor/ServiceMotor.c
)

target_link_libraries(Services
    Drivers
    Protocol
    Common
)