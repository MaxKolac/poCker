#ifndef GAMERULES_H
#define GAMERULES_H

#include <stdbool.h>

/**
 *  \file gamerules.h
 *  \brief Contains the GameRuleSet struct and functions related to its initialization through console prompts.
 */

/** \brief Maximum amount of raises per a single betting round. */
#define MAX_BETS_PER_ROUND 3
/** \brief Maximum amount of betting rounds until a showdown is performed. */
#define MAX_ROUNDS_PER_GAME 4
/** \brief Minimum amount of Players. */
#define MIN_PLAYER_COUNT 3
/** \brief Maximum amount of Players. */
#define MAX_PLAYER_COUNT 12
/** \brief Minimum amount of starting funds for each Player. */
#define MIN_FUNDS_PER_PLAYER 100
/** \brief Maximum amount of starting funds for each Player. Increasing this might displace some elements of the in-game UI. */
#define MAX_FUNDS_PER_PLAYER 10000

/**
 *  \brief Represents a set of customizable rules of the poker game.
 *
 *  Recommended to initialize it through prompt functions, unless used for unit-tests.
 */
typedef struct {
    /** \brief Amount of players that are controlled by AI. The rest is controlled through human prompts. */
    int ai_player_count;
    /**
     *  \brief Big blind bet amount.
     *
     *  This amount will influence the minimum bet amount and the pot's initial amount.
     *  Player first to the left of the dealer has to pay the small blind, and the next of him has to pay the big blind.
     *  Action starts on the third player and (assuming no raises) ends on the big blind player.
     */
    int big_blind;
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
    /** \brief Amount of all players, AI and humans included. */
    int player_count;
    /** \brief Small blind bet amount. It's automatically set to be big blind divided by two and rounded down. */
    int small_blind;
} GameRuleSet;

void promptAIPlayersCount(GameRuleSet*);
void promptBigBlind(GameRuleSet*);
void promptFundsPerPlayer(GameRuleSet*);
void promptLimitFixed(GameRuleSet*);
void promptPlayerCount(GameRuleSet*);

#endif // GAMERULES_H
