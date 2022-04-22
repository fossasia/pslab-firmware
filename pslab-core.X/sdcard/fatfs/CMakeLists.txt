target_sources(pslab-firmware.elf PRIVATE
  diskio.c
  fatfs_demo.c
  ff_time.c
  ff.c
  ffsystem.c
  ffunicode.c
)

target_include_directories(pslab-firmware.elf
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

