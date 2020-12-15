#include "../../../spi_common/spi_common.h"
#include "mcp23x17.h"

#define EXPANDER_DIRA_ADD  0x00
#define EXPANDER_DIRB_ADD  0x01
#define EXPANDER_POLA_ADD  0x02
#define EXPANDER_POLB_ADD  0x03
#define EXPANDER_GPIOA_ADD 0x12
#define EXPANDER_GPIOB_ADD 0x13


#define SET_BIT(reg, index, val) (reg & (~(1 << index))) | ((val ? 1 : 0) << index)


static int update_register_bit(spi_driver_t *driver, uint8_t devaddr, uint8_t regaddr, int index, int value);


int mcp23x17_init(spi_driver_t *driver, uint8_t address) {
    uint8_t nullify[64] = {0};

    SPI_CS(driver, 0);
    int res =
        spi_write_data(driver, nullify,
                       sizeof(nullify));     // Send some gibberish just to make sure previous sessions are finished
    SPI_CS(driver, 1);

    return res;
}


int mcp23x17_set_gpio_direction(spi_driver_t *driver, uint8_t address, mcp23x17_gpio_t gpio, mcp23x17_mode_t mode) {
    return update_register_bit(driver, address, EXPANDER_DIRA_ADD, gpio, mode);
}


int mcp23x17_set_gpio_level(spi_driver_t *driver, uint8_t address, mcp23x17_gpio_t gpio, int level) {
    return update_register_bit(driver, address, EXPANDER_GPIOA_ADD, gpio, level);
}


static int update_register_bit(spi_driver_t *driver, uint8_t devaddr, uint8_t regaddr, int index, int value) {
    uint8_t response[4] = {0};
    uint8_t command[4]  = {SPI_READ_ADDR(devaddr), regaddr, 0, 0};

    SPI_CS(driver, 0);
    int res = driver->spi_exchange(command, response, 4, driver->user_data);
    SPI_CS(driver, 1);

    if (res)
        return res;

    uint16_t current = (response[2] << 8) | response[3];
    current          = SET_BIT(current, index, value);

    command[0] = SPI_WRITE_ADDR(devaddr);
    command[2] = (current >> 8) & 0xFF;
    command[3] = current & 0xFF;

    SPI_CS(driver, 0);
    res = driver->spi_exchange(command, NULL, 4, driver->user_data);
    SPI_CS(driver, 1);

    return res;
}