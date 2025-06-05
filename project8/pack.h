#ifndef PACK_H
#define PACK_H

#include <stdint.h>

uint32_t read_u32(const unsigned char *buf);
uint16_t read_u16(const unsigned char *buf);
uint8_t  read_u8(const unsigned char *buf);

void write_u32(unsigned char *buf, uint32_t val);
void write_u16(unsigned char *buf, uint16_t val);
void write_u8(unsigned char *buf, uint8_t val);

#endif
