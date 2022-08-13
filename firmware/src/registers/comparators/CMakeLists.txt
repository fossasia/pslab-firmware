target_sources(pslab-firmware.elf PRIVATE
  cmp_params.c
  cmp1.c
  cmp2.c
  cmp3.c
  cmp4.c
  cvr.c
  ic_params.c
  ic1.c
  ic2.c
  ic3.c
  ic4.c
  oc1.c
  oc2.c
  oc3.c
  oc4.c
)

target_include_directories(pslab-firmware.elf
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

