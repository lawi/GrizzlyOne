/*
 * ui.c
 *
 * Created: 23.10.2023 00:12:28
 *  Author: lawi
 */
#include "gzuart.h"

#include "../engine.h"

const char pieceChars[8] = { 'X','P','N','B','R','Q','K','X' };
void printMove(uint8_t from, uint8_t to) {
        setColor(COLOR_YELLOW);
        uartSendHexByte(from);
        uartPutc('-');
        uartSendHexByte(to);
        uartPutc(' ');
        setColor(COLOR_WHITE);
}

void printBoard() {
    int8_t i,h;
    char a;
    char off = 'a'-'A';
    setColor(COLOR_CYAN);
    uartPuts("--------\n\r");
    for (i=0x70; i>=0; i-=0x10) {
        for (h=0; h<8; h++) {
            uint8_t p = board[h+i];
            if (p&0xE0) {
               a ='#';
               setColor(COLOR_RED);
            }
            else if ((p&0xE0) == 0x18) {
                a = pieceChars[p&7];
                setColor(COLOR_YELLOW);
            }
            else if (p&WMASK) {
                a = pieceChars[p&7];
                setColor(COLOR_GREEN);
                if (a=='X')
                    setColor(COLOR_RED);
            }
            else if(p&BMASK) {
                a = pieceChars[p&7] + off;
                setColor(COLOR_MANGENTA);
                if (a=='x')
                    setColor(COLOR_RED);
            }
            else if(p==EMPTY) {
                a ='.';
                setColor(COLOR_WHITE);
            }
            else {
                a ='?';
                setColor(COLOR_RED);
            }
            uartPutc(a);
        }
        uartPutc('\n');
        uartPutc('\r');
    }
    setColor(COLOR_CYAN);
    uartPuts("--------\n\r");
    setColor(COLOR_WHITE);
}

void printMovelist(uint8_t p) {
    Move* pMoves = hm[p].firstEntry;
    int16_t i = 0;
    uartPuts("Move list at ");
    uartSendHexWord((uint16_t)hm[p].firstEntry);
    uartPuts("\n\r---------\n\r");
    while((pMoves < hm[p+1].firstEntry) && (pMoves->from != 0xff) && (pMoves->to != 0xff)) {
        printMove(pMoves->from,pMoves->to);
        pMoves++;
        i++;
    }
    uartPuts("   count: ");
    uartSendDec16(i);
    newLine();
}

void dbgPuts(char * s) { uartPuts(s); }
void dbgInit(void) { uartInit(); }
void dbgPutc(uint8_t c) { uartPutc(c); }
void dbgSetColor (uint8_t color) { setColor(color); }
void dbgSendHexByte(uint8_t b) { uartSendHexByte(b); }
void dbgSendHexWord(uint16_t w) { uartSendHexWord(w); }
void dbgSendDec16(int16_t w) { uartSendDec16(w); }
void dbgSendDec32(int32_t w) { uartSendDec32(w); }
void dbgNewLine() { newLine(); }
void dbgPrintf(char *s, ...) {};


