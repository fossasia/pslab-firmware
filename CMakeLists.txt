cmake_minimum_required(VERSION 3.16)
set(CMAKE_TOOLCHAIN_FILE external/cmake-microchip/toolchain.cmake)
set(MICROCHIP_MCU PIC24EP256GP204)

project(PSLAB_FIRMWARE LANGUAGES C ASM)

add_executable(pslab-legacy-firmware.elf)

add_subdirectory(PSLab_Original)

target_compile_options(pslab-legacy-firmware.elf PRIVATE
  -Wall
  -Wextra
  -mno-eds-warn)

target_link_options(pslab-legacy-firmware.elf PRIVATE
  "LINKER:-Map=pslab-legacy-firmware.map,--report-mem")

bin2hex(pslab-legacy-firmware.elf)
