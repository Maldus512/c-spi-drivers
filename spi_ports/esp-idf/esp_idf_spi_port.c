#include <string.h>
#include "driver/spi_master.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_log.h"

#include "../../spi_common/spi_common.h"

static const char *TAG = "SPI PORT";

int esp_idf_spi_port_exchange(uint8_t *writebuf, uint8_t *readbuf, size_t length, void *data) {
    esp_err_t           ret;
    spi_transaction_t   t   = {0};
    spi_device_handle_t spi = (spi_device_handle_t)data;

    memset(&t, 0, sizeof(t));     // Zero out the transaction
    t.length    = 8 * length;     // Command is 8 bits
    t.tx_buffer = writebuf;
    t.rx_buffer = readbuf;

    ret = spi_device_polling_transmit(spi, &t);     // Transmit!

    ESP_ERROR_CHECK(ret);

    return ret;
}