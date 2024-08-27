/*
 * gzdisplay.c
 *
 * Created: 29.01.2022 19:03:26
 *  Author: Michael Lawatsch
 */
#include <avr/io.h>
#include "../engine.h"

const uint8_t displayCharset[18] = { CHR_0, CHR_1, CHR_2, CHR_3, CHR_4, CHR_5, CHR_6, CHR_7, CHR_8, CHR_9, CHR_A, CHR_B, CHR_C, CHR_D, CHR_E, CHR_F, CHR_G, CHR_H };


void displayInit() {

    displayDigit = 0;
    PORTC = 0xff;

    PORTD = 0xec; // display digits and LEDs off
    PORTC = 0xc2;
    displayBuffer[0]=CHR_G;
    displayBuffer[1]=CHR_R;
    displayBuffer[2]=CHR_0;
    displayBuffer[3]=CHR_N;

}

void displayShowLevel(uint8_t level) {
	displayBuffer[0] = CHR_L;
	displayBuffer[1] = CHR_MINUS;
	displayBuffer[2] = displayCharset[(level >> 4)+CHRSET_OFFSET_A];
	displayBuffer[3] = displayCharset[(level & 0xf)+CHRSET_OFFSET_1];
}

void displayShowMove(uint8_t from, uint8_t to) {
	displayBuffer[0] = displayCharset[(from & 0xf)+CHRSET_OFFSET_A];
	displayBuffer[1] = displayCharset[(from >> 4)+CHRSET_OFFSET_1];
	displayBuffer[2] = displayCharset[(to & 0xf)+CHRSET_OFFSET_A];
	displayBuffer[3] = displayCharset[(to >> 4)+CHRSET_OFFSET_1];
}

void fillDisplay(uint8_t character) {
	uint8_t i;
	for (i=0;i<4;i++)
		displayBuffer[i] = character;
}

void ledToggleWhite() {
    PORTD ^= 0x08;
}

void ledToggleBlack() {
    PORTD ^= 0x04;
}

void letSetWhiteBlack(uint8_t white, uint8_t black) {


}
