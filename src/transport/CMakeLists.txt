target_sources(pslab-firmware.elf PRIVATE
  host.c
  packet_handler.c
  uart2.c
)

target_include_directories(pslab-firmware.elf
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
