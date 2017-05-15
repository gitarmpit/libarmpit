#include "sensors.h"
#include "string.h"

void alignSensors(int32_t *src, int32_t *dest, uint8_t rotation)
{
    static uint32_t swap[3];
    memcpy(swap, src, sizeof(swap));

    switch (rotation) {
        default:
        case CW0_DEG:
            dest[0] = swap[0];
            dest[1] = swap[1];
            dest[2] = swap[2];
            break;
        case CW90_DEG:
            dest[0] = swap[1];
            dest[1] = -swap[0];
            dest[2] = swap[2];
            break;
        case CW180_DEG:
            dest[0] = -swap[0];
            dest[1] = -swap[1];
            dest[2] = swap[2];
            break;
        case CW270_DEG:
            dest[0] = -swap[1];
            dest[1] = swap[0];
            dest[2] = swap[2];
            break;
        case CW0_DEG_FLIP:
            dest[0] = -swap[0];
            dest[1] = swap[1];
            dest[2] = -swap[2];
            break;
        case CW90_DEG_FLIP:
            dest[0] = swap[1];
            dest[1] = swap[0];
            dest[2] = -swap[2];
            break;
        case CW180_DEG_FLIP:
            dest[0] = swap[0];
            dest[1] = -swap[1];
            dest[2] = -swap[2];
            break;
        case CW270_DEG_FLIP:
            dest[0] = -swap[1];
            dest[1] = -swap[0];
            dest[2] = -swap[2];
            break;
    }

}
