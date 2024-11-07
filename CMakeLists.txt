set(MODULES "spi_common")

if (CONFIG_SPI_DEVICES_MCP3202)
    list(APPEND MODULES "spi_devices/adc/MCP3202")
endif()
if (CONFIG_SPI_DEVICES_MCP23X17)
    list(APPEND MODULES "spi_devices/io/MCP23x17")
endif()
if (CONFIG_SPI_DEVICES_MCP23S08)
    list(APPEND MODULES "spi_devices/io/MCP23S08")
endif()
if (CONFIG_SPI_DEVICES_DAC102S085)
    list(APPEND MODULES "spi_devices/dac/DAC102S085")
endif()
if (CONFIG_SPI_DEVICES_MAX31865)
    list(APPEND MODULES "spi_devices/rtdd/MAX31865")
endif()
if (CONFIG_SPI_ESP_IDF_PORT)
    list(APPEND MODULES "spi_ports/esp-idf")
endif()

idf_component_register(
    SRC_DIRS ${MODULES}
    INCLUDE_DIRS .
    REQUIRES driver
    )

