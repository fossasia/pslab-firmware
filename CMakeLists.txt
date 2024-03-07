cmake_minimum_required(VERSION 3.16)
set(CMAKE_TOOLCHAIN_FILE external/cmake-microchip/toolchain.cmake)
set(MICROCHIP_MCU PIC24EP256GP204)
set(CMAKE_SYSTEM_NAME Generic)

project(PSLAB_FIRMWARE LANGUAGES C ASM)

add_executable(pslab-firmware.elf)

add_subdirectory(src)

target_include_directories(pslab-firmware.elf
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(pslab-firmware.elf PRIVATE
  -std=c99
  -Wall
  -Wextra
  -Wno-missing-field-initializers
  -mno-eds-warn)

target_link_options(pslab-firmware.elf PRIVATE
  # Head size 0x6000 is conservative. It could be increased after a
  # proper profiling of stack usage.
  "LINKER:-Map=pslab-firmware.map,--report-mem,--heap=0x6000")

if (LEGACY_HARDWARE)
  message("Target: PSLab v5")
  add_compile_definitions(V5_HW)
else()
  message("Target: PSLab v6")
endif(LEGACY_HARDWARE)

if (ESP01)
  message("Command bus: UART2")
  add_compile_definitions(UART_PRIMARY=U2SELECT)
else()
  message("Command bus: UART1")
  add_compile_definitions(UART_PRIMARY=U1SELECT)
endif(ESP01)

bin2hex(pslab-firmware.elf)
