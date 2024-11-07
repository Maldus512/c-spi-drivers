#ifndef MCP3202_H_INCLUDED
#define MCP3202_H_INCLUDED

#include <stdint.h>
#include "spi_common/spi_common.h"


typedef enum {
    SPI_ADC_CH1 = 1,
    SPI_ADC_CH2,
} adc_spi_channel_t;


int mcp3202_read_adc_channel(spi_driver_t driver, adc_spi_channel_t ch, uint16_t *value);


#endif
