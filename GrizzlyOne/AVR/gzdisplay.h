/*
 * gzdisplay.h
 *
 * Created: 29.01.2022 18:46:44
 *  Author: Michael Lawatsch
 */ 


#ifndef GZDISPLAY_H_
#define GZDISPLAY_H_

extern uint8_t displayCharset[];

void displayInit();
void displayGreeting();
void displayShowLevel(uint8_t level);
void displayShowMove(uint8_t from, uint8_t to);
void fillDisplay(uint8_t character);

#endif /* GZDISPLAY_H_ */