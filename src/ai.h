#ifndef AI_H
#define AI_H

#include "player.h"
#include "gamestate.h"
#include "gamerules.h"

/**
 *  \brief
 */

unsigned char checkAvailableDecisions(const Player*, const GameState*, const GameRuleSet*);

#endif // AI_H
