# ESC Seat Firmware

Embedded firmware for the ESC Seat project. The firmware is built with CMake for an STM32F103 target and is structured as layered application, service, protocol, middleware, driver, and platform code.

## Development environment

- **Host:** Windows
- **Development system:** Ubuntu in VMware
- **Build system:** CMake with an ARM GCC toolchain
- **MCU platform:** STM32F103, STM32 HAL/CubeMX integration

## Firmware layout

```text
firmware/
├── application/   # Seat, motor, display, LED, and system applications
├── common/        # Shared types, configuration, errors, versioning, and CRC32
├── drivers/       # BSP and HAL-facing drivers
├── middleware/    # CLI, logger, event, scheduler, memory, timer, and system time
├── platform/      # Board definition and STM32F103/CubeMX platform project
├── protocol/      # CAN, TCP, and UDS protocol layers
├── services/      # Communication, configuration, and motor services
├── test/          # Firmware tests
└── thirdparty/    # External dependencies
```

The STM32CubeMX project and target configuration are under `platform/stm32/esc_seat_fw/`. The repository-level `project/` directory contains build output, packaging and flash scripts, and toolchain configuration.

## Current foundation

- Application modules for seat control, motor control, display, LED, and system integration
- Common project definitions plus CRC32 support
- Middleware for CLI, logging, events, scheduling, memory, timers, and system time
- CAN/TCP/UDS protocol structure and communication/configuration/motor services
- STM32 UART1 transport for CLI input and runtime logs

## UART

UART1 runs at 115200 8N1. Receive defaults to circular DMA with an NDTR-derived write index, CPU read index, DMA-wrap tracking, and overflow recovery. The interrupt RX implementation is retained behind `DMA_UART` as a working alternative/reference path. TX intentionally remains blocking because current CLI and logging traffic does not justify TX DMA.

See [`drivers/hal/uart/README.md`](drivers/hal/uart/README.md) for the UART module design.

