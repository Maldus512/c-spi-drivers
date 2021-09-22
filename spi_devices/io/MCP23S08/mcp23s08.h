#ifndef MCP23S08_H_INCLUDED
#define MCP23S08_H_INCLUDED

#include "../../../spi_common/spi_common.h"

typedef enum {
    MCP23S08_OUTPUT_MODE = 0,
    MCP23S08_INPUT_MODE  = 1,
} mcp23s08_mode_t;


typedef enum {
    MCP23S08_GPIO_1 = 0,
    MCP23S08_GPIO_2,
    MCP23S08_GPIO_3,
    MCP23S08_GPIO_4,
    MCP23S08_GPIO_5,
    MCP23S08_GPIO_6,
    MCP23S08_GPIO_7,
    MCP23S08_GPIO_8,
} mcp23s08_gpio_t;


#define MCP23S08_DEFAULT_ADDR 0x40

int mcp23s08_init(spi_driver_t driver, uint8_t address, int addressable);
int mcp23s08_set_gpio_level(spi_driver_t driver, uint8_t address, mcp23s08_gpio_t gpio, int level);
int mcp23s08_set_gpio_direction(spi_driver_t driver, uint8_t address, mcp23s08_gpio_t gpio, mcp23s08_mode_t mode);
int mcp23s08_get_gpio_level(spi_driver_t driver, uint8_t address, mcp23s08_gpio_t gpio, int *level);
int mcp23s08_toggle_gpio(spi_driver_t driver, uint8_t address, mcp23s08_gpio_t gpio);
int mcp23s08_get_gpio_register(spi_driver_t driver, uint8_t address, uint8_t *reg);
int mcp23s08_set_gpio_polarity(spi_driver_t driver, uint8_t address, mcp23s08_gpio_t gpio, int inverted);
int mcp23s08_set_gpio_register(spi_driver_t driver, uint8_t address, uint8_t reg);
int mcp23s08_set_gpio_direction_register(spi_driver_t driver, uint8_t address, uint8_t reg);

#endif