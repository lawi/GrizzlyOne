/*
 * chessutils.c
 *
 * Created: 01.02.2022 19:12:25
 *  Author: Michael Lawatsch
 */
//#include <avr/io.h>
//#include <avr/iom644.h>
#include "../user.h"
#include "chessglobals.h"
#include "eval.h"


const uint8_t baseRow[] = { ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK };

//*************************************************
//* isAttacked
//*
//* in:
//* uint8_t square = square which should be checked
//* uint8_t color = color of the attacker
//* out:
//* boolean (true) 1 if attacked
//*************************************************
uint8_t isAttacked(uint8_t square, uint8_t color) {
    const int8_t* off = moffKnight;
    uint8_t sq;
    if (color == BLACK) {
        sq = square + 0x0f;
        if (!(sq&0x88) && board[sq]==BPAWN) return 1;
        sq += 2;
        if (!(sq&0x88) && board[sq]==BPAWN) return 1;
    } else {
        sq = square - 0x11;
        if (!(sq&0x88) && board[sq]==WPAWN) return 1;
        sq += 2;
        if (!(sq&0x88) && board[sq]==WPAWN) return 1;
    }
    while (*off) {
        sq = *off + square;
        if (!(sq&0x88))
            if (board[sq] == (color|KNIGHT)) return 1;
        off++;
    }
    off = moffQueen;
    uint8_t piece = color|ROOK;
    while (*off) {
        sq = *off + square;
        if (!(sq&0x88)) {
            if (board[sq] == (color|KING)) { return 1; }
            while (!(sq&0x88) && board[sq] == EMPTY) sq+=*off;
            if (!(sq&0x88) && (board[sq] == piece ||
                board[sq] == (color|QUEEN))) { return 1; }
        }
        off++;
        if (*off == 0x11) piece = color|BISHOP; // !!!! depends on unchanged offset array
    }
    return 0;
}

void clearBoard() {
    uint8_t i;
    for (i = 0; i<128; i++) {
        board[i] = EMPTY;
    }
}

void showMaterial(uint8_t n) {
    dbgPuts("Black material = "); dbgSendDec16(hm[n].matBlack); dbgNewLine();
    dbgPuts("White material = "); dbgSendDec16(hm[n].matWhite); dbgNewLine();
    dbgPuts("Black King     = "); dbgSendHexByte(hm[n].bKingPos); dbgNewLine();
    dbgPuts("White King     = "); dbgSendHexByte(hm[n].wKingPos); dbgNewLine();
}

void getMaterial(uint8_t n) {
    uint8_t i;
    hm[n].matBlack = 0;
    hm[n].matWhite = 0;
    hm[n].wKingPos = 0xff;
    hm[n].bKingPos = 0xff;
    for (i=SQ_A1;i<=SQ_H8;i++) {
        if (i & 0x88) i+=8;
        if (board[i] & WHITE) {
            hm[n].matWhite += pieceValues[board[i]];
            if (board[i] == WKING) hm[n].wKingPos = i;
        } else if (board[i] & BLACK) {
            hm[n].matBlack += pieceValues[board[i]];
            if (board[i] == BKING) hm[n].bKingPos = i;
        }
    }
}

uint8_t checkBoard() {
    uint8_t i;
    for (i=SQ_A1;i<=SQ_H8;i++) {
        if (OUTSIDE(i)) i+=8;
        switch(board[i]) {
            case WKING:
            case BKING:
            case WPAWN:
            case BPAWN:
            case WROOK:
            case BROOK:
            case WQUEEN:
            case BQUEEN:
            case WKNIGHT:
            case BKNIGHT:
            case WBISHOP:
            case BBISHOP:
            case EMPTY:
                break;
            default:
                criticalError = 4;
                #ifdef AVR_BUILD
                dbgSetColor(COLOR_RED);
                dbgPuts("CheckBoard found piece ");
                dbgSendHexByte(board[i]);
                dbgPuts(" on field "); dbgSendHexByte(i); dbgPuts(" Stack: ");
                dbgSendHexWord(SP); dbgPuts(" Board: ");
                dbgSendHexWord((uint16_t)board);
                dbgPuts("\n\r");
                dbgSetColor(COLOR_WHITE);
                #endif
                printBoard();
                return 0;
        }
    }

    return 1;
}

void newGame() {
    uint8_t i;
    clearBoard();
    hm[0].flags     =0xf;
    hm[0].actEntry  =mvList;
    hm[0].firstEntry=mvList;
    hm[0].bestFrom  =0xff;
    hm[0].bestTo    =0xff;
    hm[0].actFrom   = 0xff;
    hm[0].actTo     = 0xff;
    hm[0].bestValue = 0;
    hm[0].dynPosVal = 0;
    hm[0].ep        = 0;
    hm[1].ep        = 0;
    hm[0].capture   = 0xff;
    hm[0].movesGenerated = 0;


    for (i=SQ_A1; i<=SQ_H1;i++) {
        board[i] = baseRow[i&7]|WMASK;
        hm[0].matWhite += valPieces[baseRow[i&7]];
    }
    for (i=SQ_A2; i<=SQ_H2;i++) {
        board[i] = WPAWN;
        hm[0].matWhite += VAL_PAWN;
    }
    for (i=SQ_A7; i<=SQ_H7;i++) {
         board[i] = BPAWN;
         hm[0].matBlack += VAL_PAWN;
    }
    for (i=SQ_A8; i<=SQ_H8;i++) {
        board[i] = baseRow[i&7]|BMASK;
        hm[0].matBlack += valPieces[baseRow[i&7]];
    }
    getMaterial(0);
    side = WHITE_MOVE;
    ply = 0;
    plyMax = 1;
    maxDepth = 3; // the depth to calc to
    criticalError = 0;

}



