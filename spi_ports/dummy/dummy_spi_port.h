#ifndef DUMMY_SPI_PORT_H_INCLUDED
#define DUMMY_SPI_PORT_H_INCLUDED

#include <stdint.h>
#include <stdlib.h>

int dummy_spi_port_exchange(uint8_t *writebuf, uint8_t *readbuf, size_t length, void *data);

#endif