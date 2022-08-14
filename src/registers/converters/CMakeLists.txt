target_sources(pslab-firmware.elf PRIVATE
  adc1.c
  ctmu.c
)

target_include_directories(pslab-firmware.elf
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

