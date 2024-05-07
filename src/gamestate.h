#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <stdbool.h>
#include "gamerules.h"
#include "player.h"
#include "playingcard.h"

/**
 *  \file gamestate.h
 *  \brief Contains the logic of a poker game in a form of DI'able functions. See their arrangement and implementation in main.c.
 */

#define MAX_BETS_PER_ROUND 3
#define MAX_ROUNDS_PER_GAME 4

extern const int MAX_BETS_PER_ROUND_OBJ;
extern const int MAX_ROUNDS_PER_GAME_OBJ;

/**
 *  \brief Represents the current state of a poker game.
 *  Recommended to initialize this struct through gsCreateNew(), but hey I'm not your mom.
 */
typedef struct {
    /** \brief Amount of community cards which are "face-up" meaning visible to players. */
    int revealed_comm_cards;
    /** \brief Index of the player with dealer's button. */    int dealer_player;
    /** \brief Index of the player with "small blind" status. */
    int s_blind_player;
    /** \brief Index of the player with "big blind" status. */
    int b_blind_player;
    /** \brief Index of the player who currently has a chance to act. */
    int current_player;
    /** \brief Number of the betting round. There are 4 rounds: pre-flop, flop, turn and river. */
    int betting_round;
    /** \brief How many turns are left until the current betting round ends. */
    int turns_left;
    /** \brief How many raises have been called by all players in the current betting round. */
    int raises_performed;
    /** \brief Boolean to indicate if all but one player have folded their cards - this indicates an auto-win for that player. */
    bool all_but_one_folded;
    /** \brief Amount currently in the pot. */
    unsigned int pot;
    /** \brief Amount of the current bet. */
    unsigned int bet;
} GameState;

GameState* gsCreateNew(const GameRuleSet*);
void gsAdvancePlayerTurn(GameState*, Player*[], const GameRuleSet*, const int*);
void gsSetUpBettingRound(GameState*, Player*[], const GameRuleSet*);
void gsConcludeBettingRound(GameState*);
int gsDetermineWinners(int[], const GameRuleSet*, const GameState*, const Player*[], const PlayingCard*[]);
void gsAwardPot(GameState*, Player*[], const int[], const int);
bool gsCheckGameOverCondition(Player*[], const GameRuleSet*);
void gsPassDealerButton(GameState*, const GameRuleSet*);
void gsConcludeSingleGame(GameState*);

#endif // GAMESTATE_H
