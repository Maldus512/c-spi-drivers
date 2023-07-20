# SPI Peripheral library

A collection of SPI drivers for various integrated circuits. 

The idea is to abstract away the hardware details ([the data link layer](https://en.wikipedia.org/wiki/Data_link_layer)) and create drivers that implement the API required by each device.

Nothing here is particularly complex, but it is aggregated and standardized for ease of use over multiple and diverse projects.

Similar libraries for other protocols:
- [c-i2c-drivers](https://github.com/Maldus512/c-i2c-drivers)

# How it works

The hardware is abstracted through the `spi_driver_t` structure and its 5 fields:

 - `spi_exchange`: pointer to a function that sends some data on the SPI network and simultaneusly reads the answer (full duplex communication). 
    It is hardware dependant and should be provided by the user.
 - `spi_cs_control`: pointer to a function that sets the level of the CS line for this device.
 - `user_data`: `void*` user argument that is passed to each function pointer.

## Example

The following example defines a function that reads the io register from an io expader IC.
The hardware abstraction layer should be already initialized and ready.

```
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/timers.h"

#include "spi_common/spi_common.h"
#include "spi_devices/io/MCP23x17/mcp23x17.h"
#include "spi_ports/esp-idf/esp_idf_spi_port.h"
#include "spi_devices.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"


static int  cs1_control(int level, void *user_data);


static const char *TAG = "SpiDevices";

static spi_driver_t      mcp23s17_driver                = {0};


void spi_devices_init(void) {
    gpio_config_t config = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode      = GPIO_MODE_OUTPUT,
        .pin_bit_mask = BIT64(GPIO_NUM_9) 
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en   = GPIO_PULLUP_DISABLE,
    };
    ESP_ERROR_CHECK(gpio_config(&config));

    // Deassert chip select
    cs1_control(1, NULL);

    // IO expander config
    spi_device_interface_config_t mcp23s17_devcfg = {
        .clock_speed_hz = 10UL * 1000UL * 1000UL,     // Up to 10MHz
        .mode           = 0,
        .spics_io_num   = -1,
        .queue_size     = 10,
        .pre_cb         = NULL,
        .post_cb        = NULL,
    };

    spi_device_handle_t spi1 = NULL;
    // Attach the LCD to the SPI bus
    ESP_ERROR_CHECK(spi_bus_add_device(SPI2_HOST, &mcp23s17_devcfg, &spi1));

    mcp23s17_driver.spi_exchange   = esp_idf_spi_port_exchange;
    mcp23s17_driver.spi_cs_control = cs5_control;
    mcp23s17_driver.user_data      = spi1;
}


uint16_t spi_devices_expander_get_inputs(void) {
    uint16_t reg_value = 0;
    if (mcp23x17_get_gpio_register(mcp23s17_driver, MCP23X17_DEFAULT_ADDR, &reg_value)) {
        return 0;
    } else {
        return reg_value;
    }
}


static int cs1_control(int level, void *user_data) {
    gpio_set_level(GPIO_NUM_9, level);
    return 0;
}
```

# Project Structure

There are three main folders:
 - `spi_common`: module for functions and data structures common to all device drivers. Contains the typedef `spi_driver_t`.
 - `spi_devices`: folder with all device drivers
 - `spi_ports`: optional hardware abstraction layer implementations for some common architectures.


# Documentation

As of now only fairly simple devices are included; documention should be studied from the comments in the header files.

# Building

## SCons

This library uses `SCons` as a build tool. To include it in your project just refer to the `SConscript` and expose two variables:
`spi_env` should contain the compilation environment and `spi_selected` should be a string list with the modules that should be included.

Invoking the `SConscript` returns a tuple containing the compiled library as a static archive and a list of directories that should 
be added to the include path.

## ESP-IDF

Aside from `SCons` this is also a ready-to-use ESP-IDF component. Just add this repository as a submodule in the `components` folder of your project
and configure the required module through the `KConfig` interface. 

## Other Platforms

Each device driver is composed by very few source files with no dependencies beside `spi_common`; porting any of those to another build system should be trivial.

