#ifndef PSLAB_HARDWARE
#define PSLAB_HARDWARE

extern uint16_t volatile CNENB;
enum CNENB_Bits {
    CNENB_BITS_LA1 = 1 << 10,
    CNENB_BITS_LA2 = 1 << 11,
    CNENB_BITS_LA3 = 1 << 12,
    CNENB_BITS_LA4 = 1 << 13,
};

extern uint16_t volatile IEC0;

extern uint16_t volatile IEC1;
enum IEC1_Bits {
    IEC1_BITS_CNIE = 1 << 3,
};

extern uint16_t volatile IEC2;

extern uint16_t volatile IFS0;

extern uint16_t volatile IFS1;
enum IFS1_Bits {
    IFS1_BITS_CNIF = 1 << 3,
};

extern uint16_t volatile IFS2;

#endif // PSLAB_HARDWARE
