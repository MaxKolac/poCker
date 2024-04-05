#ifndef GAMERULES_H
#define GAMERULES_H

#include <stdbool.h>

extern const int MIN_FUNDS_PER_PLAYER;
extern const int MAX_FUNDS_PER_PLAYER;

typedef struct {
    /**
     *  The amount of funds each Player will start out with.
     */
    int funds_per_player;
    /**
     *  No-limit game means that the maximum amount of the bet isn't set, meaning players can bet as much as all of their funds.
     *  There is only the minimum amount of bet, equaling the big blind amount.
     *  Fixed-limit game restricts players to two fixed bet amounts, small limit which equals the big blind, and high limit
     *  which equals two times the big blind.
     */
    bool limit_fixed;
    /**
     *  This amount will influence the minimum bet amount and the pot's initial amount.
     *  Player first to the left of the dealer has to pay the small blind, and the next of him has to pay the big blind.
     *  Action starts on the third player and (assuming no raises) ends on the big blind player.
     */
    int big_blind;
    /**
     *  Small blind amount is automatically set to be half of it, rounded down.
     */
    int small_blind;
} GameRuleSet;

void promptFundsPerPlayer(GameRuleSet*);
void promptLimitFixed(GameRuleSet*);
void promptBigBlind(GameRuleSet*);

#endif // GAMERULES_H
