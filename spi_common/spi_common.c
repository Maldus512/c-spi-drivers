#include <string.h>
#include "spi_common.h"


int spi_write_data(spi_driver_t *driver, uint8_t *data, size_t len) {
    return driver->spi_exchange(data, NULL, len, driver->user_data);
}


int spi_write_register(spi_driver_t *driver, uint8_t addr, uint8_t *data, size_t len) {
    uint8_t buffer[len + 1];
    buffer[0] = addr;
    memcpy(&buffer[1], data, len);
    return driver->spi_exchange(buffer, NULL, len + 1, driver->user_data);
}


int spi_read_register(spi_driver_t *driver, uint8_t addr, uint8_t *data, size_t len) {
    uint8_t buffer[len + 1];
    buffer[0] = addr;
    memset(&buffer[1], 0, len);

    int res = driver->spi_exchange(buffer, buffer, len + 1, driver->user_data);
    memcpy(data, &buffer[1], len);

    return res;
}