# common head file
add_library(Common INTERFACE)

target_include_directories(Common INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# CRC32 utils
add_library(Crc32 STATIC)

target_sources(Crc32 PRIVATE
    utils/crc/crc32/CRC32.c
)

target_include_directories(Crc32 PUBLIC
    utils/crc/crc32
)

# other components
