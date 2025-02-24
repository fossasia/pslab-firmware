target_sources(pslab-firmware.elf PRIVATE
  buffer.c
  delay.c
  device.c
  interval.c
  light.c
  rtc.c
)

target_include_directories(pslab-firmware.elf
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
