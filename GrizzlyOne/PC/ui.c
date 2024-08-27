/*
 * ui.c
 *
 * Created: 22.08.2024 23:35:18
 *  Author: lawi
 */
#include "../engine.h"

const char pieceChars[8] = { 'X','P','N','B','R','Q','K','X' };
void printMove(uint8_t from, uint8_t to) {
    //setColor(COLOR_YELLOW);
    printf("%02X-%02X  ",from,to)
    //setColor(COLOR_WHITE);
}

void printBoard() {
    int8_t i,h;
    char a;
    char off = 'a'-'A';
    //setColor(COLOR_CYAN);
    printf("--------\n\r");
    for (i=0x70; i>=0; i-=0x10) {
        for (h=0; h<8; h++) {
            uint8_t p = board[h+i];
            if (p&0xE0) {
                a ='#';
                //setColor(COLOR_RED);
            }
            else if ((p&0xE0) == 0x18) {
                a = pieceChars[p&7];
                //setColor(COLOR_YELLOW);
            }
            else if (p&WMASK) {
                a = pieceChars[p&7];
                //setColor(COLOR_GREEN);
                if (a=='X')
                //setColor(COLOR_RED);
            }
            else if(p&BMASK) {
                a = pieceChars[p&7] + off;
                //setColor(COLOR_MANGENTA);
                if (a=='x')
                //setColor(COLOR_RED);
            }
            else if(p==EMPTY) {
                a ='.';
                //setColor(COLOR_WHITE);
            }
            else {
                a ='?';
                //setColor(COLOR_RED);
            }
            printf("%c",a);
        }
        printf("\n\r");
    }
    //setColor(COLOR_CYAN);
    printf("--------\n\r");
    //setColor(COLOR_WHITE);
}

void printMovelist(uint8_t p) {
    Move* pMoves = hm[p].firstEntry;
    int16_t i = 0;
    printf("Move list at %04X \n\r---------\n\r",(uint16_t)hm[p].firstEntry);
    while((pMoves < hm[p+1].firstEntry) && (pMoves->from != 0xff) && (pMoves->to != 0xff)) {
        printMove(pMoves->from,pMoves->to);
        pMoves++;
        i++;
    }
    printf("   count: %d\n\r",i);
}

void dbgPuts(char * s) { printf("%s",s); }
void dbgInit(void) {  }
void dbgPutc(uint8_t c) { printf("%c",c); }
void dbgSetColor (uint8_t color) { }
void dbgSendHexByte(uint8_t b) { printf("%02X",b); }
void dbgSendHexWord(uint16_t w) { printf("%04X",w); }
void dbgSendDec16(int16_t w) { printf("%d",w); }
void dbgSendDec32(int32_t w) { printf("%d",w); }
void dbgNewLine() { printf("\n\r"); }
void dbgPrintf(char *s, ...) {};
