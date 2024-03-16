#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>
#include "playingcard.h"

/**
* \file player.h
* \brief Contains only a Player struct.
*/

/** \brief Represents a single player. */
struct Player {
    /** \brief Player's current amount of funds. If this number reaches 0, the player looses. */
    int funds;
    /** \brief Tells if this player has already folded their cards in the current round. */
    bool folded;
    /** \brief A score table for individual hand ranks to help break up ties. See handranking.c for more info. */
    int scores[10];
    /** \brief Pointers to player's playing cards in his hand. */
    struct PlayingCard* current_hand[2];
};

#endif // PLAYER_H
