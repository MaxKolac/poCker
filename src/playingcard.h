#ifndef PLAYINGCARD_H
#define PLAYINGCARD_H

#include <string.h>

/**
* \file     playingcard.h
* \brief    Contains functions, constants and structs related to playing cards.
*/

const char* getPipName(enum Pip);
const char* getSuitName(enum Suit);
void getCardName(struct PlayingCard*, char*, int);

#endif  //PLAYINGCARD_H
