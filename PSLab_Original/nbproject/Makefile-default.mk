#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/PSLab_Original.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/PSLab_Original.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS
SUB_IMAGE_ADDRESS_COMMAND=--image-address $(SUB_IMAGE_ADDRESS)
else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=proto2_main.c PSLAB_UART.c PSLAB_I2C.c Common_Functions.c PSLAB_NRF.c PSLAB_SPI.c PSLAB_ADC.c Wave_Generator.c Function.c Measurements.c PSLAB_RTC.c Variables.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/proto2_main.o ${OBJECTDIR}/PSLAB_UART.o ${OBJECTDIR}/PSLAB_I2C.o ${OBJECTDIR}/Common_Functions.o ${OBJECTDIR}/PSLAB_NRF.o ${OBJECTDIR}/PSLAB_SPI.o ${OBJECTDIR}/PSLAB_ADC.o ${OBJECTDIR}/Wave_Generator.o ${OBJECTDIR}/Function.o ${OBJECTDIR}/Measurements.o ${OBJECTDIR}/PSLAB_RTC.o ${OBJECTDIR}/Variables.o
POSSIBLE_DEPFILES=${OBJECTDIR}/proto2_main.o.d ${OBJECTDIR}/PSLAB_UART.o.d ${OBJECTDIR}/PSLAB_I2C.o.d ${OBJECTDIR}/Common_Functions.o.d ${OBJECTDIR}/PSLAB_NRF.o.d ${OBJECTDIR}/PSLAB_SPI.o.d ${OBJECTDIR}/PSLAB_ADC.o.d ${OBJECTDIR}/Wave_Generator.o.d ${OBJECTDIR}/Function.o.d ${OBJECTDIR}/Measurements.o.d ${OBJECTDIR}/PSLAB_RTC.o.d ${OBJECTDIR}/Variables.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/proto2_main.o ${OBJECTDIR}/PSLAB_UART.o ${OBJECTDIR}/PSLAB_I2C.o ${OBJECTDIR}/Common_Functions.o ${OBJECTDIR}/PSLAB_NRF.o ${OBJECTDIR}/PSLAB_SPI.o ${OBJECTDIR}/PSLAB_ADC.o ${OBJECTDIR}/Wave_Generator.o ${OBJECTDIR}/Function.o ${OBJECTDIR}/Measurements.o ${OBJECTDIR}/PSLAB_RTC.o ${OBJECTDIR}/Variables.o

# Source Files
SOURCEFILES=proto2_main.c PSLAB_UART.c PSLAB_I2C.c Common_Functions.c PSLAB_NRF.c PSLAB_SPI.c PSLAB_ADC.c Wave_Generator.c Function.c Measurements.c PSLAB_RTC.c Variables.c




CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/PSLab_Original.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=24EP256GP204
MP_LINKER_FILE_OPTION=,--script=p24EP256GP204.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/proto2_main.o: proto2_main.c  .generated_files/b20099725a9b3ab7a072d897c1f53246d5980c0b.flag .generated_files/f7c0ce93c58705ae075be29667b4f7f8d13d214d.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/proto2_main.o.d 
	@${RM} ${OBJECTDIR}/proto2_main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  proto2_main.c  -o ${OBJECTDIR}/proto2_main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/proto2_main.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/PSLAB_UART.o: PSLAB_UART.c  .generated_files/1d3997731d7d7b3f2c4ef268b070d6eff4d752a2.flag .generated_files/f7c0ce93c58705ae075be29667b4f7f8d13d214d.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/PSLAB_UART.o.d 
	@${RM} ${OBJECTDIR}/PSLAB_UART.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  PSLAB_UART.c  -o ${OBJECTDIR}/PSLAB_UART.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/PSLAB_UART.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/PSLAB_I2C.o: PSLAB_I2C.c  .generated_files/e58e040a3b4183c8459853bce0b5ad1422456c4.flag .generated_files/f7c0ce93c58705ae075be29667b4f7f8d13d214d.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/PSLAB_I2C.o.d 
	@${RM} ${OBJECTDIR}/PSLAB_I2C.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  PSLAB_I2C.c  -o ${OBJECTDIR}/PSLAB_I2C.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/PSLAB_I2C.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Common_Functions.o: Common_Functions.c  .generated_files/6c35f68299e8bc9ba7824b09ae59f78d65dee39a.flag .generated_files/f7c0ce93c58705ae075be29667b4f7f8d13d214d.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Common_Functions.o.d 
	@${RM} ${OBJECTDIR}/Common_Functions.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Common_Functions.c  -o ${OBJECTDIR}/Common_Functions.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Common_Functions.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/PSLAB_NRF.o: PSLAB_NRF.c  .generated_files/b9557f2318eb95aea2ae34594705a59d9ce7786f.flag .generated_files/f7c0ce93c58705ae075be29667b4f7f8d13d214d.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/PSLAB_NRF.o.d 
	@${RM} ${OBJECTDIR}/PSLAB_NRF.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  PSLAB_NRF.c  -o ${OBJECTDIR}/PSLAB_NRF.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/PSLAB_NRF.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/PSLAB_SPI.o: PSLAB_SPI.c  .generated_files/4e02aa0c1814050ed8fab5cfcccfa21a50ea9ef4.flag .generated_files/f7c0ce93c58705ae075be29667b4f7f8d13d214d.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/PSLAB_SPI.o.d 
	@${RM} ${OBJECTDIR}/PSLAB_SPI.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  PSLAB_SPI.c  -o ${OBJECTDIR}/PSLAB_SPI.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/PSLAB_SPI.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/PSLAB_ADC.o: PSLAB_ADC.c  .generated_files/9c446430167ac5908d747ec2ccf92408182c5711.flag .generated_files/f7c0ce93c58705ae075be29667b4f7f8d13d214d.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/PSLAB_ADC.o.d 
	@${RM} ${OBJECTDIR}/PSLAB_ADC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  PSLAB_ADC.c  -o ${OBJECTDIR}/PSLAB_ADC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/PSLAB_ADC.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Wave_Generator.o: Wave_Generator.c  .generated_files/727872c877939ddbb65342c0369a4b23587b5e90.flag .generated_files/f7c0ce93c58705ae075be29667b4f7f8d13d214d.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Wave_Generator.o.d 
	@${RM} ${OBJECTDIR}/Wave_Generator.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Wave_Generator.c  -o ${OBJECTDIR}/Wave_Generator.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Wave_Generator.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Function.o: Function.c  .generated_files/4be293ed99d6f4a60d04ed7d0f5f5fcd9f1f3d84.flag .generated_files/f7c0ce93c58705ae075be29667b4f7f8d13d214d.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Function.o.d 
	@${RM} ${OBJECTDIR}/Function.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Function.c  -o ${OBJECTDIR}/Function.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Function.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Measurements.o: Measurements.c  .generated_files/bfa1ca8a92b0900965683913968bb514452b7bbe.flag .generated_files/f7c0ce93c58705ae075be29667b4f7f8d13d214d.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Measurements.o.d 
	@${RM} ${OBJECTDIR}/Measurements.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Measurements.c  -o ${OBJECTDIR}/Measurements.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Measurements.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/PSLAB_RTC.o: PSLAB_RTC.c  .generated_files/c7ff724db33f16f3a231e0a8c065d013eb30b29d.flag .generated_files/f7c0ce93c58705ae075be29667b4f7f8d13d214d.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/PSLAB_RTC.o.d 
	@${RM} ${OBJECTDIR}/PSLAB_RTC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  PSLAB_RTC.c  -o ${OBJECTDIR}/PSLAB_RTC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/PSLAB_RTC.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Variables.o: Variables.c  .generated_files/7f1b1411f969222b5cde36601fb6dd56242a3403.flag .generated_files/f7c0ce93c58705ae075be29667b4f7f8d13d214d.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Variables.o.d 
	@${RM} ${OBJECTDIR}/Variables.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Variables.c  -o ${OBJECTDIR}/Variables.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Variables.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
else
${OBJECTDIR}/proto2_main.o: proto2_main.c  .generated_files/6a8ecea750e02cb8ba7bac5a029ccfcafc3170fb.flag .generated_files/f7c0ce93c58705ae075be29667b4f7f8d13d214d.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/proto2_main.o.d 
	@${RM} ${OBJECTDIR}/proto2_main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  proto2_main.c  -o ${OBJECTDIR}/proto2_main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/proto2_main.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/PSLAB_UART.o: PSLAB_UART.c  .generated_files/b001520bf5037f4b41c2264d14f4ab48aaf0fd0e.flag .generated_files/f7c0ce93c58705ae075be29667b4f7f8d13d214d.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/PSLAB_UART.o.d 
	@${RM} ${OBJECTDIR}/PSLAB_UART.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  PSLAB_UART.c  -o ${OBJECTDIR}/PSLAB_UART.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/PSLAB_UART.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/PSLAB_I2C.o: PSLAB_I2C.c  .generated_files/5d4e05acfe1639feffa62d5590012f2a7ad7c2d9.flag .generated_files/f7c0ce93c58705ae075be29667b4f7f8d13d214d.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/PSLAB_I2C.o.d 
	@${RM} ${OBJECTDIR}/PSLAB_I2C.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  PSLAB_I2C.c  -o ${OBJECTDIR}/PSLAB_I2C.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/PSLAB_I2C.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Common_Functions.o: Common_Functions.c  .generated_files/119eb21a88b6031b4527590b0eceb9d2d0a85c6.flag .generated_files/f7c0ce93c58705ae075be29667b4f7f8d13d214d.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Common_Functions.o.d 
	@${RM} ${OBJECTDIR}/Common_Functions.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Common_Functions.c  -o ${OBJECTDIR}/Common_Functions.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Common_Functions.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/PSLAB_NRF.o: PSLAB_NRF.c  .generated_files/d07505e939d831f2171d0256f857bf5fda166101.flag .generated_files/f7c0ce93c58705ae075be29667b4f7f8d13d214d.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/PSLAB_NRF.o.d 
	@${RM} ${OBJECTDIR}/PSLAB_NRF.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  PSLAB_NRF.c  -o ${OBJECTDIR}/PSLAB_NRF.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/PSLAB_NRF.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/PSLAB_SPI.o: PSLAB_SPI.c  .generated_files/80f24146015c29af017551c02a7843192c6a6aa0.flag .generated_files/f7c0ce93c58705ae075be29667b4f7f8d13d214d.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/PSLAB_SPI.o.d 
	@${RM} ${OBJECTDIR}/PSLAB_SPI.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  PSLAB_SPI.c  -o ${OBJECTDIR}/PSLAB_SPI.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/PSLAB_SPI.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/PSLAB_ADC.o: PSLAB_ADC.c  .generated_files/5013b8460164945f89b71f6718a31a913a81f5a4.flag .generated_files/f7c0ce93c58705ae075be29667b4f7f8d13d214d.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/PSLAB_ADC.o.d 
	@${RM} ${OBJECTDIR}/PSLAB_ADC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  PSLAB_ADC.c  -o ${OBJECTDIR}/PSLAB_ADC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/PSLAB_ADC.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Wave_Generator.o: Wave_Generator.c  .generated_files/ff046c15f9d075114a0f51af38c7fa2c78bb34a3.flag .generated_files/f7c0ce93c58705ae075be29667b4f7f8d13d214d.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Wave_Generator.o.d 
	@${RM} ${OBJECTDIR}/Wave_Generator.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Wave_Generator.c  -o ${OBJECTDIR}/Wave_Generator.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Wave_Generator.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Function.o: Function.c  .generated_files/677231b9c13ffea101458aae3e9a2eb2892ff873.flag .generated_files/f7c0ce93c58705ae075be29667b4f7f8d13d214d.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Function.o.d 
	@${RM} ${OBJECTDIR}/Function.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Function.c  -o ${OBJECTDIR}/Function.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Function.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Measurements.o: Measurements.c  .generated_files/eb74b9b63bdcc3e5019ffc754933091b34ada8a6.flag .generated_files/f7c0ce93c58705ae075be29667b4f7f8d13d214d.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Measurements.o.d 
	@${RM} ${OBJECTDIR}/Measurements.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Measurements.c  -o ${OBJECTDIR}/Measurements.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Measurements.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/PSLAB_RTC.o: PSLAB_RTC.c  .generated_files/7b7017eadd1a796bf02cbc0375e837df2fb0a615.flag .generated_files/f7c0ce93c58705ae075be29667b4f7f8d13d214d.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/PSLAB_RTC.o.d 
	@${RM} ${OBJECTDIR}/PSLAB_RTC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  PSLAB_RTC.c  -o ${OBJECTDIR}/PSLAB_RTC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/PSLAB_RTC.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Variables.o: Variables.c  .generated_files/8456f635b781d562777d0f2f957992894691223f.flag .generated_files/f7c0ce93c58705ae075be29667b4f7f8d13d214d.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Variables.o.d 
	@${RM} ${OBJECTDIR}/Variables.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Variables.c  -o ${OBJECTDIR}/Variables.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Variables.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/PSLab_Original.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/PSLab_Original.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)   -mreserve=data@0x1000:0x101B -mreserve=data@0x101C:0x101D -mreserve=data@0x101E:0x101F -mreserve=data@0x1020:0x1021 -mreserve=data@0x1022:0x1023 -mreserve=data@0x1024:0x1027 -mreserve=data@0x1028:0x104F   -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,--defsym=__MPLAB_DEBUGGER_PK3=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/PSLab_Original.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/PSLab_Original.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	${MP_CC_DIR}/xc16-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/PSLab_Original.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf   -mdfp="${DFP_DIR}/xc16" 
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
