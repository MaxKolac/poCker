#ifndef GAMERULES_H
#define GAMERULES_H

#include <stdbool.h>

/**
 *  \file gamerules.h
 *  \brief Contains the GameRuleSet struct and functions related to its initialization through console prompts.
 */

#define MAX_BETS_PER_ROUND 3
#define MAX_ROUNDS_PER_GAME 4

extern const int MAX_BETS_PER_ROUND_OBJ;
extern const int MAX_ROUNDS_PER_GAME_OBJ;
extern const int MIN_PLAYER_COUNT;
extern const int MAX_PLAYER_COUNT;
extern const int MIN_FUNDS_PER_PLAYER;
extern const int MAX_FUNDS_PER_PLAYER;

/**
 *  \brief Represents a set of customizable rules of the poker game.
 *
 *  Recommended to initialize it through prompt functions, unless used for unit-tests.
 */
typedef struct {
    /** \brief Amount of all players, AI and humans included. */
    int player_count;
    /** \brief Amount of players that are controlled by AI. The rest is controlled through human prompts. */
    int ai_player_count;
    /** \brief The amount of funds each Player will start out with. */
    int funds_per_player;
    /**
     *  \brief Whether or not the bets will be fixed.
     *
     *  No-limit game means that the maximum amount of the bet isn't set, meaning players can bet as much as all of their funds.
     *  There is only the minimum amount of bet, equaling the big blind amount.
     *  Fixed-limit game restricts players to two fixed bet amounts, small limit which equals the big blind, and high limit
     *  which equals two times the big blind.
     */
    bool limit_fixed;
    /**
     *  \brief Big blind bet amount.
     *
     *  This amount will influence the minimum bet amount and the pot's initial amount.
     *  Player first to the left of the dealer has to pay the small blind, and the next of him has to pay the big blind.
     *  Action starts on the third player and (assuming no raises) ends on the big blind player.
     */
    int big_blind;
    /** \brief Small blind bet amount. It's automatically set to be big blind divided by two and rounded down. */
    int small_blind;
} GameRuleSet;

void promptPlayerCount(GameRuleSet*);
void promptAIPlayersCount(GameRuleSet*);
void promptFundsPerPlayer(GameRuleSet*);
void promptLimitFixed(GameRuleSet*);
void promptBigBlind(GameRuleSet*);

#endif // GAMERULES_H
