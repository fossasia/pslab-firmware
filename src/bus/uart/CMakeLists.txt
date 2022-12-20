target_sources(pslab-firmware.elf PRIVATE
  uart.c
)

target_include_directories(pslab-firmware.elf
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
