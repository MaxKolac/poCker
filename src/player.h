#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>
#include "playingcard.h"
#include "gamerules.h"

/**
*   \file player.h
*   \brief Contains a Player struct and related functions.
*/

/** \brief Defines the size of the scores array. */
#define SCORE_TABLE_SIZE 10

/** \brief Represents a single player. */
typedef struct {
    /** \brief Player's hole cards. */
    PlayingCard* current_hand[CARDS_PER_PLAYER];
    /** \brief Tells if this player has already folded their cards in the current round. */
    bool folded;
    /** \brief Player's current amount of funds. If this number reaches 0, the player looses. */
    int funds;
    /** \brief Whether or not this Player is controlled through AI or by human. */
    bool isHuman;
    /** \brief Contains a log of this Player's past decision in the current game. */
    int past_decisions[MAX_ROUNDS_PER_GAME * (MAX_BETS_PER_ROUND + 1)];
    /** \brief Current size of the past_decisions array.
     *
     *  The greatest value of this variable would be a scenario where a Player checks, and then calls all subsequent MAX_BETS_PER_ROUND raises, on every betting round.*/
    int past_decisions_size;
    /** \brief A score table for individual hand ranks to help break up ties. See handranking.c for more info. */
    int scores[SCORE_TABLE_SIZE];
    /** \brief Tells if this player has tapped out in the current round. */
    bool tappedout;
    /** \brief The amount of funds the pot held at the time of this Player tappingout. If they win the showdown, this Player will receive only this much. */
    unsigned int tappedout_funds;
} Player;

Player* playerCreateNew();
Player* playerCreateNewWithFunds(int);
void resetScores(Player*);

#endif // PLAYER_H
