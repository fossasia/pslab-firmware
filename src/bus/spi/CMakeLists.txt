target_sources(pslab-firmware.elf PRIVATE
  spi_driver.c
  spi_master.c
)

target_include_directories(pslab-firmware.elf
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

