#ifndef PLAYINGCARD_H
#define PLAYINGCARD_H

#include <string.h>
#include "playingcard_enums.h"

/**
* \file     playingcard.h
* \brief    Contains functions, constants and structs related to playing cards.
*/

#define PIPS_PER_SUIT 13
#define SUITS_COUNT 4
#define DECK_LENGTH (4 * 13)

extern const int PIPS_PER_SUIT_OBJ;
extern const int SUITS_COUNT_OBJ;
extern const int DECK_LENGTH_OBJ;

const char* getPipName(enum Pip);
const char* getSuitName(enum Suit);
void getCardName(PlayingCard*, char*, int);

#endif  //PLAYINGCARD_H
