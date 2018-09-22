#include "COMMANDS.h"
#include "Common_Functions.h"
#include "Wave_Generator.h"
#include "PSLAB_ADC.h"

int __attribute__((section("sine_table1"))) sineTable1[] = {
    256, 252, 249, 246, 243, 240, 237, 234, 230, 227, 224, 221, 218, 215, 212, 209, 206, 203, 200, 196, 193, 190, 187, 184, 181, 178, 175, 172, 169, 166, 164, 161, 158, 155, 152, 149, 146, 143, 141, 138, 135, 132, 130, 127, 124, 121, 119, 116, 114, 111, 108, 106, 103, 101, 98, 96, 93, 91, 89, 86, 84, 82, 79, 77, 75, 73, 70, 68, 66, 64, 62, 60, 58, 56, 54, 52, 50, 48, 47, 45, 43, 41, 40, 38, 36, 35, 33, 32, 30, 29, 27, 26, 25, 23, 22, 21, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 8, 7, 6, 6, 5, 4, 4, 3, 3, 2, 2, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 4, 4, 5, 6, 6, 7, 8, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 29, 30, 32, 33, 35, 36, 38, 40, 41, 43, 45, 47, 48, 50, 52, 54, 56, 58, 60, 62, 64, 66, 68, 70, 73, 75, 77, 79, 82, 84, 86, 89, 91, 93, 96, 98, 101, 103, 106, 108, 111, 114, 116, 119, 121, 124, 127, 130, 132, 135, 138, 141, 143, 146, 149, 152, 155, 158, 161, 164, 166, 169, 172, 175, 178, 181, 184, 187, 190, 193, 196, 200, 203, 206, 209, 212, 215, 218, 221, 224, 227, 230, 234, 237, 240, 243, 246, 249, 252, 256, 259, 262, 265, 268, 271, 274, 277, 281, 284, 287, 290, 293, 296, 299, 302, 305, 308, 311, 315, 318, 321, 324, 327, 330, 333, 336, 339, 342, 345, 347, 350, 353, 356, 359, 362, 365, 368, 370, 373, 376, 379, 381, 384, 387, 390, 392, 395, 397, 400, 403, 405, 408, 410, 413, 415, 418, 420, 422, 425, 427, 429, 432, 434, 436, 438, 441, 443, 445, 447, 449, 451, 453, 455, 457, 459, 461, 463, 464, 466, 468, 470, 471, 473, 475, 476, 478, 479, 481, 482, 484, 485, 486, 488, 489, 490, 492, 493, 494, 495, 496, 497, 498, 499, 500, 501, 502, 503, 503, 504, 505, 505, 506, 507, 507, 508, 508, 509, 509, 509, 510, 510, 510, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 510, 510, 510, 509, 509, 509, 508, 508, 507, 507, 506, 505, 505, 504, 503, 503, 502, 501, 500, 499, 498, 497, 496, 495, 494, 493, 492, 490, 489, 488, 486, 485, 484, 482, 481, 479, 478, 476, 475, 473, 471, 470, 468, 466, 464, 463, 461, 459, 457, 455, 453, 451, 449, 447, 445, 443, 441, 438, 436, 434, 432, 429, 427, 425, 422, 420, 418, 415, 413, 410, 408, 405, 403, 400, 397, 395, 392, 390, 387, 384, 381, 379, 376, 373, 370, 368, 365, 362, 359, 356, 353, 350, 347, 345, 342, 339, 336, 333, 330, 327, 324, 321, 318, 315, 311, 308, 305, 302, 299, 296, 293, 290, 287, 284, 281, 277, 274, 271, 268, 265, 262, 259
};

int __attribute__((section("sine_table2"))) sineTable2[] = {
    256, 252, 249, 246, 243, 240, 237, 234, 230, 227, 224, 221, 218, 215, 212, 209, 206, 203, 200, 196, 193, 190, 187, 184, 181, 178, 175, 172, 169, 166, 164, 161, 158, 155, 152, 149, 146, 143, 141, 138, 135, 132, 130, 127, 124, 121, 119, 116, 114, 111, 108, 106, 103, 101, 98, 96, 93, 91, 89, 86, 84, 82, 79, 77, 75, 73, 70, 68, 66, 64, 62, 60, 58, 56, 54, 52, 50, 48, 47, 45, 43, 41, 40, 38, 36, 35, 33, 32, 30, 29, 27, 26, 25, 23, 22, 21, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 8, 7, 6, 6, 5, 4, 4, 3, 3, 2, 2, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 4, 4, 5, 6, 6, 7, 8, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 29, 30, 32, 33, 35, 36, 38, 40, 41, 43, 45, 47, 48, 50, 52, 54, 56, 58, 60, 62, 64, 66, 68, 70, 73, 75, 77, 79, 82, 84, 86, 89, 91, 93, 96, 98, 101, 103, 106, 108, 111, 114, 116, 119, 121, 124, 127, 130, 132, 135, 138, 141, 143, 146, 149, 152, 155, 158, 161, 164, 166, 169, 172, 175, 178, 181, 184, 187, 190, 193, 196, 200, 203, 206, 209, 212, 215, 218, 221, 224, 227, 230, 234, 237, 240, 243, 246, 249, 252, 256, 259, 262, 265, 268, 271, 274, 277, 281, 284, 287, 290, 293, 296, 299, 302, 305, 308, 311, 315, 318, 321, 324, 327, 330, 333, 336, 339, 342, 345, 347, 350, 353, 356, 359, 362, 365, 368, 370, 373, 376, 379, 381, 384, 387, 390, 392, 395, 397, 400, 403, 405, 408, 410, 413, 415, 418, 420, 422, 425, 427, 429, 432, 434, 436, 438, 441, 443, 445, 447, 449, 451, 453, 455, 457, 459, 461, 463, 464, 466, 468, 470, 471, 473, 475, 476, 478, 479, 481, 482, 484, 485, 486, 488, 489, 490, 492, 493, 494, 495, 496, 497, 498, 499, 500, 501, 502, 503, 503, 504, 505, 505, 506, 507, 507, 508, 508, 509, 509, 509, 510, 510, 510, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 510, 510, 510, 509, 509, 509, 508, 508, 507, 507, 506, 505, 505, 504, 503, 503, 502, 501, 500, 499, 498, 497, 496, 495, 494, 493, 492, 490, 489, 488, 486, 485, 484, 482, 481, 479, 478, 476, 475, 473, 471, 470, 468, 466, 464, 463, 461, 459, 457, 455, 453, 451, 449, 447, 445, 443, 441, 438, 436, 434, 432, 429, 427, 425, 422, 420, 418, 415, 413, 410, 408, 405, 403, 400, 397, 395, 392, 390, 387, 384, 381, 379, 376, 373, 370, 368, 365, 362, 359, 356, 353, 350, 347, 345, 342, 339, 336, 333, 330, 327, 324, 321, 318, 315, 311, 308, 305, 302, 299, 296, 293, 290, 287, 284, 281, 277, 274, 271, 268, 265, 262, 259
};


int __attribute__((section("sine_table1_short"))) sineTable1_short[] = {
    32, 26, 20, 14, 9, 5, 2, 1, 0, 1, 2, 5, 9, 14, 20, 26, 32, 38, 44, 50, 55, 59, 62, 63, 64, 63, 62, 59, 55, 50, 44, 38
};

int __attribute__((section("sine_table2_short"))) sineTable2_short[] = {
    32, 26, 20, 14, 9, 5, 2, 1, 0, 1, 2, 5, 9, 14, 20, 26, 32, 38, 44, 50, 55, 59, 62, 63, 64, 63, 62, 59, 55, 50, 44, 38
};

void sqr1(uint16 wavelength, uint16 high_time, BYTE scaling) {
    /*-----------------------square wave output-----------------*/
    OC1R = high_time - 1;
    PR1 = wavelength - 1;

    OC1CON1 = 6; //Edge aligned PWM
    OC1CON1bits.OCTSEL = 4;
    T1CONbits.TCKPS = scaling & 0x3;
    OC1CON2 = 11; //01011 = Timer1 synchronizes or triggers OCx (default)
    T1CONbits.TON = 1;

    if ((scaling & 0x4) == 0)RPOR5bits.RP54R = 0x10; //square wave pin(RC6) mapped to 0b010001 (output compare 1 )
    /*-----------------------square wave output-----------------*/
}

void sqr2(uint16 wavelength, uint16 high_time, BYTE scaling) {
    /*-----------------------square wave output-----------------*/
    OC4R = high_time - 1;
    PR3 = wavelength - 1;
    OC4CON1 = 6; //Edge aligned PWM
    OC4CON1bits.OCTSEL = 1;
    T4CONbits.TCKPS = scaling;
    OC4CON2 = 13; //01101 = Timer3 synchronizes or triggers OCx (default)
    T4CONbits.TON = 1;

    RPOR5bits.RP55R = 0x13; //square wave pin(RC7) mapped to 0x010011 (output compare 4 )
    /*-----------------------square wave output-----------------*/

}

void sqr4(uint16 w, uint16 R0, uint16 R1, uint16 RS1, uint16 R2, uint16 RS2, uint16 R3, uint16 RS3, BYTE scaling) {
    /*-----------------------square wave output-----------------*/
    _DMA2IF = 0;
    _DMA2IE = 0;
    _DMA3IF = 0;
    _DMA3IE = 0;
    DMA2CONbits.CHEN = 0;
    DMA3CONbits.CHEN = 0;

    T1CONbits.TON = 0;
    if ((scaling >> 6)&1) {
        EnableComparator();
        OC1CON2 = 0x19;
        OC1CON2bits.OCTRIG = 1; //COMP2 triggers
        OC2CON2 = 0x19;
        OC2CON2bits.OCTRIG = 1;
        OC3CON2 = 0x19;
        OC3CON2bits.OCTRIG = 1;
        OC4CON2 = 0x19;
        OC4CON2bits.OCTRIG = 1;

    } else {
        OC1CON2 = 11; //01011 = Timer1 synchronizes or triggers OCx (default)
        OC2CON2 = 11; //01011 = Timer1 synchronizes or triggers OCx (default)
        OC3CON2 = 11; //01011 = Timer1 synchronizes or triggers OCx (default)
        OC4CON2 = 11; //01011 = Timer1 synchronizes or triggers OCx (default)
    }
    PR1 = w;
    OC1R = 0;
    OC1RS = R0;
    OC2R = R1;
    OC2RS = RS1;
    OC3R = R2;
    OC3RS = RS2;
    OC4R = R3;
    OC4RS = RS3;

    if ((scaling >> 5)&1) {
        OC1CON1 = 7;
        OC2CON1 = 7;
        OC3CON1 = 7;
        OC4CON1 = 7; //Continuous center aligned PWM. high when R, low when RS.
        OC1CON2bits.OCINV = 0;
        OC2CON2bits.OCINV = 0;
        OC3CON2bits.OCINV = 0;
        OC4CON2bits.OCINV = 0;
    } else {
        OC1CON1 = 4;
        OC2CON1 = 4;
        OC3CON1 = 4;
        OC4CON1 = 4; //double compare mode. one shot toggle on alternate matches of OCR, OCRS
        OC1CON2bits.OCINV = 0;
        OC2CON2bits.OCINV = (scaling >> 2)&1;
        OC3CON2bits.OCINV = (scaling >> 3)&1;
        OC4CON2bits.OCINV = (scaling >> 4)&1;
    }
    OC1CON1bits.OCTSEL = 4;
    OC2CON1bits.OCTSEL = 4;
    OC3CON1bits.OCTSEL = 4;
    OC4CON1bits.OCTSEL = 4;

    T1CONbits.TCKPS = scaling & 0x3;

    TMR1 = 0;
    T1CONbits.TON = 1;

    RPOR5bits.RP54R = 0x10; //SQR1(RC6) mapped to (output compare 1 )
    RPOR5bits.RP55R = 0x11; //SQR2(RC7) mapped to (output compare 2 )
    RPOR6bits.RP56R = 0x12; //SQR3(RC8) mapped to (output compare 3 )
    RPOR6bits.RP57R = 0x13; //SQR4(RC9) mapped to (output compare 4 )
}

void sineWave1(uint16 wavelength, BYTE highres) {
    /*-----------------------sine wave output-----------------*/
    T4CONbits.TON = 0;
    _DMA2IF = 0;
    _DMA2IE = 0;
    DMA2CONbits.CHEN = 0;
    if (highres & 1) {
        OC3R = HIGH_RES_WAVE >> 1;
        OC3RS = HIGH_RES_WAVE;
    } else {
        OC3R = LOW_RES_WAVE >> 1;
        OC3RS = LOW_RES_WAVE;
    }
    OC3CON2 = 0;
    OC3CON1 = 6; //Edge aligned PWM
    OC3CON2bits.SYNCSEL = 0x1F; //OCRS compare used for sync
    OC3CON1bits.OCTSEL = 7; //Fp used as clock


    DMA2CONbits.AMODE = 0; // Configure DMA for Register Indirect mode with post-increment
    DMA2CONbits.SIZE = 0; //word transfer
    DMA2CONbits.MODE = 0; // Configure DMA for Continuous mode
    DMA2CONbits.DIR = 1; // RAM-to-Peripheral data transfers
    DMA2PAD = (volatile uint16) &OC3R; // Point DMA to OC3R
    DMA2REQ = 0b11011; //timer 4 triggers DMA


    if (highres & 1) {
        DMA2STAH = __builtin_dmapage(&sineTable1);
        DMA2STAL = __builtin_dmaoffset(&sineTable1);
        DMA2CNT = WAVE_TABLE_FULL_LENGTH - 1; // total table size -1/  DMA requests
    } else {
        DMA2STAH = __builtin_dmapage(&sineTable1_short);
        DMA2STAL = __builtin_dmaoffset(&sineTable1_short);
        DMA2CNT = WAVE_TABLE_SHORT_LENGTH - 1; // total table size -1/  DMA requests
    }

    _DMA2IF = 0;
    _DMA2IE = 1;
    DMA2CONbits.CHEN = 1;

    T4CONbits.TCKPS = (highres >> 1)&3;
    PR4 = wavelength;
    T4CONbits.TON = 1;

    RPOR6bits.RP57R = 0x12; //Sine 1 Mapping output to square wave 4 SQR4
}

void sineWave2(uint16 wavelength, BYTE highres) {
    /*-----------------------sine wave output-----------------*/
    T3CONbits.TON = 0;
    T2CONbits.T32 = 0;
    _DMA3IF = 0;
    _DMA3IE = 0;
    DMA3CONbits.CHEN = 0;

    if (highres & 1) {
        OC4R = HIGH_RES_WAVE >> 1;
        OC4RS = HIGH_RES_WAVE;
    } else {
        OC4R = LOW_RES_WAVE >> 1;
        OC4RS = LOW_RES_WAVE;
    }

    OC4CON2 = 0;
    OC4CON1 = 6; //Edge aligned PWM
    OC4CON2bits.SYNCSEL = 0x1F; //OCRS compare used for sync
    OC4CON1bits.OCTSEL = 7; //Fp used as clock


    DMA3CONbits.AMODE = 0; // Configure DMA for Register Indirect mode with post-increment
    DMA3CONbits.SIZE = 0; //word transfer
    DMA3CONbits.MODE = 0; // Configure DMA for Continuous mode
    DMA3CONbits.DIR = 1; // RAM-to-Peripheral data transfers
    DMA3PAD = (volatile uint16) &OC4R; // Point DMA to OC4R
    DMA3REQ = 0b1000; //timer 3 triggers DMA

    if (highres & 1) {
        DMA3CNT = WAVE_TABLE_FULL_LENGTH - 1; // total table size -1/  DMA requests
        DMA3STAH = __builtin_dmapage(&sineTable2);
        DMA3STAL = __builtin_dmaoffset(&sineTable2);
    } else {
        DMA3CNT = WAVE_TABLE_SHORT_LENGTH - 1; // total table size -1/  DMA requests
        DMA3STAH = __builtin_dmapage(&sineTable2_short);
        DMA3STAL = __builtin_dmaoffset(&sineTable2_short);
    }

    _DMA3IF = 0;
    _DMA3IE = 1;
    DMA3CONbits.CHEN = 1;

    T3CONbits.TCKPS = (highres >> 1)&3;
    PR3 = wavelength;
    T3CONbits.TON = 1;

    RPOR6bits.RP56R = 0x13; //Sine 2 Mapping output to square wave 3 SQR3
}

void setSineWaves(uint16 wavelength1, uint16 wavelength2, uint16 pos, uint16 tmr_delay, BYTE highres) {
    /*-----------------------sine wave output-----------------*/
    T3CONbits.TON = 0;
    T2CONbits.T32 = 0;
    T4CONbits.TON = 0;
    _DMA3IF = 0;
    _DMA3IE = 0;
    _DMA2IF = 0;
    _DMA2IE = 0;
    DMA2CONbits.CHEN = 0;
    DMA3CONbits.CHEN = 0;

    if (highres & 1) { //highres = (prescaler1<<4)|(prescaler2<<2)|(HIGHRES2<<1)|(HIGHRES)
        OC3R = HIGH_RES_WAVE >> 1;
        OC3RS = HIGH_RES_WAVE;
    } else {
        OC3R = LOW_RES_WAVE >> 1;
        OC3RS = LOW_RES_WAVE;
    }


    if (highres & 2) {
        OC4R = HIGH_RES_WAVE >> 1;
        OC4RS = HIGH_RES_WAVE;
    } else {
        OC4R = LOW_RES_WAVE >> 1;
        OC4RS = LOW_RES_WAVE;
    }

    OC3CON2 = 0;
    OC3CON1 = 6; //Edge aligned PWM
    OC4CON2 = 0;
    OC4CON1 = 6; //Edge aligned PWM
    OC3CON2bits.SYNCSEL = 0x1F; //OCRS compare used for sync
    OC3CON1bits.OCTSEL = 7; //Fp used as clock
    OC4CON2bits.SYNCSEL = 0x1F; //OCRS compare used for sync
    OC4CON1bits.OCTSEL = 7; //Fp used as clock

    DMA2CONbits.AMODE = 0; // Configure DMA for Register Indirect mode with post-increment
    DMA2CONbits.SIZE = 0; //word transfer
    DMA2CONbits.MODE = 0; // Configure DMA for Continuous mode
    DMA2CONbits.DIR = 1; // RAM-to-Peripheral data transfers
    DMA2PAD = (volatile uint16) &OC3R; // Point DMA2 to OC3R
    DMA2REQ = 0b11011; //Timer 4 requests DMA


    DMA3CONbits.AMODE = 0; // Configure DMA for Register Indirect mode with post-increment
    DMA3CONbits.SIZE = 0; //word transfer
    DMA3CONbits.MODE = 0; // Configure DMA for Continuous mode
    DMA3CONbits.DIR = 1; // RAM-to-Peripheral data transfers
    DMA3PAD = (volatile uint16) &OC4R; // Point DMA3 to OC4R
    DMA3REQ = 0b1000; //Timer 3 requests DMA

    if (highres & 1) {
        DMA2CNT = WAVE_TABLE_FULL_LENGTH - 1; // total table size -1/  DMA requests
        DMA2STAH = __builtin_dmapage(&sineTable1);
        DMA2STAL = __builtin_dmaoffset(&sineTable1);
    } else {
        DMA2CNT = WAVE_TABLE_SHORT_LENGTH - 1; // total table size -1 [  DMA requests]
        DMA2STAH = __builtin_dmapage(&sineTable1_short);
        DMA2STAL = __builtin_dmaoffset(&sineTable1_short);
    }


    if (highres & 2) {
        DMA3CNT = WAVE_TABLE_FULL_LENGTH - 1; // total table size -1/  DMA requests
        DMA3STAH = __builtin_dmapage(&sineTable2);
        DMA3STAL = __builtin_dmaoffset(&sineTable2);
    } else {
        DMA3CNT = WAVE_TABLE_SHORT_LENGTH - 1; // total table size -1 [  DMA requests]
        DMA3STAH = __builtin_dmapage(&sineTable2_short);
        DMA3STAL = __builtin_dmaoffset(&sineTable2_short);
    }


    _DMA2IF = 0;
    _DMA2IE = 1;
    _DMA3IF = 0;
    _DMA3IE = 1;
    DMA2CONbits.CHEN = 1;
    DMA3CONbits.CHEN = 1;

    for (i = 0; i < pos; i++) {
        DMA3REQbits.FORCE = 1;
        while (DMA3REQbits.FORCE);
    } //skip these many points for wave #2
    T3CONbits.TCKPS = (highres >> 2)&3;
    T4CONbits.TCKPS = (highres >> 4)&3;
    PR4 = wavelength1;
    PR3 = wavelength2;
    TMR4 = 0;
    TMR3 = tmr_delay;

    RPOR6bits.RP56R = 0x13; //Sine 2 Mapping output to square wave 3 SQR3
    RPOR6bits.RP57R = 0x12; //Sine 1 Mapping output to square wave 4 SQR4

    T3CONbits.TON = 1;
    T4CONbits.TON = 1;
}
