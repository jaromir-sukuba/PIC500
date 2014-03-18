/*****************************************************
* Hardware functions for STK500, plaform dependant.
* Derived from Guido Socher's work
* by Jaromir Sukuba http://jaromir.xf.cz/, Date: 05 June 2013
* 
* 
*****************************************************/

// CONFIG1
#pragma config FOSC = XT        // Oscillator Selection (XT Oscillator, Crystal/resonator connected between OSC1 and OSC2 pins)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = ON        // Internal/External Switchover (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config PLLEN = ON       // PLL Enable (4x PLL enabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)



#include <xc.h>

#define	_XTAL_FREQ	(3638400*4)
#define	RST		LATC3
#define	RST_T	TRISC3


static unsigned char sck_dur=3;

void dly_ms(unsigned int ms)
{
unsigned int i;
for (i=0;i<ms;i++) __delay_ms(1);
}

//--------------------------------------------------------------

unsigned char  spi_set_sck_duration(unsigned char dur)
{
        if (dur >= 4){
            sck_dur=12;
            SSP1ADD = 255;
        	return(sck_dur);
        }
        if (dur >= 3){
        	// 28KHz
        	SSP1ADD = 128;
            sck_dur=3;
        	return(sck_dur);
        }
        if (dur >= 2){
        	// 57KHz
        	SSP1ADD = 63;
            sck_dur=2;
        	return(sck_dur);
        }
        if (dur == 1){
        	// 230KHz
        	SSP1ADD = 15;
            sck_dur=1;
        	return(sck_dur);
        }
        if (dur == 0){
                // 921KHz
                SSP1ADD = 3;
                 sck_dur=0;
                return(sck_dur);
        }
        return(1); // we should never come here
}

unsigned char spi_get_sck_duration(void)
{
    return(sck_dur);
}

void spi_sck_pulse(void)
{
}

void spi_reset_pulse(void) 
{
RST = 1;
dly_ms(1);
RST = 0;
dly_ms(20);
}

void spi_init(void) 
{
ANSELA = 0;
ANSELC = 0;
RST_T = 0;
RST = 1;
dly_ms(20);
TRISC0 = 0;
TRISC2 = 0;
SSPSTAT = 0xC0;
SSPCON1 = 0x2A;
SSP1ADD = 100;
sck_dur = 1;
spi_set_sck_duration(sck_dur);
RST = 0;
spi_reset_pulse();
}

// send 8 bit, return received byte
unsigned char spi_mastertransmit(unsigned char data)
{
unsigned char temp;
SSP1IF=0;
SSPBUF = data;
while (SSP1IF==0);
temp = SSPBUF;
return temp;
}

// send 16 bit, return last rec byte
unsigned char spi_mastertransmit_16(unsigned int data)
{
        spi_mastertransmit((data>>8)&0xFF);
        return(spi_mastertransmit(data&0xFF));
}

// send 32 bit, return last rec byte
unsigned char spi_mastertransmit_32(unsigned long data)
{
        spi_mastertransmit((data>>24)&0xFF);
        spi_mastertransmit((data>>16)&0xFF);
        spi_mastertransmit((data>>8)&0xFF);
        return(spi_mastertransmit(data&0xFF));
}


void spi_disable(void)
{
ANSELA = 0xFF;
ANSELC = 0xFF;
TRISC = 0xFF;
TRISA = 0xFF;
SSPSTAT = 0x00;
SSPCON1 = 0x00;
}


void eeprom_write_byte(unsigned char addr, unsigned char data)
{
//test_ee[addr] = data;
eeprom_write(addr,data);
}

unsigned char eeprom_read_byte (unsigned char addr)
{
//return test_ee[addr];
return eeprom_read(addr);
}

//--------------------------------------------------------------

void uart_init(void) 
{
    TXSTA = 0x24;
    RCSTA = 0x90;
    SPBRGL = 7;
}
/* send one character to the rs232 */
void uart_sendchar(char c) 
{
    TXREG = c;
    while (TRMT==0);
}
/* send string to the rs232 */
void uart_sendstr(char *s) 
{
        while (*s){
                uart_sendchar(*s);
                s++;
        }
}

/* get a byte from rs232
* this function does a blocking read */
unsigned char uart_getchar(unsigned char kickwd)  
{
    if (OERR)
    {
        CREN = 0;
        CREN = 1;
    }
    while (RCIF==0);
    return RCREG;
}

/* read and discard any data in the receive buffer */
void uart_flushRXbuf(void)  
{
    volatile unsigned char a;
    if (RCIF)
    {
        a = RCREG;
    }
}
