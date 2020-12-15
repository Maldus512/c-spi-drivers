set(MODULES "spi_common")

if (CONFIG_SPI_DEVICES_MCP23X17)
    list(APPEND MODULES "spi_devices/io/MCP23x17")
endif()
if (CONFIG_SPI_ESP_IDF_PORT)
    list(APPEND MODULES "spi_ports/esp-idf")
endif()

idf_component_register(
    SRC_DIRS ${MODULES}
    INCLUDE_DIRS .
    )

