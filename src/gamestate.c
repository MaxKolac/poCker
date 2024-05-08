#include "gamestate.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "ai.h"
#include "dealer.h"
#include "gamerules.h"
#include "messages.h"
#include "playerio.h"
#include "player.h"
#include "utils.h"

/**
 *  \brief Creates a new, clean and unconfigured GameState struct.
 *  \param rules The GameRuleSet struct to base the creation of the GameState on.
 *  \returns A pointer to the newly created struct.
 */
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
 *  \brief Sets the members of a GameState struct according to its current betting_round value.
 *  \param state The GameState struct to modify.
 *  \param players An array of Players which might be modified if the round being set up is the pre-flop.
 *  \param ruleSet The GameRuleSet dictating current game's rules regarding small and big blinds.
 */
void gsSetUpBettingRound(GameState* state, Player* players[], const GameRuleSet* ruleSet){
    //In each round, small blind (player to the left of dealer) is the first to act
    //Also, make sure to reset the bet back to 0
    if (state->betting_round != 0){
        state->current_player = state->s_blind_player;
        state->bet = 0;
        state->raises_performed = 0;
        state->turns_left = ruleSet->player_count;
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
        state->turns_left = ruleSet->player_count - 1;
    }
}


/**
 *  \brief Advances the player's turn by allowing them to take action and applying any changes causes by the chosen action.
 *  \param state The current state of the game to modify accordingly.
 *  \param players An array of Players to modify accordingly.
 *  \param ruleSet The GameRuleSet on which some logic is based.
 *  \param player_dec_override Meant for unit-testing. A non-null pointer will override whatever the player's decision was. Keep in mind this value won't be validated!
 */
void gsAdvancePlayerTurn(GameState* state, Player* players[], const GameRuleSet* ruleSet, const int* player_dec_override){
    Player* currentPlayer = players[state->current_player];
    //This player has not folded, we may perform an action
    //Wowza, practical use of De Morgan's law
    if (!(currentPlayer->folded || currentPlayer->tappedout)){
        //Player chooses an action and check if they even can do that
        //Validity checks should not be done by players themselves (they might cheat lol)
        int player_decision;
        if (player_dec_override == NULL){
            if (currentPlayer->isHuman){
                //For human players
                bool decisionValid = false;
                do {
                    char input[PLAYER_DECISION_LENGTH];
                    MSG_SHOWVS(GLOBAL_MSGS, "GAMESTATE_HUMANPROMPT", state->current_player + 1);
                    gets_s(input, PLAYER_DECISION_LENGTH);
                    player_decision = recognizeDecision(input);
                    decisionValid = checkPlayerDecisionValidity(currentPlayer, state, ruleSet, player_decision);
                } while(!decisionValid);
            }
            else {
                //for AI players - for now this function always returns 0 - CHECK
                player_decision = takeAction(currentPlayer);
                promptNull(msgGet(GLOBAL_MSGS, "NULL_PROMPT_NEXTTURN"));
            }
        }
        else {
            player_decision = *player_dec_override;
        }
        //Debug
        //MSG_SHOWVN(GLOBAL_MSGS, "PIO_DEBUG_DECISION", players[state->current_player]->isHuman ? "Human" : "AI", player_decision);

        //WARNING! This part assumes the player's decision was allowed and valid!
        //Consequence of player's actions
        // 0 < player_decision signifies a RAISE by player_decision amount
        //Performing a RAISE increases the bet and demands that other players match it or fold.
        if (0 < player_decision){
            if (ruleSet->limit_fixed){
                player_decision = (state->betting_round <= 1) ? state->bet + ruleSet->small_blind : state-> bet + ruleSet->big_blind;
            }
            currentPlayer->funds -= player_decision;
            state->pot += player_decision;
            state->bet = player_decision;
            state->raises_performed++;
            state->turns_left = ruleSet->player_count; //This gets decremented at the end of function
        }
        // 0 == player_decision signifies a CALL/CHECK
        //CALL means that player is matching the current bet to stay in the game.
        //If the bet is 0, that means noone raised and instead it's called CHECK.
        else if (0 == player_decision){
            currentPlayer->funds -= state->bet;
            state->pot += state->bet;
        }
        // -1 == player_decision signifies a FOLD
        //Player folds his hand and no longer participates in the current game.
        //They do not need to match any raises, but they cannot win the pot.
        else if (-1 == player_decision){
            currentPlayer->folded = true;
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
            state->pot += players[state->current_player]->funds;
            currentPlayer->funds = 0;
            currentPlayer->tappedout = true;
            currentPlayer->tappedout_funds = state->pot;
        }

        //If this condition is true, we need to return ASAP. One player just got an auto-win.
        if (state->all_but_one_folded){
            return;
        }

        //Record the chosen decision
        currentPlayer->past_decisions[currentPlayer->past_decisions_size] = player_decision;
        currentPlayer->past_decisions_size++;
    }
    state->current_player = (state->current_player + 1) % ruleSet->player_count;
    state->turns_left--;
}

/**
 *  \brief Sets the amount of revealed community cards and increments the betting_round value of the passed GameState struct.
 *  \param state The GameState struct to modify.
 */
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
 *  \brief Determines the winner(s) and populates the winners array with their indexes.
 *  \param winners An array which will be filled with Player indexes.
 *  \param rules The GameRuleSet struct containing the amount of Players.
 *  \param state The GameState containing all_but_one_folded and revealed community cards count.
 *  \param players An array of Players to modify accordingly.
 *  \param comm_cards The community cards to analyze when determining who has got the best handrank.
 *  \returns The size of the populated winners array. In other words, how many winners there are.
 *
 *  If the win happened because of everyone else folding, only one player who did not fold is awarded the whole pot.
 *  Otherwise, everyone's hands are compared, winners are pulled and awarded their fair share.
 */
int gsDetermineWinners(int winners[],
                       const GameRuleSet* rules,
                       const GameState* state,
                       const Player* players[],
                       const PlayingCard* comm_cards[]){
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
        winners_count = decideWinners(players, rules->player_count, winners);
    }
    return winners_count;
}

/**
 *  \brief Determines the winner(s) and pays them their winnings from the pot.
 *  \param state The GameState struct to modify accordingly.
 *  \param players An array of Players to modify accordingly.
 *  \param winners An array containing indexes of winning Players.
 *  \param winners_count The size of the winners array.
 *
 *  Tapped out winners receive their rewards first, the rest is then divided evenly between other winners.
 *  If the pot happened to be indivisible by amount of winners, small blind player receives the remainder.
 */
void gsAwardPot(GameState* state, Player* players[], const int winners[], const int winners_count){
    //If we have a single winner, they take the whole pot;
    if (winners_count == 1){
        //Unless they tapped out, which means they only get a portion of it and the rest goes to small blind player.
        int adjusted_pot_amount = players[winners[0]]->tappedout_funds;
        if (adjusted_pot_amount > 0){
            players[winners[0]]->funds += adjusted_pot_amount;
            players[state->s_blind_player]->funds += (state->pot - adjusted_pot_amount);
        }
        else {
            players[winners[0]]->funds += state->pot;
        }

    }
    //Otherwise, pay it to individual winners evenly.
    //In the event of a pot being indivisible by winner's count, the remainder is paid to the player to the left of dealer.
    else {
        //If one of the winners tapped out, they will not receive the full pot/winners_count.
        //Instead they are awarded the amount of funds the pot held at the time of their tapping out.
        int tappedout_players_count = 0;
        for (int i = 0; i < winners_count; i++){
            int adjusted_pot_amount = players[winners[i]]->tappedout_funds;
            if (adjusted_pot_amount > 0){
                tappedout_players_count++;
                players[winners[i]]->funds += adjusted_pot_amount;
                state->pot -= adjusted_pot_amount;
            }
        }
        //Once tappedout winners are awarded their smaller share, other winners can enjoy the full award_per_player amount
        int award_per_player = floorf(state->pot / mathMax(2, 1, winners_count - tappedout_players_count));
        //If all the winners were tapped out, each iteration of this for loop is completely skipped
        //What the tapped out winners couldn't receieve, goes to the small blind player.
        for (int i = 0; i < winners_count; i++){
            //Make sure not to accidentally give the winnings to the tappedout player again
            if (players[winners[i]]->tappedout_funds > 0){
                continue;
            }
            players[winners[i]]->funds += award_per_player;
            state->pot -= award_per_player;
        }
        if (state->pot > 0){
            players[state->s_blind_player]->funds += state->pot;
        }
    }
    state->pot = 0;
}

/**
 *  \brief Checks if everyone but one player has any funds left.
 *  \param players An array of Players to be modified accordingly.
 *  \param rules GameRuleSet containing the amount of Players.
 *  \returns True, if the whole game should be ended. False, otherwise.
 *
 *  If the condition is true, this would indicate end of the whole game.
 *  This function also reverts the "folded" and "tappedout" statuses for all Players with any funds left.
 *  Broke players are marked as folded right away, preventing them from being able to act again.
 */
bool gsCheckGameOverCondition(Player* players[], const GameRuleSet* rules){
    int broke_players = 0;
    for (int i = 0; i < rules->player_count; i++){
            players[i]->tappedout = false;
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
 *  \brief Pass the dealer button to the next player. Done once after each full round. This also causes the blind player statuses to move.
 *  \param state The GameState struct to modify.
 *  \param rules The GameRuleSet containing the amount of Players.
 */
void gsPassDealerButton(GameState* state, const GameRuleSet* rules){
    state->dealer_player = (state->dealer_player + 1) % rules->player_count;
    state->s_blind_player = (state->s_blind_player + 1) % rules->player_count;
    state->b_blind_player = (state->b_blind_player + 1) % rules->player_count;
}

/**
 *  \brief Prepares the GameState struct for a fresh game while preserving dealer button positions.
 *  \param state The GameState struct to modify.
 *
 *  Before entering a pre-flop betting round, make sure to call gsSetUpBettingRound() function.
 */
void gsConcludeSingleGame(GameState* state){
    state->revealed_comm_cards = 0;
    state->betting_round = 0;
    state->all_but_one_folded = false;
}
