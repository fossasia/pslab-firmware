cmake_minimum_required(VERSION 3.16)
set(CMAKE_TOOLCHAIN_FILE external/cmake-microchip/toolchain.cmake)
set(MICROCHIP_MCU PIC24EP256GP204)

project(PSLAB_BOOTLOADER LANGUAGES C ASM)

add_executable(pslab-bootloader.elf)

# Adding sources and headers by globbing is not recommended in CMake.
# We are doing it anyway because we don't want to contaminate the
# 'mcc_generated_files' directory with files that are not generated
# by MCC.

file(GLOB_RECURSE sources
  *.c
  *.s
  *.S
  )

target_sources(pslab-bootloader.elf PRIVATE
  ${sources})

MACRO(HEADER_DIRECTORIES return_list)
  FILE(GLOB_RECURSE new_list *.h)
  SET(dir_list "")
  FOREACH(file_path ${new_list})
      GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
      SET(dir_list ${dir_list} ${dir_path})
  ENDFOREACH()
  LIST(REMOVE_DUPLICATES dir_list)
  SET(${return_list} ${dir_list})
ENDMACRO()

HEADER_DIRECTORIES(header_dir_list)

target_include_directories(pslab-bootloader.elf
  PRIVATE ${header_dir_list})

target_compile_options(pslab-bootloader.elf PRIVATE
  -Wall
  -Wextra
  -mno-eds-warn)

target_link_options(pslab-bootloader.elf PRIVATE
  "-Wl,-Map=pslab-bootloader.map,--report-mem")

bin2hex(pslab-bootloader.elf)
