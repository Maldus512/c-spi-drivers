#ifndef DAC102S085_H_INCLUDED
#define DAC102S085_H_INCLUDED


#include <stdint.h>
#include "../../../spi_common/spi_common.h"


typedef enum {
    DAC102S085_DAC_CHANNEL_A = 0,
    DAC102S085_DAC_CHANNEL_B,
} dac102s085_dac_channel_t;


void dac102S085_set(spi_driver_t driver, dac102s085_dac_channel_t channel, uint16_t value);


#endif
