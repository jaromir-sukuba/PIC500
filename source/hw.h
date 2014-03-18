void eeprom_write_byte(unsigned char data, unsigned char addr);
unsigned char eeprom_read_byte (unsigned char addr);

#ifndef LED_H
#define LED_H

#include <xc.h>

/* enable PC0 as output */
#define LED_INIT TRISA2 = 0
/* led on, pin=0 */
#define LED_ON LATA2 = 1
/* led off, pin=1 */
#define LED_OFF LATA2 = 0

#endif //LED_H

#ifndef SPI_H
#define SPI_H

void spi_init(void);
unsigned char spi_set_sck_duration(unsigned char dur);
unsigned char spi_get_sck_duration(void);
unsigned char spi_mastertransmit(unsigned char data);
unsigned char spi_mastertransmit_16(unsigned int data);
unsigned char spi_mastertransmit_32(unsigned long data);
void spi_disable(void);
void spi_reset_pulse(void);
void spi_sck_pulse(void);
void dly_ms(unsigned int ms);
#endif /* SPI_H */


#ifndef UART_H
#define UART_H

void uart_init(void);
void uart_sendchar(char c);
void uart_sendstr(char *s);
unsigned char uart_getchar(unsigned char kickwd);
void uart_flushRXbuf(void);

#endif /* UART_H */

