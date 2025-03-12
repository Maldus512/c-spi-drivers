#include "mcp3204.h"
#include "spi_common/spi_common.h"


int mcp3204_read_adc_channel(spi_driver_t driver, mcp2304_input_mode_t input_mode, mcp3204_channel_t ch,
                             uint16_t *value) {
    uint8_t  buffer[3]      = {0};
    uint8_t  read_buffer[3] = {0};
    uint16_t reading;

    buffer[0] = (1 << 2) |     // start bit
                ((input_mode == MCP3204_INPUT_MODE_SINGLE_ENDED ? 1 : 0) << 1);
    buffer[1] = (ch & 0x3) << 6;

    SPI_CS(driver, 0);
    driver.spi_exchange(buffer, read_buffer, 3, driver.user_data);

    reading = ((read_buffer[1] & 0xF) << 8) | read_buffer[2];
    *value  = reading;

    SPI_CS(driver, 1);
    return 0;
}
