target_sources(pslab-firmware.elf PRIVATE
  ctmu.c
)

target_include_directories(pslab-firmware.elf
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
