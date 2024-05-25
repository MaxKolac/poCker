#ifndef DEALER_H
#define DEALER_H

#include <stdbool.h>
#include "gamerules.h"
#include "gamestate.h"
#include "player.h"
#include "playingcard.h"

/**
 *  \file dealer.h
 *  \brief Contains function-equivalents of what a real-life table dealer would do in a real poker game.
 */

void buildDeck(PlayingCard[], bool);
int decideWinners(Player*[], int, int*);
void distributeCards(PlayingCard[], Player*[], PlayingCard*[], const GameRuleSet*);
void scorePlayersHand(Player*, const PlayingCard*[], int);

#endif // DEALER_H
