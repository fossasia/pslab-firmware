add_subdirectory(fatfs)

target_sources(pslab-firmware.elf PRIVATE
  sd_spi.c
  sdcard.c
)

target_include_directories(pslab-firmware.elf
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
