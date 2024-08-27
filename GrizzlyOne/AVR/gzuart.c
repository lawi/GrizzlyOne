/*
 * gzuart.c
 *
 * Created: 29.01.2022 17:59:46
 *  Author: Michael Lawatsch
 */
#include <avr/io.h>
#include <avr/interrupt.h>

void uartInit(void) {
    UBRR0H = 0;     // 115200 Baud 8N1
    UBRR0L = 10;  // 9600 = 0x81  115200 = 10
    UCSR0B = (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0);
    UCSR0C = (3<<UCSZ00);  // 8 bit
}

void uartPutc(uint8_t c) {
    while (!(UCSR0A & (1<<UDRE0)));
    UDR0 = c;
}

void uartPuts (char *s) {
    while (*s) {
        uartPutc((uint8_t)*s);
        s++;
    }
}

void uartSendHexByte(uint8_t b) {
    uint8_t a = b>>4;
    if (a>9) uartPutc(a+'A'-10);
    else uartPutc(a+'0');
    a = b&0xf;
    if (a>9) uartPutc(a+'A'-10);
    else uartPutc(a+'0');
}

void uartSendHexWord(uint16_t w) {
    uartSendHexByte((uint8_t) (w>>8));
    uartSendHexByte((uint8_t) (w&0xff));
}

void uartSendDec16(int16_t w) {
    if (w<0) {
        uartPutc('-');
        w=-w;
    }
    uint8_t lz = 0;
    int16_t digits = 10000;
    while(digits>=1) {
        uint8_t digit = w/digits;
        if (digit || lz || digits == 1) {
            lz = 1;
            uartPutc(digit+'0');
        }
        w = w - digit * digits;
        digits /= 10;
    }
}


void uartSendDec32(int32_t w) {
    if (w<0) {
        uartPutc('-');
        w=-w;
    }
    uint8_t lz = 0;
    int32_t digits = 1000000000L;
    while(digits>=1) {
        uint8_t digit = w/digits;
        if (digit || lz || digits == 1) {
            lz = 1;
            uartPutc(digit+'0');
        }
        w = w - digit * digits;
        digits /= 10;
    }
}
void setColor (uint8_t color) {
    uartPutc(27);
    uartPutc('[');
    uartPutc('3');
    uartPutc('0'+color);
    uartPutc('m');
}

void newLine() {
    uartPuts("\r\n");
}

ISR(USART0_RX_vect) {

}