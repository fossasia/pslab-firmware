add_subdirectory(boot)

target_sources(pslab-legacy-firmware.elf PRIVATE
  Common_Functions.c
  Function.c
  Measurements.c
  proto2_main.c
  PSLAB_ADC.c
  PSLAB_I2C.c
  PSLAB_NRF.c
  PSLAB_RTC.c
  PSLAB_SPI.c
  PSLAB_UART.c
  Variables.c
  Wave_Generator.c)
