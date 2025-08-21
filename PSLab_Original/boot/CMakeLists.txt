target_sources(pslab-legacy-firmware.elf PRIVATE
  hardware_interrupt_table.S
  interrupts.S
  memory_partition.S
  remapped_reset.S
  user_interrupt_table.S
)

target_include_directories(pslab-legacy-firmware.elf PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
