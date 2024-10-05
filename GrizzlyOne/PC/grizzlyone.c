#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "ui.h"
#include "../engine.h"

int getToken(char* token, char* buf, int* pos) {
    int i=0;
    while((*pos<4095)&&isspace(buf[*pos])&&buf[*pos])(*pos)++;
    while((*pos<4095)&&!isspace(buf[*pos])) token[i++]=buf[(*pos)++];
    token[i]=0;
    return strlen(token);
}

int main(int argc, char ** argv) {
    char inbuf[4096], command[256],token[256],fen[256];
    int cursorPosition = 0;
    printf("GrizzlyOne Desktop Version 0.13 \n");
    newGame();
    printBoard();
    moveGen();
    printMovelist(0);
    newGame();
    printBoard();

    while(1) {
        if(fgets(inbuf,4096,stdin) == NULL)break;
        cursorPosition=0;
        if (inbuf[0]==0 || inbuf[0]=='\n') continue;
        if (!getToken(command, inbuf, &cursorPosition)) continue;
        if(strcmp(command,"go")==0) {
            printBoard();
            int score = calculateMove();
            printf("GrizzlyOne plays: ");
            printMove(hm[0].bestFrom, hm[0].bestTo);
            printf(" Score = %d\n",score);
            if (criticalError) {
                printf("Critical Error: 0x%02x\n",criticalError);
                showMaterial(0);
            } else {
                doMove(hm[0].bestFrom,hm[0].bestTo);
                printf("Nodes = %d\n",nodes);
            }
            // go and make move
        } else if (strcmp(command,"print") == 0) {
            printBoard();
        } else if(strcmp(command,"exit") == 0) {
            break;
        } else {
            printf("Unknown command, try again!\n");
        }
    }
    printf("Bye, bye!!\n");
    return 0;
}
