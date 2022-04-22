target_sources(pslab-firmware.elf PRIVATE
  logicanalyzer.c
  multimeter.c
  oscilloscope.c
  powersource.c
  sensors.c
  wavegenerator.c
)

target_include_directories(pslab-firmware.elf
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

