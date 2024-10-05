/*
 * ui.c
 *
 * Created: 22.08.2024 23:35:18
 *  Author: lawi
 */
#include "../engine.h"
#include <stdio.h>
#include <stdlib.h>

const char pieceChars[8] = { 'X','P','N','B','R','Q','K','X' };
void printMoveSimple(uint8_t from, uint8_t to) {
    //setColor(COLOR_YELLOW);
    printf("%02X-%02X  ",from,to);
    //setColor(COLOR_WHITE);
}
void printMove(uint8_t from, uint8_t to) {
     char moveString[8];
    int i=0;
    if ((board[from]&7)>PAWN) {
        moveString[i++] = pieceChars[board[from]&7];
    }
    moveString[i++] = (from & 0x07)+'a';
    moveString[i++] = ((from & 0x70)>>4)+'1';
    // if ((board[from]&7)!=PAWN) {
    //     if (board[to]!=EMPTY) {
    //         moveString[i++]='x';
    //     } else {
    moveString[i++]='-';
    //     }
    // }
    moveString[i++] = (to & 0x07)+'a';
    
    if ((board[from])==WPAWN && from >= SQ_A7) {
        moveString[i++] = '8';
        moveString[i++] = pieceChars[(board[to]>>4) & 0x07];
    } else if ((board[from])==BPAWN && from <= SQ_H2) {
        moveString[i++] = '1';
        moveString[i++] = pieceChars[(board[to]>>4) & 0x07];
    } else {
        moveString[i++] = ((to & 0x70) >> 4)+'1';
    }
    moveString[i]=0;
    printf("%s ",moveString);
}

void printLine(int count) {
    int i;
    printf("LINE: ");
    for (i=0;i<count;i++) {
        printMove(line[i].from,line[i].to);
    }
    printf("\n");

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
                //if (a=='X')
                //setColor(COLOR_RED);
            }
            else if(p&BMASK) {
                a = pieceChars[p&7] + off;
                //setColor(COLOR_MANGENTA);
                //if (a=='x')
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
    printf("Move list at %04lX \n\r---------\n\r",hm[p].firstEntry);
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
