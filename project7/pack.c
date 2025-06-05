#include "pack.h"

uint32_t read_u32(const unsigned char *buf) {
    return ((uint32_t)buf[0] << 24) |
           ((uint32_t)buf[1] << 16) |
           ((uint32_t)buf[2] << 8)  |
           ((uint32_t)buf[3]);
}

uint16_t read_u16(const unsigned char *buf) {
    return ((uint16_t)buf[0] << 8) |
           ((uint16_t)buf[1]);
}

uint8_t read_u8(const unsigned char *buf) {
    return buf[0];
}

void write_u32(unsigned char *buf, uint32_t val) {
    buf[0] = (val >> 24) & 0xFF;
    buf[1] = (val >> 16) & 0xFF;
    buf[2] = (val >> 8) & 0xFF;
    buf[3] = val & 0xFF;
}

void write_u16(unsigned char *buf, uint16_t val) {
    buf[0] = (val >> 8) & 0xFF;
    buf[1] = val & 0xFF;
}

void write_u8(unsigned char *buf, uint8_t val) {
    buf[0] = val;
}
