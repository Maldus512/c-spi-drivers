#ifndef MAX31855_H_INCLUDED
#define MAX31855_H_INCLUDED


#include "../../../spi_common/spi_common.h"
#include <stdbool.h>
#include <stdint.h>


struct max31855_data {
    int16_t thermocouple_temperature;
    int16_t internal_temperature;

    bool fault;
    bool short_to_vcc;
    bool short_to_gnd;
    bool open_circuit;
};


int max31855_read(spi_driver_t driver, struct max31855_data *data);


#endif
