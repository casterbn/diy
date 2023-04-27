#include "shtc3.h"
#include "zi2c.h"

shtc3::shtc3(zi2c& i2c) :
    _i2c(i2c)
{
}

unsigned char crc8_soft(unsigned char crc, const void* msg, int size)
{
    const unsigned char polynormial = 0x31;
    unsigned char* p = (unsigned char*)msg;
    for(int i = 0; i < size; i++) {
        crc ^= *p++;
        for(int j = 0; j < 8; j++) {
            int lsb = crc & 0x80;
            crc <<= 1;
            if(lsb)
                crc ^= polynormial;
        }
    }
    return crc;
}

shtc3_result_t shtc3::read(void)
{
    unsigned char buf[6];
    _i2c.write_reg(0x35, 0x17);
    _delay_ms(5);
    _i2c.write_reg(0x7c, 0xa2);
    _delay_ms(5);
    _i2c.read(buf, 6);
    _delay_ms(5);
    _i2c.write_reg(0xb0, 0x98);

    unsigned char crc1 = crc8_soft(0xff, buf, 2);
    unsigned char crc2 = crc8_soft(0xff, buf + 3, 2);
    shtc3_result_t result = {0};
    if(crc1 == buf[2] && crc2 == buf[5]) {
        result.T = (((buf[0] * 256 + buf[1]) * 17500UL) >> 16) - 4500;
        result.RH = ((buf[3] * 256 + buf[4]) * 10000UL) >> 16;
    }
    return result;
}
