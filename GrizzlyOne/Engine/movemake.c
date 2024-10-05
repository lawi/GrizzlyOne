/*
 * movemake.c
 *
 * Created: 29.01.2022 19:44:40
 *  Author: Michael Lawatsch
 */

// #include <avr/io.h>
#include "chessglobals.h"
#include "chessutils.h"
#include "movegen.h"
#include "../user.h"

uint8_t inCkeck() {
    return 0;
}

uint8_t nextMove(uint8_t capture) {
    //checkBoard();
    uint8_t foundMove = 0;
    uint8_t other = OTHER(side);
    // Generate moves one time
    if (!hm[ply].movesGenerated) {
        uint8_t genmoves = 1;
        if (capture) genmoves = captureMoveGen();
        else genmoves = moveGen();
        //printMovelist(ply);
        hm[ply].movesGenerated = 1;
        hm[ply].actEntry = hm[ply].firstEntry;
        if(!genmoves) return 0;
        //if (!checkBoard()) {
           // uartPuts("Nach moveGen\n\r");
        //}
    }
    // Prepare next half move every time
    hm[ply+1].movesGenerated = 0;
    hm[ply+1].ep = 0xff;
    hm[ply+1].matBlack = hm[ply].matBlack;
    hm[ply+1].matWhite = hm[ply].matWhite;
    hm[ply+1].flags   = hm[ply].flags;
    hm[ply+1].capture = EMPTY;
    hm[ply].actFrom   = 0xff;
    hm[ply].actTo     = 0xff;
    line[ply].from = 0xff;
    line[ply].to = 0xff;
    hm[ply+1].wKingPos = hm[ply].wKingPos;
    hm[ply+1].bKingPos = hm[ply].bKingPos;

    // try moves from moveList
    uint8_t kingPos = (side == WHITE) ? hm[ply].wKingPos : hm[ply].bKingPos;
    //uartPuts("selecting next move\n\r");
    while(hm[ply].actEntry < hm[ply+1].firstEntry) {
        switch(board[hm[ply].actEntry->from]&0x7) {
            case PAWN:
                board[hm[ply].actEntry->from] = EMPTY;
                if (hm[ply].ep == hm[ply].actEntry->to) {
                    board[hm[ply].actEntry->to] = PAWN|side;
                    uint8_t pawnHit = (side == WHITE) ? hm[ply].ep-0x10 : hm[ply].ep+0x10;
                    board[pawnHit] = EMPTY;
                    hm[ply].capture = PAWN|other;
                    if (isAttacked(kingPos,other)) {
                        board[hm[ply].actEntry->to] = EMPTY;
                        board[pawnHit] = PAWN|other;
                        board[hm[ply].actEntry->from] = PAWN|side;
                        hm[ply].capture = EMPTY;
                        break;
                    }
                    foundMove = 1;
                } else if (hm[ply].actEntry->to & PROMO_FLAG) {
                    uint8_t to = (hm[ply].actEntry->to & 0xf) | ((side == WHITE ) ? SQ_A8 : SQ_A1);
                    uint8_t piece = ((hm[ply].actEntry->to >> 4) & 0x7) | side ;
                    hm[ply].capture = board[to];
                    board[to] = piece;
                    if (isAttacked(kingPos,other)) {
                        board[hm[ply].actEntry->from] = PAWN|side;
                        board[to] = hm[ply].capture;
                        hm[ply].capture = EMPTY;
                        break;
                    }
                    if (side == WHITE)
                        hm[ply+1].matWhite += pieceValues[piece];
                    else
                        hm[ply+1].matBlack += pieceValues[piece];
                    foundMove = 1;
                } else {
                    hm[ply].capture = board[hm[ply].actEntry->to];
                    board[hm[ply].actEntry->to] = PAWN | side;
                    if (isAttacked(kingPos,other)) {
                        board[hm[ply].actEntry->from] = PAWN | side;
                        board[hm[ply].actEntry->to] = hm[ply].capture;
                        hm[ply].capture = EMPTY;
                        break;
                    }
                    if (side == WHITE && (hm[ply].actEntry->to - hm[ply].actEntry->from == 0x20))
                    hm[ply+1].ep = hm[ply].actEntry->to - 0x10;
                    else if (side == BLACK && (hm[ply].actEntry->from - hm[ply].actEntry->to == 0x20))
                    hm[ply+1].ep = hm[ply].actEntry->to + 0x10;
                    foundMove = 1;
                }
                break;
            case KING:
                if (side == WHITE_MOVE) {
                    if (hm[ply].actEntry->from == SQ_E1) {
                        hm[ply+1].flags&=~(FLAG_CS_WHITE|FLAG_CL_WHITE);
                        if (hm[ply].actEntry->to == SQ_G1 && !isAttacked(SQ_E1,other) && !isAttacked(SQ_F1,other) && !isAttacked(SQ_G1,other)) {  // short castle
                            hm[ply+1].wKingPos = SQ_G1;
                            board[SQ_G1] = WKING; board[SQ_E1] = EMPTY; board[SQ_F1] = WROOK; board[SQ_H1] = EMPTY;
                            foundMove=1;
                            break;
                        }
                        else if (hm[ply].actEntry->to == SQ_C1 && !isAttacked(SQ_E1,other) && !isAttacked(SQ_D1,other) && !isAttacked(SQ_C1,other)) {  // long castle
                            hm[ply+1].wKingPos = SQ_C1;
                            board[SQ_C1] = WKING; board[SQ_E1] = EMPTY; board[SQ_D1] = WROOK;  board[SQ_A1] = EMPTY;
                            foundMove=1;
                            break;
                        }
                    }
                }  else {
                    if (hm[ply].actEntry->from == SQ_E8) {
                        hm[ply+1].flags&=~(FLAG_CS_BLACK|FLAG_CL_BLACK);
                        if (hm[ply].actEntry->to == SQ_G8 && !isAttacked(SQ_E8,other) && !isAttacked(SQ_F8,other) && !isAttacked(SQ_G8,other)) {  // short castle
                            hm[ply+1].bKingPos = SQ_G8;
                            board[SQ_G8] = WKING; board[SQ_E8] = EMPTY; board[SQ_F8] = WROOK; board[SQ_H8] = EMPTY;
                            foundMove=1;
                            break;
                        } else if (hm[ply].actEntry->to == SQ_C8 && !isAttacked(SQ_E8,other) && !isAttacked(SQ_D8,other) && !isAttacked(SQ_C8,other)) {  // long castle
                            hm[ply+1].bKingPos = SQ_C8;
                            board[SQ_C8] = WKING; board[SQ_E8] = EMPTY; board[SQ_D8] = WROOK;  board[SQ_A8] = EMPTY;
                            foundMove=1;
                            break;
                        }
                    }
                }
            case ROOK:
                if (side == WHITE_MOVE && hm[ply].actEntry->from == SQ_A1) hm[ply+1].flags&=~FLAG_CL_WHITE;
                else if (side == WHITE_MOVE && hm[ply].actEntry->from == SQ_H1) hm[ply+1].flags&=~FLAG_CS_WHITE;
                else if (side == BLACK_MOVE && hm[ply].actEntry->from == SQ_A8) hm[ply+1].flags&=~FLAG_CL_BLACK;
                else if (side == BLACK_MOVE && hm[ply].actEntry->from == SQ_H8) hm[ply+1].flags&=~FLAG_CS_BLACK;
            case KNIGHT:
            case BISHOP:
            case QUEEN:
                if (board[hm[ply].actEntry->from] == WKING) {
                    hm[ply+1].wKingPos = board[hm[ply].actEntry->to];
                    kingPos = hm[ply+1].wKingPos;
                } else if (board[hm[ply].actEntry->from] == BKING) {
                    hm[ply+1].bKingPos = board[hm[ply].actEntry->to];
                    kingPos = hm[ply+1].bKingPos;
                }
                hm[ply].capture = board[hm[ply].actEntry->to];
                board[hm[ply].actEntry->to] = board[hm[ply].actEntry->from];
                board[hm[ply].actEntry->from] = EMPTY;
                if (isAttacked(kingPos,other)) {
                    board[hm[ply].actEntry->from] = board[hm[ply].actEntry->to];
                    board[hm[ply].actEntry->to] = hm[ply].capture;
                    hm[ply].capture = EMPTY;
                    hm[ply+1].wKingPos = hm[ply].wKingPos;
                    hm[ply+1].bKingPos = hm[ply].bKingPos;
                    kingPos = (side == WHITE) ?  hm[ply].wKingPos : hm[ply].bKingPos;
                }
                else {
                    foundMove = 1;
                }

                break;
            default:
                dbgSetColor(COLOR_RED);
                dbgPuts("Found piece ");
                dbgSendHexByte(board[hm[ply].actEntry->from]);
                dbgPuts("  on field ");
                dbgSendHexByte(hm[ply].actEntry->from);
                dbgPuts("\n\r");
                dbgSetColor(COLOR_WHITE);
                printLine(ply);
                printBoard();
                criticalError = 1;
                foundMove = 0;
                return 0;
            }
        // selected and made one move
        if (foundMove) {
            hm[ply].actFrom = hm[ply].actEntry->from;
            hm[ply].actTo = hm[ply].actEntry->to;
            line[ply].from = hm[ply].actFrom;
            line[ply].to = hm[ply].actTo;
            hm[ply].actEntry++;
            // incremental material calculation
            if (side==WHITE) {
                hm[ply+1].matBlack -= pieceValues[hm[ply].capture];
            } else {
                hm[ply+1].matWhite -= pieceValues[hm[ply].capture];
            }
            // change side
            side = OTHER(side);
            break;
        } else {
            hm[ply+1].flags = hm[ply].flags;
            hm[ply].actEntry++;
        }

        // no move found -> reset flags (needed?)
        hm[ply+1].flags = hm[ply].flags;
    }
    //if (!checkBoard()) {
    //    uartPuts("Ende Makemove\n\r");
    //}
    return foundMove;
}

void backMove() {
    //checkBoard();
    Move *pt;
    pt = hm[ply].actEntry-1;
    side = OTHER(side);
    if (pt->to & PROMO_FLAG) { // take back promotions
        //TODO: hier ist evtl noch was faul. 
        board[(pt->to&0x7) | ((side == WHITE_MOVE) ? SQ_A8 : SQ_A1)] = hm[ply].capture;
        board[pt->from] = PAWN|side;
        hm[ply].capture = EMPTY;
    } else if ((board[pt->to] & 7) == PAWN  && pt->to == hm[ply].ep) {
        board[pt->to] = EMPTY;
        board[pt->from] = PAWN | side;
        if (side == WHITE)
            board[hm[ply].ep-0x10]=BPAWN;
        else
            board[hm[ply].ep+0x10]=WPAWN;
    } else if (board[pt->to] == WKING && pt->from == SQ_E1) {
        board[SQ_E1] = WKING;
        board[pt->to] = hm[ply].capture;
        if (pt->to == SQ_C1) {
            board[SQ_A1] = WROOK;  board[SQ_D1] = EMPTY;
        }  else if (pt->to == SQ_G1)  {
            board[SQ_H1] = WROOK;  board[SQ_F1] = EMPTY;
        }
    }  else if (board[pt->to] == BKING && pt->from == SQ_E8) {
        board[SQ_E8] = BKING;
        board[pt->to] = hm[ply].capture;
        hm[ply].capture = EMPTY;
        if (pt->to == SQ_C8) {
            board[SQ_A8] = BROOK;  board[SQ_D8] = EMPTY;
        }  else if (pt->to == SQ_G8) {
            board[SQ_H8] = BROOK;  board[SQ_F8] = EMPTY;
        }
    }  else {
        board[pt->from] = board[pt->to];
        board[pt->to] = hm[ply].capture;
        hm[ply].capture = EMPTY;
    }
    line[ply].from = 0xff;
    line[ply].to = 0xff;

    //checkBoard();
}




void doMove(uint8_t from, uint8_t to) {
    hm[0].bestFrom= 0xff;
    hm[0].actFrom = 0xff;
    hm[0].actEntry = hm[0].firstEntry;
    uint8_t ep = hm[0].ep;
    hm[0].ep   = hm[1].ep;
    hm[0].capture = EMPTY;
    switch(board[from]&7) {
        case ROOK:
            if (side == WHITE && from == SQ_A1)
                hm[0].flags&=~(FLAG_CL_WHITE);
            else if (side == WHITE && from == SQ_H1)
                hm[0].flags&=~(FLAG_CS_WHITE);
            else if (side == BLACK && from == SQ_A8)
                hm[0].flags&=~(FLAG_CL_BLACK);
            else if (side == BLACK && from == SQ_H8)
                hm[0].flags&=~(FLAG_CS_BLACK);
        case KNIGHT:
        case BISHOP:
        case QUEEN:
            hm[0].capture = board[to];
            board[to] = board[from];
            board[from] = EMPTY;
            break;
        case PAWN:
            if (to & PROMO_FLAG) {
                uint8_t pto = (to & 0xf) | ((side == WHITE ) ? SQ_A8 : SQ_A1); // or higher bits of line
                uint8_t piece = ((pto >> 4) & 0x7) | side; // piece is included in <to> part of the move
                hm[ply].capture = board[pto];
                board[pto] = piece;
            } else {
                hm[0].capture = board[to];
                board[to] = PAWN | side;
                board[from] = EMPTY;
                if (to == ep)
                    board[side == WHITE ? to-0x10 : to+0x10] = EMPTY;
                hm[0].ep = (((side == WHITE) && ((from-to)==0x20)) || (side == BLACK && ((to-from)==0x20))) ? to : 0xff;
            }
            break;
        case KING:
            if (side == WHITE_MOVE) {
                if (from == SQ_E1) {
                    hm[ply+1].flags&=~(FLAG_CS_WHITE|FLAG_CL_WHITE);
                    if (hm[ply].actEntry->to == SQ_G1) {  // short castle
                        board[SQ_F1] = WROOK; board[SQ_H1] = EMPTY;
                    } else if (hm[ply].actEntry->to == SQ_C1) {  // long castle
                        board[SQ_D1] = WROOK;  board[SQ_A1] = EMPTY;
                    }
                }
                hm[0].wKingPos = to;
            } else {
                if (hm[ply].actEntry->from == SQ_E8) {
                    hm[ply+1].flags&=~(FLAG_CS_BLACK|FLAG_CL_BLACK);
                    if (hm[ply].actEntry->to == SQ_G8 ) { // short castle
                        board[SQ_F8] = WROOK; board[SQ_H8] = EMPTY;
                    } else if (hm[ply].actEntry->to == SQ_C8 ) { // long castle
                        board[SQ_D8] = WROOK;  board[SQ_A8] = EMPTY;
                    }
                }
                hm[0].bKingPos = to;
            }
            hm[0].capture = board[to];
            board[to]   = board[from];
            board[from] = EMPTY;
            break;

    }
    getMaterial(0);
    side = OTHER(side);
    hm[0].movesGenerated = 0;
}

void undoMove() {

}

