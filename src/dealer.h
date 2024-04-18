#ifndef DEALER_H
#define DEALER_H

#include <stdbool.h>
#include "gamerules.h"
#include "gamestate.h"
#include "player.h"
#include "playingcard.h"

/**
* \file     dealer.h
* \brief    Contains function-equivalents of what a real-life table dealer would do in a real poker game.
*/

void distributeCards(PlayingCard[], Player[], PlayingCard*[], const GameRuleSet*);
void buildDeck(PlayingCard[], bool);
void scorePlayersHand(Player*, const PlayingCard*[], int);
int decideWinners(Player*[], int, int*);
unsigned char checkAvailableDecisions(const Player*, const GameState*, const GameRuleSet*);

#endif // DEALER_H
