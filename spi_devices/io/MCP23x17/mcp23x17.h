#ifndef MCP23X17_H_INCLUDED
#define MCP23X17_H_INCLUDED

#include "../../../spi_common/spi_common.h"

typedef enum {
    MCP23X17_OUTPUT_MODE = 0,
    MCP23X17_INPUT_MODE  = 1,
} mcp23x17_mode_t;


typedef enum {
    MCP23X17_GPIO_1 = 0,
    MCP23X17_GPIO_2,
    MCP23X17_GPIO_3,
    MCP23X17_GPIO_4,
    MCP23X17_GPIO_5,
    MCP23X17_GPIO_6,
    MCP23X17_GPIO_7,
    MCP23X17_GPIO_8,
    MCP23X17_GPIO_9,
    MCP23X17_GPIO_10,
    MCP23X17_GPIO_11,
    MCP23X17_GPIO_12,
    MCP23X17_GPIO_13,
    MCP23X17_GPIO_14,
    MCP23X17_GPIO_15,
    MCP23X17_GPIO_16,
} mcp23x17_gpio_t;


#define MCP23X17_DEFAULT_ADDR 0x40

int mcp23x17_init(spi_driver_t *driver, uint8_t address);
int mcp23x17_set_gpio_level(spi_driver_t *driver, uint8_t address, mcp23x17_gpio_t gpio, int level);
int mcp23x17_set_gpio_direction(spi_driver_t *driver, uint8_t address, mcp23x17_gpio_t gpio, mcp23x17_mode_t mode);

#endif