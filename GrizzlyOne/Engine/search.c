/*
 * search.c
 *
 * Created: 22.10.2023 19:18:02
 *  Author: lawi
 */
#include "chessglobals.h"
#include "chessutils.h"
#include "eval.h"
#include "movemake.h"
#include "../user.h"
int16_t negaMax(int16_t al, int16_t be);
int16_t qsearch(int16_t al, int16_t be);
int16_t calculateMove() {
    int16_t bestScore = -INFINTE_VAL;
    int16_t al, be;
    int16_t score;
    uint8_t foundMove = 0;
    criticalError = 0;
    dbgPuts("\n\rStart thinking ...\n\r");
    maxDepth=3;
    nodes = 0;
    for (plyMax = 1; plyMax<=maxDepth; plyMax++) {
        foundMove = 0;
        al = -INFINTE_VAL;
        be = INFINTE_VAL;
        dbgPuts("Ply ");
        dbgSendDec16(plyMax);
        dbgNewLine();
        hm[0].movesGenerated=0;
        while (nextMove(0)) {
            //printMove(hm[ply].actFrom,hm[ply].actTo);newLine();
            ply++; score = -negaMax(-be,-al); ply--;
            backMove();
            //printBoard();
            if (score>bestScore) {
                dbgSendDec16(plyMax);
                dbgPuts(" ");
                printMove(hm[0].actFrom,hm[0].actTo);
                dbgPuts(" ");
                dbgSendDec16(score);
                dbgPuts("\n\r");
                hm[0].bestFrom = hm[0].actFrom;
                hm[0].bestTo = hm[0].actTo;
                hm[0].bestValue = score;
                bestScore = score;
            }
            //printMovelist(0);
            foundMove = 1;
            if (criticalError) break;
        }
        if (!foundMove || criticalError) break;
    }
    if (!foundMove) {
        if (side == WHITE) {
            dbgPuts("White is mate !!! \n\r");
        } else {
            dbgPuts("Black is mate !!! \n\r");
        }
    } else {
        dbgPuts("\n\r... end thinking best score = ");
        dbgSendDec16(bestScore);
        dbgPuts("\n\r");
    }
    return bestScore;
}
int16_t  negaMax(int16_t al, int16_t be) {
    int16_t bestScore = -INFINTE_VAL;
    int16_t score;
    uint8_t foundMove=0;
    if (ply>=plyMax) return qsearch(al,be);
    hm[ply].movesGenerated=0;
    while (nextMove(0)) {
        foundMove = 1;
        ply++; score = -negaMax(-be,-al); ply--;
        backMove();
        if (score>bestScore) {
            hm[ply].bestFrom = hm[ply].actFrom;
            hm[ply].bestTo = hm[ply].actTo;
            hm[ply].bestValue = score;
            bestScore = score;
            if (bestScore>be) return bestScore;
        }
    }
    if (!foundMove) {
        bestScore = -INFINTE_VAL+ply;
    }
    return  bestScore;

}

int16_t qsearch(int16_t al, int16_t be) {
    int16_t score = eval();
    nodes++;
    if (score >= be) return score;
    if (score > al) al = score;
    if (ply >  MAX_HMCOUNT-2) return al;
    hm[ply].movesGenerated=0;
    //uartPuts("QSearch ply = ");
    //uartSendHexByte(ply);
    //uartPuts("  bKing= ");
    //uartSendHexByte(hm[ply].bKingPos);
    //uartPuts("  wKing= ");
    //uartSendHexByte(hm[ply].wKingPos);
    //newLine();
    while(nextMove(1)) {
        //uartPuts(" ply: ");
        //uartSendHexByte(ply);
        //uartPuts(" - ");
        //printMove(hm[ply].actFrom,hm[ply].actTo); newLine();
        if (criticalError) { backMove(); dbgPuts("ERROR \n\r"); break; }
        /*
        if (hm[ply].actFrom == 0x12 && hm[ply].actTo == 0x14) {
            uartPuts("Stellung ohne Schach\n\r");
            printBoard();
            while(1);
        }
        */
        ply++; score = -qsearch(-be,-al); ply--;
        backMove();
        if (score > be || criticalError) {
            return score;
        }
        if (score>al) {
            al = score;
        }
    }
    return al;
}
