cmake_minimum_required(VERSION 3.16)
set(CMAKE_TOOLCHAIN_FILE external/cmake-microchip/toolchain.cmake)
set(MICROCHIP_MCU PIC24EP256GP204)

project(PSLAB_FIRMWARE LANGUAGES C ASM)

add_executable(pslab-firmware.elf)

add_subdirectory(src)

target_include_directories(pslab-firmware.elf
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(pslab-firmware.elf PRIVATE
  -Wall
  -Wextra
  -mno-eds-warn)

target_link_options(pslab-firmware.elf PRIVATE
  "LINKER:-Map=pslab-firmware.map,--report-mem")

if (LEGACY_HARDWARE)
  add_compile_definitions(V5_HW)
endif(LEGACY_HARDWARE)

bin2hex(pslab-firmware.elf)

