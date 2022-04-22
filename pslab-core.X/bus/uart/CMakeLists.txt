target_sources(pslab-firmware.elf PRIVATE
  uart1.c
  uart2.c
)

target_include_directories(pslab-firmware.elf
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
