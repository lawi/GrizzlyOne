/*
 * grizzly.c
 *
 * Created: 01.02.2022 19:06:33
 *  Author: Michael Lawatsch
 *
 * GrizzlyOneC is the 8 bit AVR Version of Grizzly
 * It is made only to see it work.
 * It is not tweaked for strength nor it has clean code
 *
 * Technical Data:
 * - 0x88 Board
 */

#include <avr/io.h>
#include <avr/iom644.h>
#include "gzdisplay.h"
#include "../engine.h"
#include "gzuart.h"
#include "ui.h"

#define STATE_GREET 0
#define STATE_PLAYER 1
#define STATE_FUNCTIONS 2
#define STATE_COMPMOVE 3
#define STATE_LEVEL 4

uint8_t getFieldKey() {
	uint8_t key = 0;
	for (key=0;key<8;key++) {
		if ((1<<key) & keysPressed) break;
	}
	return key;
}

void grizzly() {
    uint8_t state = 0;
    uint8_t funcKey = 0;
    uint8_t fieldKeys = 0;
    uint8_t digit = 0;
    uint8_t i, h;
	uint8_t level = 0;
    uartPuts("\e[33m\n\r---=== Welcome to GrizzlyOneC V 0.1 ===---\n\r");
    newGame();
    printBoard();
    moveGen();
    printMovelist(0);
    newGame();
    printBoard();
    uartPuts(" Stack: ");
    uartSendHexWord(SP);
    uartPuts(" Board: ");
    uartSendHexWord((uint16_t)board);
    uartPuts(" hm: ");
    uartSendHexWord((uint16_t)hm);
    uartPuts("\n\r");
    while(1) {
        funcKey = funKeysPressed;
        funKeysPressed = 0;
        fieldKeys = keysPressed;
        keysPressed = 0;
        if (funcKey != 0 || fieldKeys != 0) {
            uartPuts("Key pressed FUNC = ");
            uartSendHexByte(funcKey);
            uartPuts("  FIELD = ");
            uartSendHexByte(fieldKeys);
            uartPuts("\n\r");
        }
        if (state == STATE_GREET) {
            if (funcKey & KEY_GO) {
                state = STATE_PLAYER;
                fillDisplay(CHR_MINUS);
                digit = 0;
            }
            else if(funcKey & 0x4) {
                state = STATE_LEVEL;
                displayBuffer[0] = CHR_L;
                displayBuffer[1] = CHR_MINUS;
                displayBuffer[2] = CHR_0;
                displayBuffer[3] = CHR_1;
            }
        }
        else if (state == STATE_PLAYER) {
            if (fieldKeys != 0 && digit < 4) {
                if (digit & 1) i = CHRSET_OFFSET_1;
                else i = CHRSET_OFFSET_A;
                for (h=0;h<8;h++) {
                    if (fieldKeys&1) break;
                    i++;
                    fieldKeys>>=1;
                }
                displayBuffer[digit] = displayCharset[i];
                digit++;
            }
            else if (funcKey & KEY_CL) {
                fillDisplay(CHR_MINUS);
                digit = 0;
            }
			else if (funcKey & KEY_GO) {
				state = STATE_COMPMOVE;
                 displayBuffer[0] = CHR_MINUS;
                 displayBuffer[1] = CHR_MINUS;
                 displayBuffer[2] = CHR_G;
                 displayBuffer[3] = CHR_0;
			}
			else if (funcKey & KEY_FN) {
				state = STATE_LEVEL;
				displayShowLevel(level);
			}
        }
		else if (state == STATE_LEVEL) {

		}
        else if (STATE_COMPMOVE) {
            int16_t score = calculateMove();
            if (criticalError) {
                uartPuts("Critical Error: ");
                uartSendHexByte(criticalError);
                newLine();
                showMaterial(0);
            }
            else if (score > -30000) {
                displayShowMove(hm[0].bestFrom,hm[0].bestTo);
                doMove(hm[0].bestFrom,hm[0].bestTo);
                uartPuts("Nodes = ");  uartSendDec32(nodes); newLine();
            }
            printBoard();
            state = STATE_PLAYER;
        }
    }
}