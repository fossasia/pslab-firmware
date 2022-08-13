target_sources(pslab-firmware.elf PRIVATE
  clock.c
  interrupt_manager.c
  pin_manager.c
  system.c
)

target_include_directories(pslab-firmware.elf
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

