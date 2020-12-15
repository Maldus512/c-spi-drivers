#include <string.h>
#include "driver/spi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "../../spi_common/spi_common.h"

static const char *TAG = "SPI PORT";


int esp_idf_spi_port_exchange(uint8_t *writebuf, uint8_t *readbuf, size_t length, void *data) {
    esp_err_t           ret;
    spi_transaction_t   t   = {0};
    spi_device_handle_t spi = (spi_device_handle_t)data;

    t.flags = SPI_TRANS_USE_RXDATA | SPI_TRANS_USE_TXDATA;
    memset(&t, 0, sizeof(t));     // Zero out the transaction
    t.length    = 8 * len;        // Command is 8 bits
    t.tx_buffer = writebuf;
    t.rx_buffer = readbuf;

    spi_device_acquire_bus(spi, portMAX_DELAY);
    ret = spi_device_polling_transmit(spi, &t);     // Transmit!
    spi_device_release_bus(spi);

    ESP_ERROR_CHECK(ret);

    return ret;
}