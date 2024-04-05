#ifndef PLAYINGCARD_H
#define PLAYINGCARD_H

#include <string.h>
#include "playingcard_enums.h"

/**
* \file     playingcard.h
* \brief    Contains functions, constants and structs related to playing cards.
*/

extern const int PIPS_PER_SUIT;
extern const int SUITS_COUNT;
extern const int DECK_LENGTH;

const char* getPipName(enum Pip);
const char* getSuitName(enum Suit);
void getCardName(PlayingCard*, char*, int);

#endif  //PLAYINGCARD_H
