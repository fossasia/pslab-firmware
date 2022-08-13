cmake_minimum_required(VERSION 3.16)
set(CMAKE_TOOLCHAIN_FILE external/cmake-microchip/toolchain.cmake)
set(MICROCHIP_MCU PIC24EP256GP204)

project(PSLAB_BOOTLOADER LANGUAGES C ASM)

add_executable(pslab-bootloader.elf)

add_subdirectory(src)

target_compile_options(pslab-bootloader.elf PRIVATE
  -Wall
  -Wextra
  -mno-eds-warn)

target_link_options(pslab-bootloader.elf PRIVATE
  "-Wl,-Map=pslab-bootloader.map,--report-mem")

bin2hex(pslab-bootloader.elf)
