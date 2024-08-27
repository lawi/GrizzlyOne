/*
 * chessglobals.h
 *
 * Created: 29.01.2022 19:55:44
 *  Author: Michael Lawatsch
 */

#include <ctype.h>
#include <stdint.h>

#ifndef CHESSGLOBALS_H_
#define CHESSGLOBALS_H_

typedef struct {
    uint8_t from;
    uint8_t to;
    int16_t value;
} Move;

typedef struct {
    Move*    firstEntry;
    Move*    actEntry;
    int16_t  matWhite;
    int16_t  matBlack;
    int16_t  dynPosVal;
    int16_t  bestValue;
    uint8_t  bestFrom;
    uint8_t  bestTo;
    uint8_t  actFrom;
    uint8_t  actTo;
    uint8_t  flags;
    uint8_t  ep;
    uint8_t  capture;
    uint8_t  movesGenerated;
    uint8_t  bKingPos;
    uint8_t  wKingPos;
} HalfMove;

extern uint8_t  board[];
extern Move     mvList[];
extern HalfMove hm[];
extern uint8_t  maxDepth;
extern volatile uint8_t displayDigit;
extern volatile uint8_t displayBuffer[4];
extern uint8_t  side;
extern uint8_t  ply;
extern uint8_t  plyMax;
extern uint8_t  criticalError;
extern int32_t nodes;
extern volatile uint8_t keysPressed;
extern volatile uint8_t funKeysPressed;
// Values
extern const int16_t pieceValues[];
extern const int16_t centerValues[];
extern const int8_t moffKnight[];
extern const int8_t moffKing[];
extern const int8_t moffRook[];
extern const int8_t moffBishop[];
extern const int8_t moffQueen[];

// keyboard and display
#ifndef NULL
#define NULL  0
#endif
#define CHR_0 0xc0
#define CHR_1 0xf9
#define CHR_2 0xa4
#define CHR_3 0xb0
#define CHR_4 0x99
#define CHR_5 0x92
#define CHR_6 0x82
#define CHR_7 0xf8
#define CHR_8 0x80
#define CHR_9 0x90
#define CHR_A 0x88
#define CHR_B 0x83
#define CHR_C 0xc6
#define CHR_D 0xa1
#define CHR_E 0x86
#define CHR_F 0x8e
#define CHR_G 0xc2
#define CHR_H 0x89
#define CHR_MINUS 0xBF
#define CHR_R 0xAF
#define CHR_N 0xAB
#define CHR_L 0xc7
#define CHRSET_OFFSET_1 1
#define CHRSET_OFFSET_A 10
#define KEY_GO 0x01
#define KEY_CL 0x02
#define KEY_FN 0x04

#define VAL_PAWN     100
#define VAL_KNIGHT   300
#define VAL_BISHOP   320
#define VAL_ROOK     500
#define VAL_QUEEN    900
#define VAL_KING     0
#define INFINTE_VAL  32000

#define  PAWN 		0x01
#define  KNIGHT 	0x02
#define  BISHOP		0x03
#define  ROOK		0x04
#define  QUEEN		0x05
#define  KING		0x06
#define  WPAWN 		0x09
#define  WKNIGHT 	0x0A
#define  WBISHOP	0x0B
#define  WROOK		0x0C
#define  WQUEEN		0x0D
#define  WKING		0x0E
#define  BPAWN		0x11
#define  BKNIGHT	0x12
#define  BBISHOP	0x13
#define  BROOK		0x14
#define  BQUEEN		0x15
#define  BKING		0x16
#define  EMPTY      0x00
#define  WMASK		0x08
#define  WBIT		3
#define  BMASK		0x10
#define  BBIT		4
#define  WHITE_MOVE   0x08
#define  WHITE        0x08
#define  BLACK_MOVE   0x10
#define  BLACK        0x10
#define  OTHER(s)     (s^0x18)
#define  SAME_COLOR(f,t) (board[f]&board[t]&0x18)
#define  OUTSIDE(f)   (f&0x88)
#define  ON_BOARD(f)  (!(f&0x88))
#define  PROMO_FLAG   0x80

#define  FLAG_CS_WHITE 0x01
#define  FLAG_CL_WHITE 0x02
#define  FLAG_CS_BLACK 0x04
#define  FLAG_CL_BLACK 0x08

#define  SQ_A1		 0x00
#define  SQ_B1		 0x01
#define  SQ_C1		 0x02
#define  SQ_D1		 0x03
#define  SQ_E1		 0x04
#define  SQ_F1		 0x05
#define  SQ_G1		 0x06
#define  SQ_H1		 0x07
#define  SQ_A2		 0x10
#define  SQ_B2		 0x11
#define  SQ_C2		 0x12
#define  SQ_D2		 0x13
#define  SQ_E2		 0x14
#define  SQ_F2		 0x15
#define  SQ_G2		 0x16
#define  SQ_H2		 0x17
#define  SQ_A3		 0x20

#define  SQ_A7		 0x60
#define  SQ_B7		 0x61
#define  SQ_C7		 0x62
#define  SQ_D7		 0x63
#define  SQ_E7		 0x64
#define  SQ_F7		 0x65
#define  SQ_G7		 0x66
#define  SQ_H7		 0x67
#define  SQ_A8		 0x70
#define  SQ_B8		 0x71
#define  SQ_C8		 0x72
#define  SQ_D8		 0x73
#define  SQ_E8		 0x74
#define  SQ_F8		 0x75
#define  SQ_G8		 0x76
#define  SQ_H8		 0x77

#define COLOR_BLACK    0
#define COLOR_RED      1
#define COLOR_GREEN    2
#define COLOR_YELLOW   3
#define COLOR_BLUE     4
#define COLOR_MANGENTA 5
#define COLOR_CYAN     6
#define COLOR_WHITE    7

#define MAX_HMCOUNT    22
#define MAX_MOVECOUNT  300


#endif /* CHESSGLOBALS_H_ */