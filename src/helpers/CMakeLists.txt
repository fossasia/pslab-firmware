target_sources(pslab-firmware.elf PRIVATE
  delay.c
  device.c
  light.c
  rtc.c
)

target_include_directories(pslab-firmware.elf
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
