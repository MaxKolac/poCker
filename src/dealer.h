#ifndef DEALER_H
#define DEALER_H

#include <stdbool.h>
#include "gamerules.h"
#include "player.h"
#include "playingcard_enums.h"
#include "playingcard.h"

/**
* \file     dealer.h
* \brief    Contains function-equivalents of what a real-life table dealer would do in a real poker game.
*/

void distributeCards(struct PlayingCard[], Player[], struct PlayingCard*[]);
void buildDeck(struct PlayingCard[], bool);
void scorePlayersHand(Player, struct PlayingCard*[], int);
int decideWinners(Player[], int, int*);
bool checkPlayerDecisionValidity(const Player*, const struct GameRuleSet*, int, unsigned int);

#endif // DEALER_H
