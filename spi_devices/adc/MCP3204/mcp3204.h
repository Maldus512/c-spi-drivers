#ifndef MCP3204_H_INCLUDED
#define MCP3204_H_INCLUDED

#include <stdint.h>
#include "spi_common/spi_common.h"


typedef enum {
    MCP3204_CHANNEL_CH0 = 0,
    MCP3204_CHANNEL_CH1,
    MCP3204_CHANNEL_CH2,
    MCP3204_CHANNEL_CH3,
} mcp3204_channel_t;


typedef enum {
    MCP3204_INPUT_MODE_SINGLE_ENDED = 0,
    MCP3204_INPUT_MODE_DIFFERENTIAL,
} mcp2304_input_mode_t;


int mcp3204_read_adc_channel(spi_driver_t driver, mcp2304_input_mode_t input_mode, mcp3204_channel_t ch,
                             uint16_t *value);


#endif
