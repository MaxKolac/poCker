#include "ai.h"

#include <math.h>
#include <stdlib.h>
#include "player.h"
#include "gamestate.h"
#include "gamerules.h"
#include "handranking.h"
#include "utils.h"

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
 *  \brief Acts as a execution chain for other AI functions. Returns the AI's decision as an integer.
 *
 *  Return values are:
 *  - greater than 0 - AI wants to RAISE by the returned value.
 *  - 0 - AI wants to CALL/CHECK.
 *  - -1 - AI wishes to FOLD.
 *  - -2 - AI wants to TAPOUT.
 */
int ai_takeAction(const GameRuleSet* rules, const GameState* state, const Player** players){
    unsigned int decisions = ai_getAvailableDecisions(rules, state, players[state->current_player]);
    switch (ai_determineStrategy(rules, players[state->current_player], randFloat))
    {
        case FIGHT:
            if (decisions & RAISE)
            {
                return 1;
            }
            else if (decisions & CALL)
            {
                return 0;
            }
            else if (decisions & TAPOUT)
            {
                return -2;
            }
            else
            {
                return -1;
            }
            break;
        case BLUFF:
            if (decisions & RAISE)
            {
                return 1;
            }
            else if (decisions & CALL)
            {
                return 0;
            }
            else if (decisions & TAPOUT)
            {
                return -2;
            }
            else
            {
                return -1;
            }
            break;
        case ABANDON:
            if (decisions & TAPOUT){
                return -2;
            }
            else {
                return -1;
            }
            break;
    }
}

/**
 *  \brief Calculates weights for each AIStrategy enum and returns one for the AI to try and follow.
 *  \param rules Current GameRuleSet struct. Only the funds_per_player member will be read.
 *  \param self The current Player. Their current_hand and funds will be read.
 *  \param rngFunction A pointer to a parameterless function which will return a random float from an inclusive range of (0.0 - 1.0).
 *  \returns One of the AIStrategy enums, chosen pseudo-randomly.
 */
AIStrategy ai_determineStrategy(const GameRuleSet* rules, const Player* self, float (*rngFunction)()){
    const int pairMaxScore = ACE * pow(20, 3);
    const int handMaxScore = ACE + ACE;

    int pairRankScore = detectPair(self->current_hand, CARDS_PER_PLAYER);
    int handScore = self->current_hand[0]->pips + self->current_hand[1]->pips;

    //The odds of AI trying to fight legitimately
    float fightWeight = pairRankScore != 0 ? //If we have a pair (which is a strong opening hand)
                        CLAMP(2.0f * pairRankScore / pairMaxScore, 0.0f, 1.0f) : //Then odds of fighting should be somewhat great, based on how strong the pair is
                        handScore / handMaxScore; //Otherwise, odds of fighting should be high only if we have strong cards

    //The odds of AI bluffing and acting like they own better cards
    //The opposite of fightWeight divided by two and multiplied by how much they can risk to bluff, based on current funds ratio to what we started with.
    float bluffWeight = MAX(0.0f, ((1.0f - fightWeight) / 2.0f) * (CLAMP(self->funds / rules->funds_per_player, 0.0f, 2.0f) - 0.5f + fightWeight));

    //Now arrange the odds and decide on the strategy
    float rngResult = CLAMP(rngFunction(), 0.0f, 1.0f);
    if (rngResult <= fightWeight){
        return FIGHT;
    }
    else if (rngResult <= fightWeight + bluffWeight){
        return BLUFF;
    }
    else {
        return ABANDON;
    }
}
