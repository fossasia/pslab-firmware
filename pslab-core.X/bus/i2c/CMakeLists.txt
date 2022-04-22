target_sources(pslab-firmware.elf PRIVATE
  i2c.c
)

target_include_directories(pslab-firmware.elf
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
