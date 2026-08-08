#ifndef CRC32_H
#define CRC32_H

#include "string.h"
#include "stdint.h"
#include "stdio.h"

#define CLI_CRC32_DATA_MAX_SIZE (128u)
#define CRC_32_REGISHTER_LENGTH 4

extern void u32CrcIeee8023_Init(uint32_t *crc);
extern void u32CrcIeee8023_Excu(uint32_t *retCrc, uint8_t* inData, size_t len);
extern void u32CrcIeee8023_Final(uint32_t *retCrc);

extern uint32_t u32CrcIeee8023(uint8_t* inData, size_t len);

#endif // CRC32_H