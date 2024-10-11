#include "UART.h"
#include "pctime.h"

#if 0
bool is_match(uint8_t* str, size_t str_len, uint8_t* substr, size_t substr_len) {

    if (substr_len > str_len) {
        return false;
    }

    for (size_t i = 0; i <= str_len - substr_len; i++) {
        if (memcmp(&str[i], substr, substr_len) == 0) {
            return true;
        }
    }

    return false;
}

static void testReceive() {

    uint8_t buf[2048];
    uint8_t b = 0;
    int total = 0;
    int nerrors = 0;
    int cnt = 20;

    UART uart;
    if (!uart.Initialize("COM8", CBR_1200)) {
        exit(1);
    }

    while (true)
    {
        uint32_t cmd;
        if (uart.Read((uint8_t*)&cmd, 4, 1000)) {
            if (cmd != STM32_CMD_START) {
                continue;
            }
        }
        //printf("got command start\n");
        uint8_t expected_src;
        if (uart.Read(&expected_src, 1, 1000)) {
            //printf("expected crc: %x\n", expected_src);
        }
        else {
            continue;
        }

        if (uart.Read(buf, cnt, 1000)) {
            ++total;
            uint8_t crc = calculateCRC(buf, cnt);
            if (crc != expected_src) {
                printf("crc error\n");
                ++nerrors;
                for (int i = 0; i < cnt; ++i) {
                    printf("%d ", buf[i]);
                }
                printf("\n");
            }
            printf("total: %d, errors: %d, %%: %f\n", total, nerrors, (float)nerrors / (float)total * 100.0);

        }
        else {
            continue;
        }

    }

}

static void testReceive2() {

    UART uart;
    if (!uart.Initialize("COM8", CBR_600)) {
        exit(1);
    }
    int current = -1;
    uint8_t b;
    int total = 0;
    int nerrors = 0;

    while (true)
    {
        if (uart.Read(&b, 1, 1000)) {
            if (current == -1) {
                current = b;
                continue;
            }
            ++current;
            ++total;
            if ((current % 256) != b) {
                ++nerrors;
                printf("total: %d, errors: %d, %%: %f\n", total, nerrors, (float)nerrors / (float)total * 100.0);
            }
            //printf("read: %d, expected: %d, nerrors: %d\n", b, current % 256, nerrors);
        }
        if (total % 100 == 0) {
            printf("total: %d, errors: %d, %%: %f\n", total, nerrors, (float)nerrors / (float)total * 100.0);
        }

    }

}

static void testReceive3() {

    UART uart;
    if (!uart.Initialize("COM8", CBR_1200)) {
        exit(1);
    }
    int current = -1;
    uint8_t b;
    int total = 0;
    int nerrors = 0;

    while (true)
    {
        if (uart.Read(&b, 1, 1000)) {
            printf("%d\n", b);
        }

    }

}

static void testReceive4() {

    UART uart;
    if (!uart.Initialize("COM8", CBR_1200)) {
        exit(1);
    }
    int current = -1;
    const int n = 254;
    uint8_t buf[n];
    int total = 0;
    int nerrors = 0;

    while (true)
    {
        if (uart.Read(buf, n, 1000)) {
            uint8_t expected_crc;
            if (uart.Read(&expected_crc, 1, 1000)) {
                uint8_t crc = calculateCRC(buf, n);
                printf("expected: %x, calculated: %x\n", expected_crc, crc);
                uart.Write(buf, n);
                uart.Write(&crc, 1);
                break;
            }
        }

    }

}

static void testSend() {

    UART uart;
    if (!uart.Initialize("COM8", CBR_1200)) {
        exit(1);
    }
    uint8_t b = 0;
    const int n = 50;
    uint8_t buf[n];

    for (int i = 0; i < n; ++i) {
        buf[i] = i;
    }

    uint8_t crc = calculateCRC(buf, n);
    printf("crc: %x\n", crc);

    while (true)
    {
        //printf("sending %d\n", b);
        uart.Write(buf, n);
        uart.Write(&crc, 1);
        break;
        ::Sleep(5000);
        //++b;
    }

}

static void STM32_Session() {

    UART uart;
    if (!uart.Initialize("COM8", CBR_1200)) {
        exit(1);
    }

    uint32_t cmd = 0;
    bool cmdReceived = false;
    bool rc = uart.Read((uint8_t*)&cmd, sizeof cmd, 10000);
    if (rc) {
        printf("received: %x\n", cmd);
        if (cmd == STM32_CMD_START) {
            cmdReceived = true;
        }
    }


    if (!cmdReceived) {
        printf("Handshake failed\n");
        return;
    }

    cmd = STM32_CMD_START;
    uart.Write((uint8_t*)&cmd, sizeof cmd);
    printf("Handshake OK\n");

}

static void STM32_Session2() {

    UART uart;
    if (!uart.Initialize("COM8", CBR_1200)) {
        exit(1);
    }

    uint32_t cmd = STM32_CMD_START;
    uint8_t buf[256];
    uint8_t b;
    int idx = 0;
    bool start_received = false;
    uint8_t cmd_size = sizeof(cmd);
    while (1) {
        bool rc = uart.Read(&b, 1, 5000);
        if (rc) {
            buf[idx++] = b;
            printf("got: %x\n", b);
            if (is_match(buf, cmd_size + 1, (uint8_t*)&cmd, cmd_size)) {
                start_received = true;
                break;
            }
        }
        if (idx > cmd_size + 1) {
            break;
        }
    }

    printf("start received: %d\n", start_received);
    if (start_received) {
        cmd = STM32_CMD_START;
        uart.Write((uint8_t*)&cmd, sizeof cmd);
    }

}

#endif