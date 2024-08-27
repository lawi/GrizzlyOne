/*
 * gztimer.c
 *
 * Created: 29.01.2022 18:05:13
 *  Author: Michael Lawatsch
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "gzdisplay.h"
#include "../engine.h"

volatile uint8_t timerUptime;
volatile uint8_t timerSilentCount;

volatile uint8_t timerRefreshTimer;
volatile uint8_t timerRefreshNow;
#define TIMER_REFRESH_CYCLES 4
void timerInit() {

    timerUptime = 2;
    timerRefreshTimer = 0;
    timerRefreshNow = 0;
    keysPressed = 0;
    funKeysPressed = 0;

    TCCR1A = 0x00;
    TCCR1B = 0x0d; // set prescaler to 1024 CPU Clk / 1024 = 20 Mhz /1024  19531,25 kHz
    OCR1AH = 0;    // 20 ms = 391
    OCR1AL = 78;
    TIMSK1 = 0x02;
    //PORTD ^= 0x08;
    timerRefreshTimer = TIMER_REFRESH_CYCLES;
}

ISR(TIMER1_COMPA_vect) {

    uint8_t tmp1, tmp2;
    tmp1 = displayBuffer[displayDigit];
    tmp2 = 1<<(displayDigit+4);
    tmp2 ^= 0xff;
    PORTD = tmp2;
    PORTC = tmp1;
    displayDigit++;
    if (displayDigit>=4) displayDigit = 0;

    if (timerRefreshTimer != 0) {
        timerRefreshTimer--;
    }
    else {
       timerRefreshNow = 1;
       timerRefreshTimer = TIMER_REFRESH_CYCLES;

       // Keyboard PORTA a1 .. h8 (eight buttons)
       tmp1 = PINA;
       tmp2 = PINB & 0x07;
       if (timerUptime == 0) {
            if (tmp1!=0xff) {
                keysPressed = tmp1^0xff; // Save Key for mainloop
                timerUptime = 2;  // now wait for two cycle
            }
            if (tmp2 != 0x07) {
                funKeysPressed = tmp2^0x07; // Save Func Key for mainloop
                timerUptime = 2;  // now wait for two cycle
            }
       }
       else if (tmp1 == 0xff && tmp2 == 0x07) timerUptime--;
       else timerUptime=2;
        //PORTD ^= 0x0c;
    }

}



