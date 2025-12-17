#include <stdio.h>
#include <stdint.h>

// CRC-16-CCITT (“False”), but processes 16 bits per loop iteration
uint16_t crc16_ccitt_16bit(const uint8_t *data, size_t length) {
    uint16_t crc  = 0xFFFF;
    uint16_t poly = 0x1021;

    // length must be an even number of bytes
    for (size_t i = 0; i + 1 < length; i += 2) {
        // combine two bytes into one 16-bit word (MSB first)
        uint16_t word = ((uint16_t)data[i] << 8) | data[i+1];
        crc ^= word;
        printf( "16-bit crc ^ data: %0x\n", crc );
        // now shift through all 16 bits
        for (int bit = 0; bit < 16; bit++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ poly;
                printf( "16-bit chunks: %0x\n", crc );
            } else
                crc <<= 1;
        }
    }

    return crc;
}

// original byte-wise version for comparison
uint16_t crc16_ccitt_8bit(const uint8_t *data, size_t length) {
    uint16_t crc  = 0xFFFF;
    uint16_t poly = 0x1021;

    for (size_t i = 0; i < length; i++) {
        crc ^= (uint16_t)data[i] << 8;
        printf( "8-bit crc ^ data: %0x\n", crc );
        for (int b = 0; b < 8; b++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ poly;
                printf( "8-bit chunks: %0x\n", crc );
            } else
                crc <<= 1;
        }
    }

    return crc;
}

int main(void) {
    uint8_t buf[] = { 0x20, 0x00, 0x14, 0x00 };
    size_t   len  = sizeof(buf);

    uint16_t crc8  = crc16_ccitt_8bit(buf, len);
    uint16_t crc16 = crc16_ccitt_16bit(buf, len);

    printf("CRC-16-CCITT (8-bit chunks):  0x%04X\n", crc8);
    printf("CRC-16-CCITT (16-bit chunks): 0x%04X\n", crc16);
    return 0;
}
