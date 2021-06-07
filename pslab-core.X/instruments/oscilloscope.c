#define BUFFER_SIZE         10000

int __attribute__((section(".adcbuff"), far)) ADCbuffer[BUFFER_SIZE];
