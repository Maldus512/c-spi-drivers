#include <stdint.h>
#include <math.h>
#include "../../../spi_common/spi_common.h"

#include "max31865.h"


#define MAX32865_SPI_CS 17


#define CONFIG_REGISTER       0x00
#define RTD_MSB_REGISTER      0x01
#define RTD_LSB_REGISTER      0x02
#define HIGH_FAULT_THRESH_MSB 0x03
#define HIGH_FAULT_THRESH_LSB 0x04
#define LOW_FAULT_THRESH_MSB  0x05
#define LOW_FAULT_THRESH_LSB  0x06
#define FAULT_REGISTER        0x07

#define CONFIG_THREE_WIRES        0x10
#define CONFIG_BIAS               0x80
#define CONFIG_AUTOCONVERT        0x40
#define CONFIG_ONESHOT            0x20
#define CONFIG_FAULT_CLEAR        0x02
#define CONFIG_FAULT_CONTROL_MASK 0x0C


static uint8_t  read_register_8(spi_driver_t driver, uint8_t addr);
static uint16_t read_register_16(spi_driver_t driver, uint8_t addr);
static void     write_register_8(spi_driver_t driver, uint8_t addr, uint8_t data);
static void     write_register_16(spi_driver_t driver, uint8_t addr, uint16_t data);


void max31865_set_wires(spi_driver_t driver, max31865_wires_t wires) {
    uint8_t config = read_register_8(driver, CONFIG_REGISTER);
    switch (wires) {
        case MAX31865_THREE_WIRES:
            config |= CONFIG_THREE_WIRES;
            break;

        case MAX31865_TWO_WIRES:
        case MAX31865_FOUR_WIRES:
            config &= ~CONFIG_THREE_WIRES;
            break;
    }

    write_register_8(driver, CONFIG_REGISTER, config);
}


uint8_t max31865_status(spi_driver_t driver) {
    return read_register_8(driver, FAULT_REGISTER);
}


void max31865_bias(spi_driver_t driver, int bias) {
    uint8_t config = read_register_8(driver, CONFIG_REGISTER);

    if (bias)
        config |= CONFIG_BIAS;
    else
        config &= ~CONFIG_BIAS;

    write_register_8(driver, CONFIG_REGISTER, config);
}


void max31865_autoconvert(spi_driver_t driver, int autoconvert) {
    uint8_t config = read_register_8(driver, CONFIG_REGISTER);

    if (autoconvert)
        config |= CONFIG_AUTOCONVERT;
    else
        config &= ~CONFIG_AUTOCONVERT;

    write_register_8(driver, CONFIG_REGISTER, config);
}


void max31865_clear_fault(spi_driver_t driver) {
    uint8_t config = read_register_8(driver, CONFIG_REGISTER);

    config &= ~(CONFIG_FAULT_CONTROL_MASK | CONFIG_ONESHOT);
    config |= CONFIG_FAULT_CLEAR;

    write_register_8(driver, CONFIG_REGISTER, config);
}


void max31865_set_high_threshold(spi_driver_t driver, uint16_t threshold) {
    write_register_16(driver, HIGH_FAULT_THRESH_MSB, threshold);
}


void max31865_set_low_threshold(spi_driver_t driver, uint16_t threshold) {
    write_register_16(driver, LOW_FAULT_THRESH_MSB, threshold);
}


uint16_t max31865_read_resistance(spi_driver_t driver) {
    return (read_register_16(driver, RTD_MSB_REGISTER) >> 1);
}


double max31865_calculate_temperature(double reference_resistance, uint16_t rtd_adc_value) {
    // Each step of the calculation refers to a page of the datasheet (MAX31865-775735 19-6478; Rev 3; 7/15)
    const unsigned long RTD_ADC_RESOLUTION = (1u << 15); /* 15 bits */

    double RTDnominal  = 1000.0;
    double Z1, Z2, Z3, Z4, Rt, temp;

    // Initial equation, page 19 
    Rt = (double)rtd_adc_value;
    Rt *= reference_resistance;
    Rt /= RTD_ADC_RESOLUTION;

    Z1 = -RTD_A;
    Z2 = RTD_A * RTD_A - (4 * RTD_B);
    Z3 = (4 * RTD_B) / RTDnominal;
    Z4 = 2 * RTD_B;

    temp = Z2 + (Z3 * Rt);
    temp = (sqrt(temp) + Z1) / Z4;

    if (temp >= 0)
        return temp;

    // ugh.
    Rt /= RTDnominal;
    //TODO: check if this should be 100
    Rt *= 100;     // normalize to 100 ohm

    double rpoly = Rt;

    temp = -242.02;
    temp += 2.2228 * rpoly;
    rpoly *= Rt;     // square
    temp += 2.5859e-3 * rpoly;
    rpoly *= Rt;     // ^3
    temp -= 4.8260e-6 * rpoly;
    rpoly *= Rt;     // ^4
    temp -= 2.8183e-8 * rpoly;
    rpoly *= Rt;     // ^5
    temp += 1.5243e-10 * rpoly;

    return temp;
}


double max31865_read_temperature(double reference_resistance, spi_driver_t driver) {
    return max31865_calculate_temperature(reference_resistance, max31865_read_resistance(driver));
}


static void write_register_8(spi_driver_t driver, uint8_t addr, uint8_t data) {
    uint8_t tx_data[2] = {addr | 0x80, data};
    spi_write_data(driver, tx_data, sizeof(tx_data));
}


static void write_register_16(spi_driver_t driver, uint8_t addr, uint16_t data) {
    uint8_t tx_data[3] = {addr | 0x80, (data >> 8) & 0xFF, data & 0xFF};
    spi_write_data(driver, tx_data, sizeof(tx_data));
}


static uint8_t read_register_8(spi_driver_t driver, uint8_t address) {
    uint8_t tx_data[2] = {address, 0};
    uint8_t rx_data[2] = {0};

    spi_exchange_data(driver, tx_data, rx_data, 2);

    return rx_data[1];
}


static uint16_t read_register_16(spi_driver_t driver, uint8_t addr) {
    uint8_t tx_data[3] = {addr, 0, 0};
    uint8_t rx_data[3] = {0};

    spi_exchange_data(driver, tx_data, rx_data, 3);

    return (rx_data[1] << 8) | (rx_data[2]);
}
