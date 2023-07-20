#ifndef MCP23X17_H_INCLUDED
#define MCP23X17_H_INCLUDED

#include "../../../spi_common/spi_common.h"


#define MCP23X17_INPUT_BIT(b)  (1U << b)
#define MCP23X17_OUTPUT_BIT(b) (0U << b)


typedef enum {
    MCP23X17_OUTPUT_MODE = 0,
    MCP23X17_INPUT_MODE  = 1,
} mcp23x17_mode_t;


typedef enum {
    MCP23X17_GPIO_B0 = 0,
    MCP23X17_GPIO_B1,
    MCP23X17_GPIO_B2,
    MCP23X17_GPIO_B3,
    MCP23X17_GPIO_B4,
    MCP23X17_GPIO_B5,
    MCP23X17_GPIO_B6,
    MCP23X17_GPIO_B7,
    MCP23X17_GPIO_A0,
    MCP23X17_GPIO_A1,
    MCP23X17_GPIO_A2,
    MCP23X17_GPIO_A3,
    MCP23X17_GPIO_A4,
    MCP23X17_GPIO_A5,
    MCP23X17_GPIO_A6,
    MCP23X17_GPIO_A7,
} mcp23x17_gpio_t;


#define MCP23X17_DEFAULT_ADDR 0x40

int mcp23x17_init(spi_driver_t driver, uint8_t address, int addressable);
int mcp23x17_set_gpio_level(spi_driver_t driver, uint8_t address, mcp23x17_gpio_t gpio, int level);
int mcp23x17_set_gpio_direction(spi_driver_t driver, uint8_t address, mcp23x17_gpio_t gpio, mcp23x17_mode_t mode);
int mcp23x17_get_gpio_level(spi_driver_t driver, uint8_t address, mcp23x17_gpio_t gpio, int *level);
int mcp23x17_toggle_gpio(spi_driver_t driver, uint8_t address, mcp23x17_gpio_t gpio);
int mcp23x17_get_gpio_register(spi_driver_t driver, uint8_t address, uint16_t *reg);
int mcp23x17_set_gpio_polarity(spi_driver_t driver, uint8_t address, mcp23x17_gpio_t gpio, int inverted);
int mcp23x17_set_gpio_register(spi_driver_t driver, uint8_t address, uint16_t reg);
int mcp23x17_set_gpio_direction_register(spi_driver_t driver, uint8_t address, uint16_t reg);
int mcp23x17_get_gpio_direction_register(spi_driver_t driver, uint8_t address, uint16_t *reg);
int mcp23x17_get_iocon_register(spi_driver_t driver, uint8_t address, uint8_t *reg);
int mcp23x17_set_iocon_register(spi_driver_t driver, uint8_t address, uint8_t reg);

#endif
