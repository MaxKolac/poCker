#include "gamerules.h"

#include <math.h>
#include <stdio.h>
#include "constants.h"
#include "utils.h"

const int MIN_FUNDS_PER_PLAYER = 100;
const int MAX_FUNDS_PER_PLAYER = 100000;

/**
 *  \brief Prompts the user for the initial funds per player.
 *  \param grs The GameRuleSet struct to modify.
 */
void promptFundsPerPlayer(struct GameRuleSet* grs){
    int ini_funds_per_player = -1;
    do {
        char msg[128];
        snprintf(msg,
                 sizeof(msg),
                 "Set the initial amount of funds for each player. Min - %d, Max - %d",
                 MIN_FUNDS_PER_PLAYER,
                 MAX_FUNDS_PER_PLAYER);
        ini_funds_per_player = prompt_i(6, msg);
    } while (ini_funds_per_player < MIN_FUNDS_PER_PLAYER || MAX_FUNDS_PER_PLAYER < ini_funds_per_player);
    //Debug
    //printf("Initial funds per player: %d\n", ini_funds_per_player);
    grs->funds_per_player = ini_funds_per_player;
}

/**
 *  \brief Prompts the user to make the game fixed-limit or no-limit.
 *  \param grs The GameRuleSet struct to modify.
 */
void promptLimitFixed(struct GameRuleSet* grs){
    //TODO: i'm unable to find a site which would finally explain the following:
    // - fixed limits means that players can only raise by high and small limits, or by any amount inbetween?
    bool limit_fixed = prompt_b("Should the betting limits be fixed?");
    //Debug
    //printf("Limits are fixed: %s\n", limit_fixed ? "true" : "false");
    grs->limit_fixed = limit_fixed;
}

/**
 *  \brief Prompts the user to set the big blind amount. This also sets the small amount.
 *  \param grs The GameRuleSet struct to modify.
 *  \warning This needs to be called before setting the funds_per_player!
 */
void promptBigBlind(struct GameRuleSet* grs){
    int big_blind = -1;
    do {
        big_blind = prompt_i(6, "Set the big blind amount. Minimum is 2, maximum is 10% of funds per player");
    } while (big_blind < 2 || floorf(grs->funds_per_player * 0.1) < big_blind);
    int small_blind = floorf(big_blind / 2);
    //Debug
    //printf("Big blind: %d, Small blind: %d\n", big_blind, small_blind);
    grs->big_blind = big_blind;
    grs->small_blind = small_blind;
}
