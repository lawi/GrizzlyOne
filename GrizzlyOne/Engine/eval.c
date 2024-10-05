/*
 * eval.c
 *
 * Created: 06.02.2022 15:35:42
 *  Author: Michael Lawatsch
 */

// #include <avr/io.h>
#include "chessglobals.h"

const uint16_t valPieces[7] = {0, VAL_PAWN, VAL_KNIGHT, VAL_BISHOP, VAL_ROOK, VAL_QUEEN, VAL_KING };

int16_t eval() {
    uint8_t i;
    int16_t score = 0;
    for (i=0;i<=SQ_H8;i++) {
        if (i&0x88) i+=8;
        switch(board[i]) {
            case WKNIGHT:
            case WBISHOP:
                score += centerValues[i];
                break;
            case BKNIGHT:
            case BBISHOP:
                score -= centerValues[i];
                break;
            case WPAWN:
                score += centerValues[i+8];
                break;
            case BPAWN:
                score -= centerValues[(i^0x70)+8];
                break;
        }
    }
    score += (hm[ply].matWhite-hm[ply].matBlack); //+hm[ply].dynPosVal;
    return (side == WHITE) ? score : -score;

}    