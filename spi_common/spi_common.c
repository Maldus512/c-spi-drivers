#include <string.h>
#include "spi_common.h"


int spi_write_data(spi_driver_t driver, uint8_t *data, size_t len) {
    return spi_exchange_data(driver, data, NULL, len);
}


int spi_exchange_data(spi_driver_t driver, uint8_t *tx_data, uint8_t *rx_data, size_t len) {
    SPI_CS_TAKE(driver);
    int res = driver.spi_exchange(tx_data, rx_data, len, driver.user_data);
    SPI_CS_GIVE(driver);
    return res;
}
