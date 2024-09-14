add_subdirectory(boot)
add_subdirectory(bus)
add_subdirectory(helpers)
add_subdirectory(instruments)
add_subdirectory(registers)
add_subdirectory(sdcard)
add_subdirectory(transport)

target_sources(pslab-firmware.elf PRIVATE
  commands.c
  main.c
  states.c)
