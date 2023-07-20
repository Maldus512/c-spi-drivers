#ifndef SPI_COMMON_H_INCLUDED
#define SPI_COMMON_H_INCLUDED

#include <stdint.h>
#include <stdlib.h>

#define SPI_READ_ADDR(addr)  (addr | 1)
#define SPI_WRITE_ADDR(addr) (addr & 0xFE)
#define SPI_CS(driver, lvl)                                                                                            \
    if (driver.spi_cs_control) {                                                                                       \
        driver.spi_cs_control(lvl, driver.user_data);                                                                  \
    }
#define SPI_CS_TAKE(driver) SPI_CS(driver, 0)
#define SPI_CS_GIVE(driver) SPI_CS(driver, 1)

#define SPI_DRIVER(ex, cs, data)     ((spi_driver_t){.spi_exchange = ex, .spi_cs_control = cs, .user_data = data})
#define SPI_DRIVER_AUTO_CS(ex, data) SPI_DRIVER(ex, NULL, data)

typedef struct {
    int (*spi_exchange)(uint8_t *writebuf, uint8_t *readbuf, size_t length, void *user_data);
    int (*spi_cs_control)(int level, void *user_data);
    void *user_data;
} spi_driver_t;

int spi_write_data(spi_driver_t driver, uint8_t *data, size_t len);
int spi_exchange_data(spi_driver_t driver, uint8_t *tx_data, uint8_t *rx_data, size_t len);

#endif
