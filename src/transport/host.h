#ifndef _PSLAB_HOST
#define _PSLAB_HOST

enum Status HOST_read(uint8_t *buffer, uint16_t size);
enum Status HOST_write(uint8_t const *buffer, uint16_t size);
enum Status HOST_flush_rx(void);

#endif // _PSLAB_HOST
