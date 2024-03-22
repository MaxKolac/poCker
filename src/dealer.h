#ifndef DEALER_H
#define DEALER_H

#include <stdbool.h>
#include "player.h"
#include "playingcard_enums.h"
#include "playingcard.h"

/**
* \file     dealer.h
* \brief    Contains function-equivalents of what a real-life table dealer would do in a real poker game.
*/

void distributeCards(struct PlayingCard[], struct Player[], struct PlayingCard*[]);
void buildDeck(struct PlayingCard[], bool);
void scorePlayersHand(struct Player, struct PlayingCard*[], int);
int decideWinners(struct Player[], int, int*);
bool checkPlayerDecisionValidity(struct Player, int, unsigned int, bool, int);

#endif // DEALER_H
