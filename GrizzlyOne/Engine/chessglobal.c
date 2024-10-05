/*
 * chessglobal.c
 *
 * Created: 29.01.2022 19:51:47
 *  Author: Michael Lawatsch
 */
// #include <avr/io.h>

#include "chessglobals.h"

// This section should contain all globals.
// Hopefully the position can be optimized for speed

uint8_t board[128];
Move mvList[MAX_MOVECOUNT];
Move line[23];
HalfMove hm[MAX_HMCOUNT];
uint8_t maxDepth;
uint8_t side;
uint8_t ply;
uint8_t plyMax;
uint8_t criticalError;
int32_t nodes;

// Values
const int16_t pieceValues[24] = { 0,0,0,0,0,0,0,0,0, VAL_PAWN, VAL_KNIGHT, VAL_BISHOP, VAL_ROOK, VAL_QUEEN, VAL_KING, 0,0,
    VAL_PAWN, VAL_KNIGHT, VAL_BISHOP, VAL_ROOK, VAL_QUEEN, VAL_KING ,0};

const int16_t centerValues[128] = { -10, -5, -5,  -3, -3, -5, -5, -10,  0,  0,  0,  0,  0, 0,   0,  0,
                                     -5, -3, -2,   0,  0, -2, -3,  -5,  2,  3,  3,  0,  0, 3,   3,  2,
                                     -5, -1,  2,   5,  5,  2, -1,  -5,  3,  4,  4,  5,  5, 4,   4,  3,
                                     -2,  3,  7,  10, 10,  7,  3,  -2,  6,  7,  8, 10, 10, 8,   7,  6,
                                     -2,  3,  7,  10, 10,  7,  3,  -2, 13, 14, 15, 17, 17, 15, 14, 13,
                                     -5, -1,  2,   5,  5,  2, -1,  -5, 20, 21, 25, 28, 28, 25, 21, 20,
                                     -5, -3, -2,   0,  0, -2, -3,  -5, 43, 44, 47, 50, 50, 47, 44, 43,
                                    -10, -5, -5,  -3, -3, -5, -5, -10,  0,  0,  0,  0,  0,  0,  0,  0};

// Offsets
const int8_t moffKnight[10] =  { 0x0e,0x1F,0x21,0x12,-0x0e,-0x12,-0x21,-0x1F,0x00, 0x00 };
const int8_t moffKing[10]   =  { 0x10,0x01,-0x10,-0x01,0x11,-0x0F,-0x11,0x0F,0x00, 0x00 };
const int8_t moffRook[6]    =  { 0x10,0x01,-0x10,-0x01,0x00, 0x00 };
const int8_t moffBishop[6]  =  { 0x11,-0x0f,-0x11,0x0f,0x00, 0x00 };
const int8_t moffQueen[10]  =  { 0x10,0x01,-0x10,-0x01,0x11,-0x0F,-0x11,0x0F,0x00, 0x00 }; // ROOK + BISHOP

volatile uint8_t displayDigit;
volatile uint8_t displayBuffer[4] = {0xff, 0xff, 0xff, 0xff};
volatile uint8_t keysPressed;
volatile uint8_t funKeysPressed;

