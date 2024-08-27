/*
 * GrizzlyOne.c
 *
 * Created: 29.01.2022 17:21:49
 * Author : Michael Lawatsch
 */

#include <avr/io.h>
#include <avr/iom644.h>
#include <avr/interrupt.h>
#include "gzuart.h"
#include "gztimer.h"
#include "gzdisplay.h"
#include "grizzly.h"

void systemInit() {
    DDRC = 0xff;
    DDRB = 0x08;
    DDRD = 0xfc;
    DDRA = 0x00;
    PORTA = 0xff; // Pull up
    PORTB = 0x07;
    PORTD = 0xff;
 	uartInit();
 	displayInit();
 	timerInit();
 	sei();
}

int main(void)
{
    SP = RAMEND-4;
    systemInit();

    grizzly();
    while (1)
    {
    }
}

