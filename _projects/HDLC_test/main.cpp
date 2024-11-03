#include "MSLP.h"
#include "UART.h"

static uint16_t cksum(void* addr, uint16_t count)
{
    uint32_t sum = 0;
    uint16_t* p = (uint16_t*)addr;

    while (count > 1) {
        sum += *p++;
        printf("sum: %x\n", sum);
        count -= 2;
    }

    if (count > 0) {
        sum += *(uint8_t*)addr;
        printf("sum: %x\n", sum);
    }

    while (sum >> 16)
        sum = (sum & 0xffff) + (sum >> 16);

    return ~sum;
}


static void testRecv() {

    UART uart;
    if (!uart.Initialize("COM7", CBR_1200)) {
        exit(1);
    }

}


int main(int argc, char* argv[]) {

    uint8_t buf[6];
    for (int i = 0; i < 6; ++i) {
        buf[i] = i;
    }
    //uint16_t cs = cksum(buf, sizeof buf);
    
    UART uart;
    if (!uart.Initialize("COM7", CBR_1200)) {
        exit(1);
    }

    /*
    uint8_t data[DATA_SIZE];
    for (int i = 0; i < DATA_SIZE; ++i) {
        data[i] = i + 1;
    }

    data[1] = 0x7E;
    data[2] = 0x7D;
    data[3] = 0x7F;
    */

    SerialDev_UART comm(&uart);
    MSLP mslp(&comm, 0xbeef);
    uint8_t data[32];
    mslp.ReceiveData(data, 16);
    printf("data:\n");
    for (int i = 0; i < 16; ++i) {
        printf("%d: %x\n", i, data[i]);
    }

}
