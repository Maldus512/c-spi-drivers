#include "../../../spi_common/spi_common.h"
#include "mcp23s08.h"

#define EXPANDER_DIR_ADD   0x00
#define EXPANDER_POL_ADD   0x01
#define EXPANDER_IOCON_ADD 0x05
#define EXPANDER_GPIO_ADD  0x09

#define HAEN_INDEX 3


#define SET_BIT(reg, index, val) (reg & (~(1 << index))) | ((val ? 1 : 0) << index)


static int update_single_register_bit(spi_driver_t driver, uint8_t devaddr, uint8_t regaddr, int index, int value);


int mcp23s08_init(spi_driver_t driver, uint8_t address, int addressable) {
    uint8_t nullify[64] = {0};

    // Send some gibberish just to make sure previous sessions are finished
    int res = spi_write_data(driver, nullify, sizeof(nullify));

    if (res) {
        return res;
    }

    if (addressable) {
        return update_single_register_bit(driver, address, EXPANDER_IOCON_ADD, HAEN_INDEX, 1);
    }

    return 0;
}


int mcp23s08_set_gpio_polarity(spi_driver_t driver, uint8_t address, mcp23s08_gpio_t gpio, int inverted) {
    return update_single_register_bit(driver, address, EXPANDER_POL_ADD, gpio, inverted > 0);
}


int mcp23s08_set_gpio_direction(spi_driver_t driver, uint8_t address, mcp23s08_gpio_t gpio, mcp23s08_mode_t mode) {
    return update_single_register_bit(driver, address, EXPANDER_DIR_ADD, gpio, mode);
}


int mcp23s08_toggle_gpio(spi_driver_t driver, uint8_t address, mcp23s08_gpio_t gpio) {
    int level = 0;
    int res   = mcp23s08_get_gpio_level(driver, address, gpio, &level);

    if (res) {
        return res;
    }

    return update_single_register_bit(driver, address, EXPANDER_GPIO_ADD, gpio, !level);
}


int mcp23s08_set_gpio_direction_register(spi_driver_t driver, uint8_t address, uint8_t reg) {
    uint8_t command[] = {SPI_WRITE_ADDR(address), EXPANDER_DIR_ADD, reg};

    SPI_CS(driver, 0);
    int res = driver.spi_exchange(command, NULL, sizeof(command) / sizeof(command[0]), driver.user_data);
    SPI_CS(driver, 1);

    return res;
}


int mcp23s08_set_gpio_polarity_register(spi_driver_t driver, uint8_t address, uint8_t reg) {
    uint8_t command[] = {SPI_WRITE_ADDR(address), EXPANDER_POL_ADD, reg};

    SPI_CS(driver, 0);
    int res = driver.spi_exchange(command, NULL, sizeof(command) / sizeof(command[0]), driver.user_data);
    SPI_CS(driver, 1);

    return res;
}


int mcp23s08_set_gpio_register(spi_driver_t driver, uint8_t address, uint8_t reg) {
    uint8_t command[] = {SPI_WRITE_ADDR(address), EXPANDER_GPIO_ADD, reg};

    SPI_CS(driver, 0);
    int res = driver.spi_exchange(command, NULL, sizeof(command) / sizeof(command[0]), driver.user_data);
    SPI_CS(driver, 1);

    return res;
}


int mcp23s08_get_gpio_register(spi_driver_t driver, uint8_t address, uint8_t *reg) {
    uint8_t command[] = {SPI_READ_ADDR(address), EXPANDER_GPIO_ADD, 0};

    uint8_t response[sizeof(command) / sizeof(command[0])] = {0};

    SPI_CS(driver, 0);
    int res = driver.spi_exchange(command, response, sizeof(command) / sizeof(command[0]), driver.user_data);
    SPI_CS(driver, 1);

    *reg = response[2];

    return res;
}


int mcp23s08_get_gpio_level(spi_driver_t driver, uint8_t address, mcp23s08_gpio_t gpio, int *level) {
    uint8_t current = 0;
    int     res     = mcp23s08_get_gpio_register(driver, address, &current);

    *level = (current & (1 << gpio)) > 0;

    return res;
}


int mcp23s08_set_gpio_level(spi_driver_t driver, uint8_t address, mcp23s08_gpio_t gpio, int level) {
    return update_single_register_bit(driver, address, EXPANDER_GPIO_ADD, gpio, level);
}


static int update_single_register_bit(spi_driver_t driver, uint8_t devaddr, uint8_t regaddr, int index, int value) {
    uint8_t command[] = {SPI_READ_ADDR(devaddr), regaddr, 0};

    uint8_t response[sizeof(command) / sizeof(command[0])] = {0};

    SPI_CS(driver, 0);
    int res = driver.spi_exchange(command, response, sizeof(command) / sizeof(command[0]), driver.user_data);
    SPI_CS(driver, 1);

    if (res) {
        return res;
    }

    uint8_t current = response[2];
    current         = SET_BIT(current, index, value);

    command[0] = SPI_WRITE_ADDR(devaddr);
    command[2] = current;

    SPI_CS(driver, 0);
    res = driver.spi_exchange(command, NULL, sizeof(command) / sizeof(command[0]), driver.user_data);
    SPI_CS(driver, 1);

    return res;
}
