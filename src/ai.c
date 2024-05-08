#include "ai.h"

#include "player.h"
#include "gamestate.h"
#include "gamerules.h"

//0000 0001 = CALL/CHECK
//0000 0010 = RAISE
//0000 0100 = TAPOUT
//Function assumes folded/tappedout players will not call this function at all, since they cant act
//Fold is always available so it is not stated explicitly
//TODO: remind myself to look at this function and create tests for it once I actually start working on AI
//since this is only meant for it
unsigned char checkAvailableDecisions(const Player* _player, const GameState* state, const GameRuleSet* rules){
    unsigned char result = 0;
    //Can we call/check? Aka, can the player can afford current bet
    if (_player->funds >= state->bet)
        result = 1;
    else
        //If we can't, we have can only tap-out
        return 4;

    //If we can call/check, can we also raise?
    //This means:
    // - raising still hasn't reached the round limit AND the game is fixed-limit
    // - player can afford a raise AND the game is fixed-limit
    // - game is no-limit AND their funds is greater than current bet.
    //(but do we tell the player how much can he raise it by??? -> lets assume their funds is the upper limit)
    if (
        (rules->limit_fixed && state->raises_performed < MAX_BETS_PER_ROUND &&
          (
            (_player->funds >= state->bet + rules->small_blind && state->betting_round <= 1) ||
            (_player->funds >= state->bet + rules->big_blind && state->betting_round > 1)
          )
        ) || (!rules->limit_fixed && _player->funds > state->bet)){
           result += 2;
    }
    return result;
}

/**
 *  \brief TODO
 */
 int takeAction(Player *self){
    return 0;
 }
