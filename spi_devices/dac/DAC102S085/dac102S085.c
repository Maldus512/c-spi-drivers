#include "dac102S085.h"


#define WRITE_REGISTER_AND_UPDATE_OUTPUT_OP 0x01


void dac102S085_set(spi_driver_t driver, dac102s085_dac_channel_t channel, uint16_t value) {
    uint16_t buffer = 0;

    channel &= 0x3;     // Cap to two channels
    value &= 0x3FF;     // Cap to 1023

    buffer = ((channel ? 1 : 0) << 14) | (WRITE_REGISTER_AND_UPDATE_OUTPUT_OP << 12) | (value << 2);

    uint8_t tx_data[2] = {
        (buffer >> 8) & 0xFF,
        buffer & 0xFF,
    };

    spi_write_data(driver, tx_data, sizeof(tx_data));
}
