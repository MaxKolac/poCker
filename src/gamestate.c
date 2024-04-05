#include "gamestate.h"

#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "dealer.h"
#include "gamerules.h"
#include "player.h"
#include "utils.h"

GameState* gsCreateNew(){
    GameState* state = ((GameState*)malloc(sizeof(GameState)));
    state->revealed_comm_cards = 0;
    state->dealer_player = 0;
    state->s_blind_player = 1;
    state->b_blind_player = 2;
    state->current_player = 0;
    state->betting_round = 0;
    state->turns_left = PLAYER_COUNT - 1;
    state->pot = 0;
    state->bet = 0;
    state->all_but_one_folded = false;
    return state;
}

void gsAdvancePlayerTurn(GameState* state, Player* players[], GameRuleSet* ruleSet){
    //This player has folded, skip his turn
    if (!(players[state->current_player]->folded)){
        //Player chooses an action and check if they even can do that
        //Validity checks should not be done by players themselves (they might cheat lol)
        int player_decision;
        bool decisionValid = false;
        do {
            player_decision = takeAction(players[state->current_player]);
            player_decision = mathClamp(player_decision, -1, players[state->current_player]->funds);
            decisionValid = checkPlayerDecisionValidity(players[state->current_player],
                                                        ruleSet,
                                                        player_decision,
                                                        state->bet);
        } while(!decisionValid);

        //Consequence of player's actions
        // 0 < player_decision signifies a RAISE by player_decision amount
        // 0 == player_decision signifies a CALL/CHECK
        // -1 == player_decision signifies a FOLD
        // -2 == player decision signifies a TAPOUT
        if (0 <= player_decision){
            players[state->current_player]->funds -= player_decision;
            state->pot += player_decision;

            if (0 < player_decision){
                state->bet = player_decision;
                state->turns_left = PLAYER_COUNT - 1;
            }
        }
        else if (-1 == player_decision){
            players[state->current_player]->folded = true;
            //Have everyone but one person folded?
            //This would indicate an auto-win of current pot for last unfolded player
            int folded_players = 0;
            for (int i = 0; i < PLAYER_COUNT; i++){
                if (players[i]->folded){
                    folded_players++;
                }
            }
            state->all_but_one_folded = folded_players == PLAYER_COUNT - 1;
        }
        else if (-2 == player_decision){
            //TODO: tapouts
        }

        //If this condition is true, we need to return ASAP. One player just got an auto-win.
        if (state->all_but_one_folded){
            return;
        }
    }
    state->current_player = (state->current_player + 1) % PLAYER_COUNT;
    state->turns_left--;
}

void gsSetUpBettingRound(GameState* state, Player* players[], const GameRuleSet* ruleSet){
    //In each round, small blind (player to the left of dealer) is the first to act
    if (state->betting_round != 0){
        state->current_player = state->s_blind_player;
    }
    //Pre-flop always starts out like this.
    //Force blind players to chip into the pot, without affecting the turns variable
    else {
        players[state->s_blind_player]->funds -= ruleSet->small_blind;
        state->pot += ruleSet->small_blind;
        players[state->b_blind_player]->funds -= ruleSet->big_blind;
        state->pot += ruleSet->big_blind;
        state->bet = ruleSet->big_blind;
        state->current_player = (state->b_blind_player + 1) % PLAYER_COUNT;
    }
}

void gsConcludeBettingRound(GameState* state){
    //Pre-flop has no cards.
    //Flop has 3 cards revealed.
    //Turns has 4, and river has 5.
    switch (state->betting_round){
        case 0:
            state->revealed_comm_cards = 3;
            break;
        case 1:
        case 2:
            state->revealed_comm_cards++;
            break;
    }
    state->betting_round++;
}

/**
 *  \brief Determines the winner(s) and pays them their winnings from the pot.
 *  If the win happened because of everyone else folding, only one player who did not fold is awarded the whole pot.
 *  Otherwise, everyone's hands are compared, winners are pulled and awarded their fair share.
 */
void gsPerformShowdown(GameState* state, Player* players[], const PlayingCard* comm_cards[]){
    int winners[PLAYER_COUNT];
    int winners_count = 0;
    //If the win occurred through everyone but one player folding:
    if (state->all_but_one_folded){
        //Find the player who did not fold, give them the whole pot
        for (int i = 0; i < PLAYER_COUNT; i++){
            if (!players[i]->folded){
                winners[0] = i;
                winners_count = 1;
                break;
            }
        }
    }
    else {
        //Else, compare cards and decide the winner(s)
        for (int i = 0; i < PLAYER_COUNT; i++){
            scorePlayersHand(players[i], comm_cards, state->revealed_comm_cards);
        }
        winners_count = decideWinners(players, PLAYER_COUNT, winners);
    }

    //If we have a single winner, they take the whole pot;
    if (winners_count == 1){
        players[winners[0]]->funds += state->pot;
    }
    //Otherwise, pay it to individual winners evenly.
    //In the event of a pot being indivisible by winner's count, the remainder is paid to the player to the left of dealer.
    else {
        for (int i = 0; i < winners_count; i++){
            int amount = floorf(state->pot / winners_count);
            players[winners[i]]->funds += amount;
            state->pot -= amount;
        }
        if (state->pot > 0){
            players[state->s_blind_player]->funds += state->pot;
        }
    }
}

/**
 *  \brief Checks if everyone but one player has any funds left.
 *  If the condition is true, this would indicate end of the game.
 *  This function also unmarks all Players with funds left. Broke players are marked as folded right away.
 */
bool gsCheckGameOverCondition(GameState* state, Player* players[]){
    int broke_players = 0;
    for (int i = 0; i < PLAYER_COUNT; i++){
        if (players[i]->funds > 0){
            players[i]->folded = false;
        }
        else {
            players[i]->folded = true;
            broke_players++;
        }
    }
    return broke_players == PLAYER_COUNT - 1;
}

/**
 *  \brief Pass the dealer button to the next player. Done once after each full round.
 *  This also causes the blind player status to move.
 */
void gsPassDealerButton(GameState* state){
    state->dealer_player = (state->dealer_player + 1) % PLAYER_COUNT;
    state->s_blind_player = (state->s_blind_player + 1) % PLAYER_COUNT;
    state->b_blind_player = (state->b_blind_player + 1) % PLAYER_COUNT;
}
