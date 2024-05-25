#include "ai.h"

#include "player.h"
#include "gamestate.h"
#include "gamerules.h"

/**
 *  \brief Tells the caller what decisions are valid for the current state of the game.
 *  \warning This function assumes it won't be called by tappedout/folded players, since by default they shouldn't be able to act.
 *  \returns An unsigned integer made up of AIDecision enum flags.
 */
unsigned int ai_getAvailableDecisions(const GameRuleSet* rules, const GameState* state, const Player* player){
    //Fold is assumed to be always available
    unsigned int result = FOLD;

    //Can we call/check?
    if (player->funds >= state->bet){
        result = result | CALL;
    }
    //If we can't, we have can only tap-out
    else {
        return result | TAPOUT;
    }

    //If we can call/check, can we raise?
    //If MAX_BETS_PER_ROUND is reached, we can't. Simple as.
    if (state->raises_performed == MAX_BETS_PER_ROUND){
        return result;
    }

    //For fixed limit games:
    if (rules->limit_fixed){
        //Depending on the current round number:
        int minAmountToRaiseBy = state->betting_round <= 1 ?
                                 state->bet + rules->small_blind :
                                 state->bet + rules->big_blind;
        //A bet can be raised by at least the respective blind amount.
        return player->funds >= minAmountToRaiseBy ? result | RAISE : result;
    }
    //For no-limit games, the raise can be as little as by 1$.
    return player->funds >= state->bet + 1 ? result | RAISE : result;
}

/**
 *  \brief Analyzes the current game situation and return a decision as an integer.
 */
 int ai_takeAction(const GameRuleSet* rules, const GameState* state, const Player** players){
    return 0;
 }
