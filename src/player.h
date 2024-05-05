#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>
#include "playingcard.h"

/**
* \file player.h
* \brief Contains a Player struct and related functions.
*/

/** \brief Defines the size of the scores[] array. */
#define SCORE_TABLE_SIZE 10

extern const int SCORE_TABLE_SIZE_OBJ;

/** \brief Represents a single player. */
typedef struct {
    /** \brief Player's current amount of funds. If this number reaches 0, the player looses. */
    int funds;
    /** \brief Tells if this player has already folded their cards in the current round. */
    bool folded;
    /** \brief Tells if this player has tapped out in the current round. */
    bool tappedout;
    /** \brief A score table for individual hand ranks to help break up ties. See handranking.c for more info. */
    int scores[10];
    /** \brief Pointers to player's playing cards in his hand. */
    PlayingCard* current_hand[CARDS_PER_PLAYER];
    /** \brief Whether or not this player is controlled through AI or by human. */
    bool isHuman;
} Player;

Player* playerCreateNew();
Player* playerCreateNewWithFunds(int);
int takeAction(Player*);
void resetScores(Player*);

#endif // PLAYER_H
