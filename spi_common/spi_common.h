#ifndef SPI_COMMON_H_INCLUDED
#define SPI_COMMON_H_INCLUDED

#include <stdint.h>
#include <stdlib.h>

#define SPI_READ_ADDR(addr)  (addr | 1)
#define SPI_WRITE_ADDR(addr) (addr & 0xFE)
#define SPI_CS(driver, lvl)                                                                                            \
    if (driver.spi_cs_control)                                                                                        \
        driver.spi_cs_control(lvl);

#define SPI_DRIVER(ex, cs, data)     ((spi_driver_t){.spi_exchange = ex, .spi_cs_control = cs, .user_data = data})
#define SPI_DRIVER_AUTO_CS(ex, data) SPI_DRIVER(ex, NULL, data)

typedef struct {
    int (*spi_exchange)(uint8_t *writebuf, uint8_t *readbuf, size_t length, void *data);
    int (*spi_cs_control)(int level);
    void *user_data;
} spi_driver_t;

int spi_write_data(spi_driver_t driver, uint8_t *data, size_t len);

#endif