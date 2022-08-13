/**
  @Generated 16-bit Bootloader Source File

  @Company:
    Microchip Technology Inc.

  @File Name: 
    interrupts.S

  @Summary:
    This is the interrupts.S file generated using 16-bit Bootloader

  @Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  16-bit Bootloader - 1.18.4
        Device            :  PIC24EP256GP204
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.36B
        MPLAB             :  MPLAB X v5.15
*/
/*
Copyright (c) [2012-2019] Microchip Technology Inc.  

    All rights reserved.

    You are permitted to use the accompanying software and its derivatives 
    with Microchip products. See the Microchip license agreement accompanying 
    this software, if any, for additional info regarding your rights and 
    obligations.
    
    MICROCHIP SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT 
    WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT 
    LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT 
    AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP OR ITS
    LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT 
    LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE 
    THEORY FOR ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT 
    LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES, 
    OR OTHER SIMILAR COSTS. 
    
    To the fullest extend allowed by law, Microchip and its licensors 
    liability will not exceed the amount of fees, if any, that you paid 
    directly to Microchip to use this software. 
    
    THIRD PARTY SOFTWARE:  Notwithstanding anything to the contrary, any 
    third party software accompanying this software is subject to the terms 
    and conditions of the third party's license agreement.  To the extent 
    required by third party licenses covering such third party software, 
    the terms of such license will apply in lieu of the terms provided in 
    this notice or applicable license.  To the extent the terms of such 
    third party licenses prohibit any of the restrictions described here, 
    such restrictions will not apply to such third party software.
*/
/* This file lists all of the interrupts in the system and how they will be
 * handled.  This file is use by both the hardware interrupt table and the 
 * user remap table to make sure they both stay in sync.
 *
 * IMPORTANT NOTE: Once a boot loader is deployed using this file, this file 
 * can not be modified in the end application.  Take care when selecting how to 
 * handle each interrupt at the development of the boot loader.
 *
 * Options:
 *   remap_to_application - this forwards the interrupt directly to the
 *   user application.  Each interrupt forwarded will require 2 instructions
 *   for the GOTO in the user reamapping table thus taking up 6 bytes of program
 *   memory each.  This is the safest option for all interrupts implemented in 
 *   hardware.  
 *
 *   keep_in_bootloader - this option will keep the interrupt in the boot
 *   loader.  If the interrupt function is defined, it will just to that
 *   function.  If it is not defined, it will jump to the default interrupt 
 *   handler function.
 *
 *   remap_to_application_default - this forwards the interrupt directly to the
 *   user application's default interrupt handler rather than the specified 
 *   unique interrupt vector.  This option is useful for interrupts that the
 *   application does not expect to use.  Since each interrupt forwarded by the
 *   remap_to_application option takes 2 instruction words, can get quite costly
 *   to forward all possible interrupts.  Interrupts forwarded with this option
 *   do not take any additional code space to forward.  Since multiple
 *   interrupts will be forwarded to the same vector, the user code will have to
 *   check to see which interrupt triggered to process them accordingly.  
 */
    
#ifdef REMAP_TABLE    

REMAP_TO_APPLICATION   OscillatorFail       
REMAP_TO_APPLICATION   AddressError       
REMAP_TO_APPLICATION   HardTrapError       
REMAP_TO_APPLICATION   StackError       
REMAP_TO_APPLICATION   MathError       
REMAP_TO_APPLICATION   DMACError       
REMAP_TO_APPLICATION   SoftTrapError       
REMAP_TO_APPLICATION_DEFAULT   ReservedTrap7       
REMAP_TO_APPLICATION   INT0Interrupt       
REMAP_TO_APPLICATION   IC1Interrupt       
REMAP_TO_APPLICATION   OC1Interrupt       
REMAP_TO_APPLICATION   T1Interrupt       
REMAP_TO_APPLICATION   DMA0Interrupt       
REMAP_TO_APPLICATION   IC2Interrupt       
REMAP_TO_APPLICATION   OC2Interrupt       
REMAP_TO_APPLICATION   T2Interrupt       
REMAP_TO_APPLICATION   T3Interrupt       
REMAP_TO_APPLICATION   SPI1ErrInterrupt       
REMAP_TO_APPLICATION   SPI1Interrupt       
REMAP_TO_APPLICATION   U1RXInterrupt       
REMAP_TO_APPLICATION   U1TXInterrupt       
REMAP_TO_APPLICATION   AD1Interrupt       
REMAP_TO_APPLICATION   DMA1Interrupt       
REMAP_TO_APPLICATION_DEFAULT   Interrupt15       
REMAP_TO_APPLICATION   SI2C1Interrupt       
REMAP_TO_APPLICATION   MI2C1Interrupt       
REMAP_TO_APPLICATION   CM1Interrupt       
REMAP_TO_APPLICATION   CNInterrupt       
REMAP_TO_APPLICATION   INT1Interrupt       
REMAP_TO_APPLICATION_DEFAULT   Interrupt21       
REMAP_TO_APPLICATION_DEFAULT   Interrupt22       
REMAP_TO_APPLICATION_DEFAULT   Interrupt23       
REMAP_TO_APPLICATION   DMA2Interrupt       
REMAP_TO_APPLICATION   OC3Interrupt       
REMAP_TO_APPLICATION   OC4Interrupt       
REMAP_TO_APPLICATION   T4Interrupt       
REMAP_TO_APPLICATION   T5Interrupt       
REMAP_TO_APPLICATION   INT2Interrupt       
REMAP_TO_APPLICATION   U2RXInterrupt       
REMAP_TO_APPLICATION   U2TXInterrupt       
REMAP_TO_APPLICATION   SPI2ErrInterrupt       
REMAP_TO_APPLICATION   SPI2Interrupt       
REMAP_TO_APPLICATION_DEFAULT   Interrupt34       
REMAP_TO_APPLICATION_DEFAULT   Interrupt35       
REMAP_TO_APPLICATION   DMA3Interrupt       
REMAP_TO_APPLICATION   IC3Interrupt       
REMAP_TO_APPLICATION   IC4Interrupt       
REMAP_TO_APPLICATION_DEFAULT   Interrupt39       
REMAP_TO_APPLICATION_DEFAULT   Interrupt40       
REMAP_TO_APPLICATION_DEFAULT   Interrupt41       
REMAP_TO_APPLICATION_DEFAULT   Interrupt42       
REMAP_TO_APPLICATION_DEFAULT   Interrupt43       
REMAP_TO_APPLICATION_DEFAULT   Interrupt44       
REMAP_TO_APPLICATION_DEFAULT   Interrupt45       
REMAP_TO_APPLICATION_DEFAULT   Interrupt46       
REMAP_TO_APPLICATION_DEFAULT   Interrupt47       
REMAP_TO_APPLICATION_DEFAULT   Interrupt48       
REMAP_TO_APPLICATION   SI2C2Interrupt       
REMAP_TO_APPLICATION   MI2C2Interrupt       
REMAP_TO_APPLICATION_DEFAULT   Interrupt51       
REMAP_TO_APPLICATION_DEFAULT   Interrupt52       
REMAP_TO_APPLICATION_DEFAULT   Interrupt53       
REMAP_TO_APPLICATION_DEFAULT   Interrupt54       
REMAP_TO_APPLICATION_DEFAULT   Interrupt55       
REMAP_TO_APPLICATION_DEFAULT   Interrupt56       
REMAP_TO_APPLICATION_DEFAULT   Interrupt57       
REMAP_TO_APPLICATION   QEI1Interrupt       
REMAP_TO_APPLICATION_DEFAULT   Interrupt59       
REMAP_TO_APPLICATION_DEFAULT   Interrupt60       
REMAP_TO_APPLICATION_DEFAULT   Interrupt61       
REMAP_TO_APPLICATION_DEFAULT   Interrupt62       
REMAP_TO_APPLICATION_DEFAULT   Interrupt63       
REMAP_TO_APPLICATION_DEFAULT   Interrupt64       
REMAP_TO_APPLICATION   U1ErrInterrupt       
REMAP_TO_APPLICATION   U2ErrInterrupt       
REMAP_TO_APPLICATION   CRCInterrupt       
REMAP_TO_APPLICATION_DEFAULT   Interrupt68       
REMAP_TO_APPLICATION_DEFAULT   Interrupt69       
REMAP_TO_APPLICATION_DEFAULT   Interrupt70       
REMAP_TO_APPLICATION_DEFAULT   Interrupt71       
REMAP_TO_APPLICATION_DEFAULT   Interrupt72       
REMAP_TO_APPLICATION_DEFAULT   Interrupt73       
REMAP_TO_APPLICATION_DEFAULT   Interrupt74       
REMAP_TO_APPLICATION_DEFAULT   Interrupt75       
REMAP_TO_APPLICATION_DEFAULT   Interrupt76       
REMAP_TO_APPLICATION   CTMUInterrupt       
REMAP_TO_APPLICATION_DEFAULT   Interrupt78       
REMAP_TO_APPLICATION_DEFAULT   Interrupt79       
REMAP_TO_APPLICATION_DEFAULT   Interrupt80       
REMAP_TO_APPLICATION_DEFAULT   Interrupt81       
REMAP_TO_APPLICATION_DEFAULT   Interrupt82       
REMAP_TO_APPLICATION_DEFAULT   Interrupt83       
REMAP_TO_APPLICATION_DEFAULT   Interrupt84       
REMAP_TO_APPLICATION_DEFAULT   Interrupt85       
REMAP_TO_APPLICATION_DEFAULT   Interrupt86       
REMAP_TO_APPLICATION_DEFAULT   Interrupt87       
REMAP_TO_APPLICATION_DEFAULT   Interrupt88       
REMAP_TO_APPLICATION_DEFAULT   Interrupt89       
REMAP_TO_APPLICATION_DEFAULT   Interrupt90       
REMAP_TO_APPLICATION_DEFAULT   Interrupt91       
REMAP_TO_APPLICATION_DEFAULT   Interrupt92       
REMAP_TO_APPLICATION_DEFAULT   Interrupt93       
REMAP_TO_APPLICATION_DEFAULT   Interrupt94       
REMAP_TO_APPLICATION_DEFAULT   Interrupt95       
REMAP_TO_APPLICATION_DEFAULT   Interrupt96       
REMAP_TO_APPLICATION_DEFAULT   Interrupt97       
REMAP_TO_APPLICATION_DEFAULT   Interrupt98       
REMAP_TO_APPLICATION_DEFAULT   Interrupt99       
REMAP_TO_APPLICATION_DEFAULT   Interrupt100       
REMAP_TO_APPLICATION_DEFAULT   Interrupt101       
REMAP_TO_APPLICATION_DEFAULT   Interrupt102       
REMAP_TO_APPLICATION_DEFAULT   Interrupt103       
REMAP_TO_APPLICATION_DEFAULT   Interrupt104       
REMAP_TO_APPLICATION_DEFAULT   Interrupt105       
REMAP_TO_APPLICATION_DEFAULT   Interrupt106       
REMAP_TO_APPLICATION_DEFAULT   Interrupt107       
REMAP_TO_APPLICATION_DEFAULT   Interrupt108       
REMAP_TO_APPLICATION_DEFAULT   Interrupt109       
REMAP_TO_APPLICATION_DEFAULT   Interrupt110       
REMAP_TO_APPLICATION_DEFAULT   Interrupt111       
REMAP_TO_APPLICATION_DEFAULT   Interrupt112       
REMAP_TO_APPLICATION_DEFAULT   Interrupt113       
REMAP_TO_APPLICATION_DEFAULT   Interrupt114       
REMAP_TO_APPLICATION_DEFAULT   Interrupt115       
REMAP_TO_APPLICATION_DEFAULT   Interrupt116       
REMAP_TO_APPLICATION_DEFAULT   Interrupt117       
REMAP_TO_APPLICATION_DEFAULT   Interrupt118       
REMAP_TO_APPLICATION_DEFAULT   Interrupt119       
REMAP_TO_APPLICATION_DEFAULT   Interrupt120       
REMAP_TO_APPLICATION_DEFAULT   Interrupt121       
REMAP_TO_APPLICATION_DEFAULT   Interrupt122       
REMAP_TO_APPLICATION_DEFAULT   Interrupt123       
REMAP_TO_APPLICATION_DEFAULT   Interrupt124       
REMAP_TO_APPLICATION_DEFAULT   Interrupt125       
REMAP_TO_APPLICATION_DEFAULT   Interrupt126       
REMAP_TO_APPLICATION_DEFAULT   Interrupt127       
REMAP_TO_APPLICATION_DEFAULT   Interrupt128       
REMAP_TO_APPLICATION_DEFAULT   Interrupt129       
REMAP_TO_APPLICATION_DEFAULT   Interrupt130       
REMAP_TO_APPLICATION_DEFAULT   Interrupt131       
REMAP_TO_APPLICATION_DEFAULT   Interrupt132       
REMAP_TO_APPLICATION_DEFAULT   Interrupt133       
REMAP_TO_APPLICATION_DEFAULT   Interrupt134       
REMAP_TO_APPLICATION_DEFAULT   Interrupt135       
REMAP_TO_APPLICATION_DEFAULT   Interrupt136       
REMAP_TO_APPLICATION_DEFAULT   Interrupt137       
REMAP_TO_APPLICATION_DEFAULT   Interrupt138       
REMAP_TO_APPLICATION_DEFAULT   Interrupt139       
REMAP_TO_APPLICATION_DEFAULT   Interrupt140       
REMAP_TO_APPLICATION_DEFAULT   Interrupt141       
REMAP_TO_APPLICATION   ICDInterrupt       
REMAP_TO_APPLICATION   JTAGInterrupt       
REMAP_TO_APPLICATION_DEFAULT   Interrupt144       
REMAP_TO_APPLICATION   PTGSTEPInterrupt       
REMAP_TO_APPLICATION   PTGWDTInterrupt       
REMAP_TO_APPLICATION   PTG0Interrupt       
REMAP_TO_APPLICATION   PTG1Interrupt       
REMAP_TO_APPLICATION   PTG2Interrupt       
REMAP_TO_APPLICATION   PTG3Interrupt       
REMAP_TO_APPLICATION_DEFAULT   Interrupt151       
REMAP_TO_APPLICATION_DEFAULT   Interrupt152       
REMAP_TO_APPLICATION_DEFAULT   Interrupt153       
REMAP_TO_APPLICATION_DEFAULT   Interrupt154       
REMAP_TO_APPLICATION_DEFAULT   Interrupt155       
REMAP_TO_APPLICATION_DEFAULT   Interrupt156       
REMAP_TO_APPLICATION_DEFAULT   Interrupt157       
REMAP_TO_APPLICATION_DEFAULT   Interrupt158       
REMAP_TO_APPLICATION_DEFAULT   Interrupt159       
REMAP_TO_APPLICATION_DEFAULT   Interrupt160       
REMAP_TO_APPLICATION_DEFAULT   Interrupt161       
REMAP_TO_APPLICATION_DEFAULT   Interrupt162       
REMAP_TO_APPLICATION_DEFAULT   Interrupt163       
REMAP_TO_APPLICATION_DEFAULT   Interrupt164       
REMAP_TO_APPLICATION_DEFAULT   Interrupt165       
REMAP_TO_APPLICATION_DEFAULT   Interrupt166       
REMAP_TO_APPLICATION_DEFAULT   Interrupt167       
REMAP_TO_APPLICATION_DEFAULT   Interrupt168       
REMAP_TO_APPLICATION_DEFAULT   Interrupt169       
REMAP_TO_APPLICATION_DEFAULT   Interrupt170       
REMAP_TO_APPLICATION_DEFAULT   Interrupt171       
REMAP_TO_APPLICATION_DEFAULT   Interrupt172       
REMAP_TO_APPLICATION_DEFAULT   Interrupt173       
REMAP_TO_APPLICATION_DEFAULT   Interrupt174       
REMAP_TO_APPLICATION_DEFAULT   Interrupt175       
REMAP_TO_APPLICATION_DEFAULT   Interrupt176       
REMAP_TO_APPLICATION_DEFAULT   Interrupt177       
REMAP_TO_APPLICATION_DEFAULT   Interrupt178       
REMAP_TO_APPLICATION_DEFAULT   Interrupt179       
REMAP_TO_APPLICATION_DEFAULT   Interrupt180       
REMAP_TO_APPLICATION_DEFAULT   Interrupt181       
REMAP_TO_APPLICATION_DEFAULT   Interrupt182       
REMAP_TO_APPLICATION_DEFAULT   Interrupt183       
REMAP_TO_APPLICATION_DEFAULT   Interrupt184       
REMAP_TO_APPLICATION_DEFAULT   Interrupt185       
REMAP_TO_APPLICATION_DEFAULT   Interrupt186       
REMAP_TO_APPLICATION_DEFAULT   Interrupt187       
REMAP_TO_APPLICATION_DEFAULT   Interrupt188       
REMAP_TO_APPLICATION_DEFAULT   Interrupt189       
REMAP_TO_APPLICATION_DEFAULT   Interrupt190       
REMAP_TO_APPLICATION_DEFAULT   Interrupt191       
REMAP_TO_APPLICATION_DEFAULT   Interrupt192       
REMAP_TO_APPLICATION_DEFAULT   Interrupt193       
REMAP_TO_APPLICATION_DEFAULT   Interrupt194       
REMAP_TO_APPLICATION_DEFAULT   Interrupt195       
REMAP_TO_APPLICATION_DEFAULT   Interrupt196       
REMAP_TO_APPLICATION_DEFAULT   Interrupt197       
REMAP_TO_APPLICATION_DEFAULT   Interrupt198       
REMAP_TO_APPLICATION_DEFAULT   Interrupt199       
REMAP_TO_APPLICATION_DEFAULT   Interrupt200       
REMAP_TO_APPLICATION_DEFAULT   Interrupt201       
REMAP_TO_APPLICATION_DEFAULT   Interrupt202       
REMAP_TO_APPLICATION_DEFAULT   Interrupt203       
REMAP_TO_APPLICATION_DEFAULT   Interrupt204       
REMAP_TO_APPLICATION_DEFAULT   Interrupt205       
REMAP_TO_APPLICATION_DEFAULT   Interrupt206       
REMAP_TO_APPLICATION_DEFAULT   Interrupt207       
REMAP_TO_APPLICATION_DEFAULT   Interrupt208       
REMAP_TO_APPLICATION_DEFAULT   Interrupt209       
REMAP_TO_APPLICATION_DEFAULT   Interrupt210       
REMAP_TO_APPLICATION_DEFAULT   Interrupt211       
REMAP_TO_APPLICATION_DEFAULT   Interrupt212       
REMAP_TO_APPLICATION_DEFAULT   Interrupt213       
REMAP_TO_APPLICATION_DEFAULT   Interrupt214       
REMAP_TO_APPLICATION_DEFAULT   Interrupt215       
REMAP_TO_APPLICATION_DEFAULT   Interrupt216       
REMAP_TO_APPLICATION_DEFAULT   Interrupt217       
REMAP_TO_APPLICATION_DEFAULT   Interrupt218       
REMAP_TO_APPLICATION_DEFAULT   Interrupt219       
REMAP_TO_APPLICATION_DEFAULT   Interrupt220       
REMAP_TO_APPLICATION_DEFAULT   Interrupt221       
REMAP_TO_APPLICATION_DEFAULT   Interrupt222       
REMAP_TO_APPLICATION_DEFAULT   Interrupt223       
REMAP_TO_APPLICATION_DEFAULT   Interrupt224       
REMAP_TO_APPLICATION_DEFAULT   Interrupt225       
REMAP_TO_APPLICATION_DEFAULT   Interrupt226       
REMAP_TO_APPLICATION_DEFAULT   Interrupt227       
REMAP_TO_APPLICATION_DEFAULT   Interrupt228       
REMAP_TO_APPLICATION_DEFAULT   Interrupt229       
REMAP_TO_APPLICATION_DEFAULT   Interrupt230       
REMAP_TO_APPLICATION_DEFAULT   Interrupt231       
REMAP_TO_APPLICATION_DEFAULT   Interrupt232       
REMAP_TO_APPLICATION_DEFAULT   Interrupt233       
REMAP_TO_APPLICATION_DEFAULT   Interrupt234       
REMAP_TO_APPLICATION_DEFAULT   Interrupt235       
REMAP_TO_APPLICATION_DEFAULT   Interrupt236       
REMAP_TO_APPLICATION_DEFAULT   Interrupt237       
REMAP_TO_APPLICATION_DEFAULT   Interrupt238       
REMAP_TO_APPLICATION_DEFAULT   Interrupt239       
REMAP_TO_APPLICATION_DEFAULT   Interrupt240       
REMAP_TO_APPLICATION_DEFAULT   Interrupt241       
REMAP_TO_APPLICATION_DEFAULT   Interrupt242       
REMAP_TO_APPLICATION_DEFAULT   Interrupt243       
REMAP_TO_APPLICATION_DEFAULT   Interrupt244       
REMAP_TO_APPLICATION_DEFAULT   Interrupt245       

#endif
