add_subdirectory(boot)
add_subdirectory(bus)
add_subdirectory(helpers)
add_subdirectory(instruments)
add_subdirectory(registers)
add_subdirectory(registers_ng)
add_subdirectory(sdcard)
add_subdirectory(transport)

target_sources(pslab-firmware.elf PRIVATE
  commands.c
  main.c
  states.c)

target_include_directories(pslab-firmware.elf PRIVATE
  $(CMAKE_CURRENT_SOURCE_DIR)
  $(CMAKE_CURRENT_SOURCE_DIR)/registers_ng
  $(CMAKE_CURRENT_SOURCE_DIR)/helpers
)
