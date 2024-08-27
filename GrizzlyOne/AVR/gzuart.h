/*
 * gzuart.h
 *
 * Created: 29.01.2022 18:02:14
 *  Author: Michael Lawatsch
 */

#include "stdint.h"

#ifndef GZUART_H_
#define GZUART_H_

void uartInit(void);
void uartPutc(uint8_t c);
void uartPuts (char *s);
void setColor (uint8_t color);
void uartSendHexByte(uint8_t b);
void uartSendHexWord(uint16_t w);
void uartSendDec16(int16_t w);
void uartSendDec32(int32_t w);
void newLine();

#endif /* GZUART_H_ */