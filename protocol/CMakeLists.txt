add_library(Protocol STATIC)

target_sources(Protocol PRIVATE
    can/CanFrame.c
    tcp/TCP.c
)

target_link_libraries(Protocol
    Common
)