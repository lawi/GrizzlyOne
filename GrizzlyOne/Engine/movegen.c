/*
 * movegen.c
 *
 * Created: 30.01.2022 00:32:55
 *  Author: Michael Lawatsch
 */
// #include <avr/io.h>
#include "chessglobals.h"
//#include "../AVR/gzuart.h"
#include "../user.h"
#include "chessutils.h"


//#define DEBUG_SPEZ
//TODO:
// * surround usart calls with #ifdef DEBUG
// * methode to insert PV Move first place
// * captures sort to front (2nd place)
uint8_t moveGen() {
    //checkBoard();
    int8_t i,h;
    const int8_t* sliderMoff = 0;
    const int8_t* off;
    uint8_t  to;
    Move* pMoves = hm[ply].firstEntry;
    int8_t foff;
#ifdef DEBUG_SPEZ
    setColor(COLOR_BLUE);
    uartPuts("\n\rMOVEGENERATION for side ");
    if (side==WHITE)
        uartPuts("white\n\r");
    else
        uartPuts("black\n\r");
#endif
    for (i=SQ_A1; i<=SQ_H8; i++) {
        if (i&0x88)  i+=8;
#ifdef DEBUG_SPEZ
        if (i>0 && ((i&0xf) == 0)) uartPuts("\n\r");

        uartSendHexByte(i);
        uartPutc(' ');
        uartSendHexByte(board[i]);
        uartPutc(':');
#endif
        if (!board[i]) continue;
        if (!(side & board[i])) continue;
#ifdef DEBUG_SPEZ
        uartPutc('<');
        uartSendHexByte(board[i]);
        uartPutc('>');
#endif
        sliderMoff = NULL;
        switch (board[i]&7) {
            case PAWN:
                to = (side==WHITE) ? i+0x10 : i-0x10;
                if (to >= SQ_A8 || to <= SQ_H1) {
                    for (foff = KNIGHT<<4; foff <= QUEEN<<4; foff+=0x10) {
                        pMoves->from = i;
                        pMoves->to = (to&0xf)|foff|PROMO_FLAG;
                        pMoves++;
                    }
                } else if (!board[to]) {
                    pMoves->from = i;
                    pMoves->to   = to;
                    pMoves++;
                    to = 0;
                    if (side==WHITE && i<=SQ_H2)
                        to = i+0x20;
                    else if (side == BLACK && i>=SQ_A7)
                        to = i-0x20;
                    if (to && !board[to]) {
                        pMoves->from = i;
                        pMoves->to   = to;
                        pMoves++;
                    }
                }
                for (h=0x0F; h<=0x11; h+=2) { // capturing moves (left an right)
                    to = (side==WHITE)? i+h : i-h;
                    if (!(to&0x88) && (OTHER(side) & board[to])) {
                        if(to >= SQ_A8 || to <= SQ_H1) {
                            for (foff = KNIGHT<<4; foff <= QUEEN<<4; foff+=0x10) {
                                pMoves->from = i;
                                pMoves->to = (to&0xf)|foff|PROMO_FLAG;
                                pMoves++;
                            }
                        } else {
                            pMoves->from = i;
                            pMoves->to = to;
                            pMoves++;
                        }
                    }
                }
                int8_t epoff = side==WHITE ? 0x10 : -0x10;
                if ((i+epoff-hm[ply].ep) == 1 || (hm[ply].ep+epoff-i) == 1) { // if ep field near PAWN
                    pMoves->from = i;
                    pMoves->to = hm[ply].ep;
                    pMoves++;
                }
                break;
            case BISHOP:
                sliderMoff = moffBishop;
                break;
            case QUEEN:
                sliderMoff = moffQueen;
                break;
            case ROOK:
                sliderMoff = moffRook;
                break;
            case KING:
                // White Castle
                if (i==SQ_E1 && side == WHITE_MOVE && (hm[ply].flags&(FLAG_CL_WHITE|FLAG_CS_WHITE))) {
                    if ((hm[ply].flags&(FLAG_CL_WHITE)) && board[SQ_A1]==WROOK &&
                        board[SQ_B1]==EMPTY &&  board[SQ_C1]==EMPTY &&  board[SQ_D1]==EMPTY ) {
                        pMoves->from = i;
                        pMoves->to = SQ_C1;
                        pMoves++;
                    }
                    if ((hm[ply].flags&(FLAG_CS_WHITE)) && board[SQ_H1]==WROOK &&
                        board[SQ_F1]==EMPTY &&  board[SQ_G1]==EMPTY  ) {
                        pMoves->from = i;
                        pMoves->to = SQ_G1;
                        pMoves++;
                    }
                }
                // Black Castle
                else if (i==SQ_E8 && side == BLACK_MOVE && (hm[ply].flags&(FLAG_CL_BLACK|FLAG_CS_BLACK))) {
                    if ((hm[ply].flags&(FLAG_CL_BLACK)) && board[SQ_A8]==BROOK &&
                    board[SQ_B8]==EMPTY &&  board[SQ_C8]==EMPTY &&  board[SQ_D8]==EMPTY ) {
                        pMoves->from = i;
                        pMoves->to = SQ_C8;
                        pMoves++;
                    }
                    if ((hm[ply].flags&(FLAG_CS_BLACK)) && board[SQ_H8]==BROOK &&
                    board[SQ_F8]==EMPTY &&  board[SQ_G8]==EMPTY  ) {
                        pMoves->from = i;
                        pMoves->to = SQ_G8;
                        pMoves++;
                    }
                }
            // Knight and King Moves
            case KNIGHT:
                off = ((board[i]&7) == KNIGHT)? moffKnight : moffKing;
                while (*off) {
                    to = i + *off;
                    off++;
                    if (to&0x88) continue;
                    if (!(board[to]&board[i]&0x18)) {
                        pMoves->from = i;
                        pMoves->to = to;
                        pMoves++;
                    }
                }
                break;
            default:
                criticalError = 3;
                dbgSetColor(COLOR_RED);
                dbgPuts("MoveGen found piece ");
                dbgSendHexByte(board[i]);
                dbgPuts(" on field "); dbgSendHexByte(i); dbgPuts("\n\r");
                dbgSetColor(COLOR_WHITE);
                printBoard();
                return 0;


        }
        // Slider Moves
        if (sliderMoff) {
            while (*sliderMoff) {
                to = *sliderMoff + i;
                while (ON_BOARD(to)) {
                    if (!board[to]) {
                        // empty field
                        pMoves->from = i;
                        pMoves->to   = to;
                        to += *sliderMoff;
                        pMoves++;
                    } else if (!(SAME_COLOR(i,to))) {
                        // enemy piece
                        pMoves->from = i;
                        pMoves->to  = to;
                        pMoves++;
                        break;
                    } else {
                        // own piece
                        break;
                    }
                }
                sliderMoff++;
            }
        }

    }
    pMoves->from= 0xff; // end marker
    pMoves->to =  0xff;
    // Next moveList start
    hm[ply+1].firstEntry = pMoves;
    hm[ply+1].actEntry = pMoves;
#ifdef DEBUG_SPEZ
    uartPuts("\n\r");
    setColor(COLOR_WHITE);
#endif
    return hm[ply].firstEntry != pMoves;
}



uint8_t captureMoveGen() {
    //checkBoard();
    int8_t i,h;
    const int8_t* sliderMoff = 0;
    const int8_t* off;
    uint8_t  to;
    Move* pMoves = hm[ply].firstEntry;
    int8_t foff;
    int cmn =0;
    for (i=SQ_A1; i<=SQ_H8; i++) {
        if (OUTSIDE(i))  i+=8;
        if (!board[i]) continue;
        if (!(side & board[i])) continue;
        sliderMoff = NULL;
        switch (board[i]&7) {
            case PAWN:
            to = (side==WHITE) ? i+0x10 : i-0x10;
            if (to >= SQ_A8 || to <= SQ_H1) {
                for (foff = KNIGHT<<4; foff <= QUEEN<<4; foff+=0x10) { 
                    pMoves->from = i;
                    pMoves->to = (to&0xf)|foff|PROMO_FLAG;
                    pMoves++;
                    cmn++;
                }
            }
            for (h=0x0F; h<=0x11; h+=2) { // capturing moves (left an right)
                to = (side==WHITE)? i+h : i-h;
                if (ON_BOARD(to) && (OTHER(side) & board[to])) {
                     cmn++;
                    if(to >= SQ_A8 || to <= SQ_H1) {
                        for (foff = KNIGHT<<4; foff <= QUEEN<<4; foff+=0x10) {
                            pMoves->from = i;
                            pMoves->to = (to&0xf)|foff|PROMO_FLAG;
                            pMoves++;
                            cmn++;
                        }
                    } else {
                        pMoves->from = i;
                        pMoves->to = to;
                        pMoves++;       
                    }
                }
            }
            int8_t epoff = (side==WHITE) ? 0x10 : -0x10;
            if ((i+epoff-hm[ply].ep) == 1 || (hm[ply].ep+epoff-i) == 1) { // if ep field near PAWN
                pMoves->from = i;
                pMoves->to = hm[ply].ep;
                pMoves++;
                cmn++;
            }
            break;
            case BISHOP:
            sliderMoff = moffBishop;
            break;
            case QUEEN:
            sliderMoff = moffQueen;
            break;
            case ROOK:
            sliderMoff = moffRook;
            break;
            // Knight and King Moves
            case KING:
            case KNIGHT:
            off = ((board[i]&7) == KNIGHT)? moffKnight : moffKing;
            while (*off) {
                to = i + *off;
                off++;
                if (OUTSIDE(to)) continue;
                if (board[to] && (!(SAME_COLOR(to,i)))) {
                    pMoves->from = i;
                    pMoves->to = to;
                    pMoves++;
                    cmn++;
                }
            }
            break;
            default:
                criticalError = 2;
                dbgSetColor(COLOR_RED);
                dbgPuts("CaptureMoveGen found piece ");
                dbgSendHexByte(board[i]);
                dbgPuts(" on field "); dbgSendHexByte(i); dbgPuts("\n\r");
                dbgSetColor(COLOR_WHITE);
                printBoard();
                return 0;
        }
        // Slider Moves
        if (sliderMoff) {
            while (*sliderMoff) {
                to = *sliderMoff + i;
                while (ON_BOARD(to)) {
                    if (!board[to]) {
                        to += *sliderMoff;
                    } else if (!(SAME_COLOR(to,i))) {
                        pMoves->from = i;
                        pMoves->to  = to;
                        pMoves++;
                        cmn++;
                        break;
                    } else {
                        break;
                    }
                }
                sliderMoff++;
            }
        }

    }
    //if (!checkBoard()) { uartPuts("fast am Ende von Movegen\n\r"); }

    pMoves->from= 0xff; // end marker
    pMoves->to =  0xff;
    // Next moveList start
    hm[ply+1].firstEntry = pMoves;
    hm[ply+1].actEntry = pMoves;
    //if (!checkBoard()) {
    //     uartPuts("am Ende von Movegen ");
    //     uartSendHexWord((uint16_t)pMoves); newLine();
    //     uartSendHexWord((uint16_t)hm[ply+1].firstEntry); newLine();
    //     uartSendHexWord((uint16_t)hm[ply+1].actEntry); newLine();
    //     }
    return hm[ply].firstEntry != pMoves;
}



