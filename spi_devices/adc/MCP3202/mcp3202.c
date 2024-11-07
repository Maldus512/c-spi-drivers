#include "mcp3202.h"
#include "spi_common/spi_common.h"


int mcp3202_read_adc_channel(spi_driver_t driver, adc_spi_channel_t ch, uint16_t *value) {
    uint8_t  request        = 0;
    uint8_t  buffer[3]      = {0};
    uint8_t  read_buffer[3] = {0};
    uint16_t reading;

    if (ch != SPI_ADC_CH1 && ch != SPI_ADC_CH2)
        return 0;

    // 0xD0 = Start bit + single ended mode
    if (ch == SPI_ADC_CH1)
        request = 0xD0;
    else
        request = 0xF0;

    SPI_CS(driver, 0);
    buffer[0] = request;
    driver.spi_exchange(buffer, read_buffer, 3, driver.user_data);

    reading = ((read_buffer[0] & 0x7) << 9) | (read_buffer[1] << 1) | ((read_buffer[2] & 0x80) >> 7);
    *value  = reading;

    SPI_CS(driver, 1);
    return 0;
}
