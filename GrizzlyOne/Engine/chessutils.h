/*
 * chessutils.h
 *
 * Created: 01.02.2022 19:12:46
 *  Author: Michael Lawatsch
 */


#ifndef CHESSUTILS_H_
#define CHESSUTILS_H_

void newGame();
void clearBoard();
void showMaterial(uint8_t n);
void getMaterial(uint8_t n);
uint8_t checkBoard();
uint8_t isAttacked(uint8_t square, uint8_t color);

#endif /* CHESSUTILS_H_ */