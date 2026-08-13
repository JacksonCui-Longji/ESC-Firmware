add_library(Middleware STATIC)


target_sources(Middleware PRIVATE
    event/Event.c
    logger/Logger.c
    scheduler/Scheduler.c
    timer/Timer.c
    memory/MemoryPool.c
    system/system_time/SystemTime.c
    GUI/GraphicUserInterface.c
    CLI/CLI.c
    CLI/CLI_Command.c
)


target_include_directories(Middleware PUBLIC
    event
    logger
    scheduler
    timer
    memory
    system/system_time
    CLI
    GUI
)


target_link_libraries(Middleware PUBLIC
    Common
    Drivers
    Crc32
)