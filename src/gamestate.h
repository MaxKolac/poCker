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

/**
 *  \brief Represents the current state of a poker game.
 *
 *  Recommended to initialize this struct through gsCreateNew(), but hey I'm not your mom.
 */
typedef struct {
    /** \brief Boolean to indicate if all but one player have folded their cards - this indicates an auto-win for that Player. */
    bool all_but_one_folded;
    /** \brief Index of the Player with "big blind" status. */
    int b_blind_player;
    /** \brief Amount of the current bet. */
    unsigned int bet;
    /** \brief Number of the betting round. There are 4 rounds: pre-flop, flop, turn and river. */
    int betting_round;
    /** \brief Index of the Player who is currently deciding what action to take. */
    int current_player;
    /** \brief Index of the Player with dealer's button. */    int dealer_player;
    /** \brief Amount currently in the pot. */
    unsigned int pot;
    /** \brief How many raises have been called by all players in the current betting round. */
    int raises_performed;
    /** \brief Amount of community cards which are "face-up" meaning visible to players. */
    int revealed_comm_cards;
    /** \brief Index of the Player with "small blind" status. */
    int s_blind_player;
    /** \brief How many turns are left until the current betting round ends. */
    int turns_left;
} GameState;

GameState* gsCreateNew(const GameRuleSet*);
void gsSetUpBettingRound(GameState*, Player*[], const GameRuleSet*);
void gsAdvancePlayerTurn(GameState*, Player*[], const GameRuleSet*, const int*);
void gsConcludeBettingRound(GameState*);
int gsDetermineWinners(int[], const GameRuleSet*, const GameState*, const Player*[], const PlayingCard*[]);
void gsAwardPot(GameState*, Player*[], const int[], const int);
bool gsCheckGameOverCondition(Player*[], const GameRuleSet*);
void gsPassDealerButton(GameState*, const GameRuleSet*);
void gsConcludeSingleGame(GameState*);

#endif // GAMESTATE_H
