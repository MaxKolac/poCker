#include "gamestate.h"

#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "dealer.h"
#include "gamerules.h"
#include "player.h"
#include "utils.h"

const int MAX_BETS_PER_ROUND_OBJ = MAX_BETS_PER_ROUND;

GameState* gsCreateNew(const GameRuleSet* rules){
    GameState* state = ((GameState*)malloc(sizeof(GameState)));
    state->revealed_comm_cards = 0;
    state->dealer_player = 0;
    state->s_blind_player = 1;
    state->b_blind_player = 2;
    state->current_player = 0;
    state->betting_round = 0;
    state->turns_left = rules->player_count - 1;
    state->raises_performed = 0;
    state->pot = 0;
    state->bet = 0;
    state->all_but_one_folded = false;
    return state;
}

/**
 *  \param player_dec_override Meant for unit-testing. A non-null pointer will override whatever the player's decision was. Keep in mind this value won't be validated!
 */
void gsAdvancePlayerTurn(GameState* state, Player* players[], unsigned int tapout_pot_statuses[], const GameRuleSet* ruleSet, const int* player_dec_override){
    //This player has not folded, we may perform an action
    //Wowza, practical use of De Morgan's law
    if (!(players[state->current_player]->folded || players[state->current_player]->tappedout)){
        //Player chooses an action and check if they even can do that
        //Validity checks should not be done by players themselves (they might cheat lol)
        int player_decision;
        if (player_dec_override == NULL){
            bool decisionValid = false;
            do {
                //player_decision = takeAction(players[state->current_player]);
                //player_decision = mathClamp(player_decision, -2, players[state->current_player]->funds);
                //decisionValid = checkPlayerDecisionValidity(players[state->current_player],
                //                                            ruleSet,
                //                                            player_decision,
                //                                            state->bet);
            } while(!decisionValid);
        }
        else{
            player_decision = *player_dec_override;
        }

        //Consequence of player's actions
        // 0 < player_decision signifies a RAISE by player_decision amount
        //Performing a RAISE increases the bet and demands that other players match it or fold.
        if (0 < player_decision){
            players[state->current_player]->funds -= player_decision;
            state->pot += player_decision;
            state->bet = player_decision;
            state->raises_performed++;
            state->turns_left = ruleSet->player_count;
        }
        // 0 == player_decision signifies a CALL/CHECK
        //CALL means that player is matching the current bet to stay in the game.
        //If the bet is 0, that means noone raised and instead it's called CHECK.
        else if (0 == player_decision){
            players[state->current_player]->funds -= state->bet;
            state->pot += state->bet;
        }
        // -1 == player_decision signifies a FOLD
        //Player folds his hand and no longer participates in the current game.
        //They do not need to match any raises, but they cannot win the pot.
        else if (-1 == player_decision){
            players[state->current_player]->folded = true;
            //Have everyone but one person folded?
            //This would indicate an auto-win of current pot for last unfolded player
            int folded_players = 0;
            for (int i = 0; i < ruleSet->player_count; i++){
                if (players[i]->folded){
                    folded_players++;
                }
            }
            state->all_but_one_folded = folded_players == ruleSet->player_count - 1;
        }
        // -2 == player decision signifies a TAPOUT
        //Special situation when a player wishes to CALL the bet, but can not afford it.
        //They are allowed to throw their remaining funds into the pot to continue with their current hand.
        //Once a player performs a TAPOUT, they have no obligation to match any RAISES and remain in the game.
        //However, if a TAPPEDOUT player wins the hand, they can only get the amount of the pot had at the time they tapped out.
        //The rest is paid to second best hand.
        else if (-2 == player_decision){
            players[state->current_player]->tappedout;
            tapout_pot_statuses[state->current_player] = state->pot;
        }

        //If this condition is true, we need to return ASAP. One player just got an auto-win.
        if (state->all_but_one_folded){
            return;
        }
    }
    state->current_player = (state->current_player + 1) % ruleSet->player_count;
    state->turns_left--;
}

void gsSetUpBettingRound(GameState* state, Player* players[], const GameRuleSet* ruleSet){
    //In each round, small blind (player to the left of dealer) is the first to act
    //Also, make sure to reset the bet back to 0
    if (state->betting_round != 0){
        state->current_player = state->s_blind_player;
        state->bet = 0;
        state->raises_performed = 0;
    }
    //Unless it's a pre-flop (beggining of a single game)
    //Force blind players to chip into the pot, without affecting the turns variable
    else {
        players[state->s_blind_player]->funds -= ruleSet->small_blind;
        state->pot += ruleSet->small_blind;
        players[state->b_blind_player]->funds -= ruleSet->big_blind;
        state->pot += ruleSet->big_blind;
        state->bet = ruleSet->big_blind;
        state->current_player = (state->b_blind_player + 1) % ruleSet->player_count;
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
void gsPerformShowdown(GameState* state, Player* players[], unsigned int tapout_pot_statuses[], const GameRuleSet* rules, const PlayingCard* comm_cards[]){
    int winners[rules->player_count];
    int winners_count = 0;
    //If the win occurred through everyone but one player folding:
    if (state->all_but_one_folded){
        //Find the player who did not fold, give them the whole pot
        for (int i = 0; i < rules->player_count; i++){
            if (!players[i]->folded){
                winners[0] = i;
                winners_count = 1;
                break;
            }
        }
    }
    else {
        //Else, compare cards and decide the winner(s)
        for (int i = 0; i < rules->player_count; i++){
            scorePlayersHand(players[i], comm_cards, state->revealed_comm_cards);
        }
        winners_count = decideWinners(*players, rules->player_count, winners);
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
bool gsCheckGameOverCondition(GameState* state, Player* players[], const GameRuleSet* rules){
    int broke_players = 0;
    for (int i = 0; i < rules->player_count; i++){
        if (players[i]->funds > 0){
            players[i]->folded = false;
        }
        else {
            players[i]->folded = true;
            broke_players++;
        }
    }
    return broke_players == rules->player_count - 1;
}

/**
 *  \brief Pass the dealer button to the next player. Done once after each full round.
 *  This also causes the blind player status to move.
 */
void gsPassDealerButton(GameState* state, const GameRuleSet* rules){
    state->dealer_player = (state->dealer_player + 1) % rules->player_count;
    state->s_blind_player = (state->s_blind_player + 1) % rules->player_count;
    state->b_blind_player = (state->b_blind_player + 1) % rules->player_count;
}
