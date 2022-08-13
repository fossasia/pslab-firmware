target_sources(pslab-firmware.elf PRIVATE
  tmr1.c
  tmr2.c
  tmr3.c
  tmr4.c
  tmr5.c
)

target_include_directories(pslab-firmware.elf
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

