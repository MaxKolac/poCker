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

/**
 *  \brief List of strategies the AI will choose from at the beginning of each game.
 */
typedef enum {
    /** \brief AI will stay in the game until the showdown, attempting to match most calls. */
    FIGHT,
    /** \brief AI will attempt to stay in the game and pretend as if it has better cards than it actually holds. */
    BLUFF,
    /** \brief AI will fold on its next action, as it has deemed the stakes to be against its favour. */
    ABANDON
} AIStrategy;

unsigned int ai_getAvailableDecisions(const GameRuleSet*, const GameState*, const Player*);
int ai_takeAction(const GameRuleSet*, const GameState*, const Player**);
AIStrategy ai_determineStrategy(const GameRuleSet*, const Player*, float (*)());

#endif // AI_H
