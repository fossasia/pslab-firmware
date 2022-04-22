target_sources(pslab-firmware.elf PRIVATE
  hardware_interrupt_table.S
  interrupts.S
  memory_partition.S
  remapped_reset.S
  user_interrupt_table.S
)
