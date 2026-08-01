add_library(Middleware STATIC)


target_sources(Middleware PRIVATE
    event/Event.c
    logger/Logger.c
    scheduler/Scheduler.c
    timer/Timer.c
    memory/MemoryPool.c
)


target_include_directories(Middleware PUBLIC
    event
    logger
    scheduler
    timer
    memory
)


target_link_libraries(Middleware PUBLIC
    Common
    Drivers
)