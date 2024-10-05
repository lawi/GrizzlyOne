/*
 * IncFile1.h
 *
 * Created: 22.08.2024 23:34:44
 *  Author: lawi
 */

#include <stdlib.h>
#include <stdio.h>
#ifndef UI_H_
#define UI_H_
void printBoard();
void printMovelist(uint8_t ply);
void printMove(uint8_t from, uint8_t to);
void printLine(int count);
void dbgPuts(char *s);
void dbgInit(void);
void dbgPutc(uint8_t c);
void dbgSetColor (uint8_t color);
void dbgSendHexByte(uint8_t b);
void dbgSendHexWord(uint16_t w);
void dbgSendDec16(int16_t w);
void dbgSendDec32(int32_t w);
void dbgNewLine();
void dbgPrintf(char *s, ...);


#endif /* UI_H_ */