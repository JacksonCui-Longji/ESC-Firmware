# UART Driver

STM32 HAL UART1 driver for the ESC Seat firmware. The public interface is in `BSP_UART.h`; the STM32-specific implementation is in `stm32/BSP_UART.c`.

UART1 is configured for **115200 baud, 8 data bits, no parity, and 1 stop bit (8N1)**. It carries CLI input and runtime logger output.

## TX

`BSP_UART_Send()` uses blocking transmission:

```c
HAL_UART_Transmit(&huart1, data, len, 1000);
```

This is intentional. Current traffic consists of short CLI replies and logs, so a TX DMA queue would add complexity without a practical benefit. Reconsider TX DMA only for sustained traces, large data dumps, or asynchronous high-throughput output.

## RX default: circular DMA

`DMA_UART` is enabled by default. It starts circular reception using `HAL_UART_Receive_DMA()` and writes directly into the RX buffer.

```c
#define DMA_UART
```

During `BSP_UART_RxPop()`, the CPU derives the DMA producer position from NDTR:

```text
write_index = BSP_UART_RX_BUFFER_MAX - NDTR
```

The CPU consumes bytes using a separate read index, giving the driver a producer/consumer ring-buffer model without a receive interrupt for every byte.

## DMA wrap, overflow, and recovery

`HAL_UART_RxCpltCallback()` records each completed DMA buffer cycle. The driver maintains:

- `dma_wrap_count` for outstanding DMA cycles
- `last_read_index` to identify a consumer wrap
- `overflow` for the current overwrite condition
- `overflow_reported` to report each condition once

If DMA has an outstanding wrap and its write index catches the CPU read index, unread data has been overwritten. The driver reports overflow once. As the CPU consumes data and wraps, the outstanding count is reduced. When it reaches zero, the overflow state is cleared and recovery is reported.

## Retained interrupt RX path

Removing `DMA_UART` selects the alternative one-byte interrupt RX implementation:

```c
// #define DMA_UART
```

`HAL_UART_Receive_IT()` receives one byte, `HAL_UART_RxCpltCallback()` pushes it into the ring buffer, and the driver rearms reception. This path is retained as a working reference and regression/comparison option.

## Summary

| Area | Current implementation |
| --- | --- |
| UART | STM32 HAL UART1, 115200 8N1 |
| Default RX | Circular DMA selected by `DMA_UART` |
| Write position | Derived from DMA NDTR |
| Consumption | CPU read index via `BSP_UART_RxPop()` |
| DMA cycle event | `HAL_UART_RxCpltCallback()` |
| Protection | Overflow detection, one-time reporting, and recovery |
| Alternative RX | One-byte interrupt RX behind the macro |
| TX | Blocking `HAL_UART_Transmit()` |

