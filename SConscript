import os

components = [
    "io/MCP23x17",
]
ports = [
    'dummy'
]
sources = {}

Import('spi_selected', 'spi_env')

for c in components:
    sources[c] = Glob(os.path.join('./spi_devices', c, "*.c"))

for c in ports:
    sources[c] = Glob(os.path.join('./spi_ports', c, "*.c"))

objects = [spi_env.Object(x) for x in Glob('./spi_common/*.c')]

for s in spi_selected:
    objects += spi_env.Object(sources[s])

result = (objects, list(map(lambda x: os.path.join(
    os.getcwd(), x), ['.', 'spi_ports', 'spi_devices'])))
Return('result')
