
#ifndef MAX31865_H
#define MAX31865_H

#include <stdint.h>
#include "../../../spi_common/spi_common.h"


/* RTD data, RTD current, and measurement reference
   voltage. The ITS-90 standard is used; other RTDs
   may have coefficients defined by the DIN 43760 or
   the U.S. Industrial (American) standard. */
#define RTD_A_ITS90        3.9080e-3
#define RTD_A_USINDUSTRIAL 3.9692e-3
#define RTD_A_DIN43760     3.9848e-3
#define RTD_B_ITS90        -5.870e-7
#define RTD_B_USINDUSTRIAL -5.8495e-7
#define RTD_B_DIN43760     -5.8019e-7
/* RTD coefficient C is required only for temperatures
   below 0 deg. C.  The selected RTD coefficient set
   is specified below. */
#define SELECT_RTD_HELPER(x) x
#define SELECT_RTD(x)        SELECT_RTD_HELPER(x)
#define RTD_A                SELECT_RTD(RTD_A_ITS90)
#define RTD_B                SELECT_RTD(RTD_B_ITS90)
/*
 * The reference resistor on the hardware; see the MAX31865 datasheet
 * for details.  The values 400 and 4000 Ohm are recommended values for
 * the PT100 and PT1000.
 */
#define RTD_RREF_PT100  9925 /* Ohm */
#define RTD_RREF_PT1000 4000 /* Ohm */
/*
 * The RTD resistance at 0 degrees Celcius.  For the PT100, this is 100 Ohm;
 * for the PT1000, it is 1000 Ohm.
 */
#define RTD_RESISTANCE_PT100  100  /* Ohm */
#define RTD_RESISTANCE_PT1000 1000 /* Ohm */


typedef enum {
    MAX31865_TWO_WIRES,
    MAX31865_THREE_WIRES,
    MAX31865_FOUR_WIRES,
} max31865_wires_t;


void     max31865_set_wires(spi_driver_t driver, max31865_wires_t wires);
uint8_t  max31865_status(spi_driver_t driver);
void     max31865_bias(spi_driver_t driver, int bias);
void     max31865_autoconvert(spi_driver_t driver, int autoconvert);
void     max31865_clear_fault(spi_driver_t driver);
void     max31865_set_high_threshold(spi_driver_t driver, uint16_t threshold);
void     max31865_set_low_threshold(spi_driver_t driver, uint16_t threshold);
uint16_t max31865_read_resistance(spi_driver_t driver);
double   max31865_calculate_temperature(double reference_resistance, uint16_t rtd_adc_value);
double   max31865_read_temperature(double reference_resistance, spi_driver_t driver);


#endif /* MAX31865_H */
