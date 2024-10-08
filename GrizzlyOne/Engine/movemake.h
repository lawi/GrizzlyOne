/*
 * movemake.h
 *
 * Created: 29.01.2022 19:39:27
 *  Author: Michael Lawatsch
 */ 

#ifndef MOVEMAKE_H_
#define MOVEMAKE_H_

uint8_t nextMove(uint8_t capture);
void backMove();
void doMove(uint8_t from, uint8_t to);
void undoMove();

uint8_t inCkeck();

#endif /* MOVEMAKE_H_ */