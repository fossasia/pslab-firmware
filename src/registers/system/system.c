#pragma config ICS = PGD2    //ICD Communication Channel Select bits->Communicate on PGEC2 and PGED2
#pragma config JTAGEN = OFF    //JTAG Enable bit->JTAG is disabled

// FPOR
#pragma config ALTI2C1 = OFF    //Alternate I2C1 pins->I2C1 mapped to SDA1/SCL1 pins
#pragma config ALTI2C2 = OFF    //Alternate I2C2 pins->I2C2 mapped to SDA2/SCL2 pins
#pragma config WDTWIN = WIN75    //Watchdog Window Select bits->WDT Window is 25% of WDT period

// FWDT
#pragma config WDTPOST = PS512    //Watchdog Timer Postscaler bits->1:32768
#pragma config WDTPRE = PR128    //Watchdog Timer Prescaler bit->1:128
#pragma config PLLKEN = ON    //PLL Lock Enable bit->Clock switch to PLL source will wait until the PLL lock signal is valid.
#pragma config WINDIS = OFF    //Watchdog Timer Window Enable bit->Watchdog Timer in Non-Window mode
#pragma config FWDTEN = OFF    //Watchdog Timer Enable bit->Watchdog timer enabled/disabled by user software

// FOSC
#pragma config POSCMD = HS    //Primary Oscillator Mode Select bits->HS Crystal Oscillator Mode
#pragma config OSCIOFNC = OFF    //OSC2 Pin Function bit->OSC2 is clock output
#pragma config IOL1WAY = OFF    //Peripheral pin select configuration->Allow multiple reconfigurations
#pragma config FCKSM = CSECMD    //Clock Switching Mode bits->Clock switching is enabled,Fail-safe Clock Monitor is disabled

// FOSCSEL
#pragma config FNOSC = FRC    //Oscillator Source Selection->FRC
#pragma config IESO = OFF    //Two-speed Oscillator Start-up Enable bit->Start up with user-selected oscillator source

// FGS
#pragma config GWRP = OFF    //General Segment Write-Protect bit->General Segment may be written
#pragma config GCP = OFF    //General Segment Code-Protect bit->General Segment Code protect is Disabled

#include "pin_manager.h"
#include "clock.h"
#include "system.h"
#include "system_types.h"
#include "../memory/dma.h"
#include "../converters/adc1.h"
#include "../../bus/i2c/i2c.h"
#include "../../bus/uart/uart.h"
#include "../timers/tmr1.h"
#include "../timers/tmr2.h"
#include "../timers/tmr3.h"
#include "../timers/tmr4.h"
#include "../timers/tmr5.h"
#include "../comparators/cvr.h"
#include "../comparators/cmp1.h"
#include "../comparators/cmp2.h"
#include "../comparators/cmp3.h"
#include "../comparators/cmp4.h"
#include "../comparators/ic1.h"
#include "../comparators/ic2.h"
#include "../comparators/ic3.h"
#include "../comparators/ic4.h"
#include "../comparators/oc1.h"
#include "../comparators/oc2.h"
#include "../comparators/oc3.h"
#include "../comparators/oc4.h"
#include "../../registers/system/interrupt_manager.h"
#include "../../sdcard/sd_spi.h"
#include "../../sdcard/fatfs/ff.h"
#include "../../helpers/light.h"

void SYSTEM_Initialize(void) {

    CLOCK_Initialize();

    PIN_MANAGER_Initialize();

    SYSTEM_CORCONInitialize();

    INTERRUPT_Initialize();

    UART_initialize(U1SELECT);
    UART_initialize(U2SELECT);

    ADC1_Initialize();

    CMP1_Initialize();
    CMP2_Initialize();
    CMP3_Initialize();
    CMP4_Initialize();

    TMR1_Initialize();
    TMR2_Initialize();
    TMR3_Initialize();
    TMR4_Initialize();
    TMR5_Initialize();

    IC1_Initialize();
    IC2_Initialize();
    IC3_Initialize();
    IC4_Initialize();

    I2C_Initialize();

    CVR_Initialize();

    OC1_Initialize();
    OC2_Initialize();
    OC3_Initialize();
    OC4_Initialize();

    DMA_Initialize();

    LIGHT_RGB(0, 20, 0);
    UART_flush_rx(U1SELECT);
    UART_flush_rx(U2SELECT);
}
