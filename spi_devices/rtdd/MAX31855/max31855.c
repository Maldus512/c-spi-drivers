#include <stdint.h>
#include <math.h>
#include "../../../spi_common/spi_common.h"
#include "max31855.h"


int max31855_read(spi_driver_t driver, struct max31855_data *data) {
    uint8_t rx_data[4] = {0};

    int res = spi_exchange_data(driver, NULL, rx_data, sizeof(rx_data));
    if (res) {
        return res;
    } else {
        data->thermocouple_temperature = (int16_t)((((uint16_t)rx_data[0]) << 6) | (((uint16_t)rx_data[1] >> 2)));
        data->fault                    = (rx_data[1] & 1) > 0;
        data->internal_temperature     = (int16_t)((((uint16_t)rx_data[2]) << 4) | (((uint16_t)rx_data[3] >> 4)));
        data->short_to_vcc             = (rx_data[3] & (1 << 2)) > 0;
        data->short_to_gnd             = (rx_data[3] & (1 << 1)) > 0;
        data->open_circuit             = (rx_data[3] & (1 << 0)) > 0;

        return 0;
    }
}
