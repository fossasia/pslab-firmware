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
