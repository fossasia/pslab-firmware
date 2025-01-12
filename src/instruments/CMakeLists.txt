target_sources(pslab-firmware.elf PRIVATE
  logic_analyzer.c
  multimeter.c
  oscilloscope.c
  powersource.c
  sensors.c
  wavegenerator.c
)

target_include_directories(pslab-firmware.elf
  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
