#ifndef AI_H
#define AI_H

#include "player.h"
#include "gamestate.h"
#include "gamerules.h"

/**
 *  \file   ai.h
 *  \brief  Contains the logic of AI opponents.
 */

/**
 *  \brief Flags used by AI to recognize what decision it can take.
 *
 *  Implementation idea from here: https://stackoverflow.com/a/1631281/21342746
 */
typedef enum {
    FOLD = 1 << 0,
    CALL = 1 << 1,
    TAPOUT = 1 << 2,
    RAISE = 1 << 3
} AIDecision;

unsigned int ai_getAvailableDecisions(const GameRuleSet*, const GameState*, const Player*);
int ai_takeAction(const GameRuleSet*, const GameState*, const Player**);

#endif // AI_H
