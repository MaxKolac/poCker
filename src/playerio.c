#include "playerio.h"

#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "messages.h"
#include "gamerules.h"
#include "gamestate.h"
#include "utils.h"

/**
 *  \brief Prompts the user with a message for a positive integer or zero value.
 *  \param max_length The maximum amount of input characters to consider
 *  \param msg The message to show to the user. The function appends a colon with a space at the end automatically.
 *  \returns The first input from user that can be considered a positive integer or zero. Rules of strtol() function apply.
 *
 *  The function will not end until the user provides a valid string which can be considered an valid returnable integer.
 */
int promptInt(int max_length, char* msg){
    char input[max_length];
    int result = 0;
    do {
        printf("%s: ", msg);
        gets_s(&input, max_length);
        result = strtol(input, NULL, 10);
    } while (result < 0);
    return result;
}

/**
 *  \brief Prompts the user with a message for a boolean value.
 *  \param msg The message to show to the user. The function appends a ' (Y/N): ' at the end automatically.
 *  \returns User's choice as bool.
 *
 *  The function will not end until the user provides one of the two possible, case-insensitive inputs.
 *  Only the first character of user's input will be evaluated. That means that for example: "yFFFF" will be considered true.
 *  Y and y returns True, N and n returns false.
 */
bool promptBool(char* msg){
    char input[5];
    bool input_valid = false;
    bool result;
    do {
        printf("%s (Y/N): ", msg);
        gets_s(input, 5);
        if (input[0] == 'y' || input[0] == 'Y'){
            result = true;
            input_valid = true;
        }
        else if (input[0] == 'n' || input[0] == 'N'){
            result = false;
            input_valid = true;
        }
    } while (!input_valid);
    return result;
}

/**
 *  \brief Recognizes one of the predetermined string literals in the input parameter and returns it as the corresponding integer.
 *  \returns The amount to raise by for RAISES, 0 for CALL/CHECK, -1 for FOLD, -2 for TAPOUT, INT_MIN for unrecognized decision.
 *
 *  Available decisions are "call", "check", "raise", "fold" and "tapout".
 *  Function is case-insensitive. Raises can be made of two parts separated with a space,
 *  where first part is the "raise" decision and the second is an integer indicating by
 *  how many funds should the bet be raised.
 */
int recognizeDecision(char* input){
    if (input == NULL || input[0] == '\0')
        return INT_MIN;

    //Determine input length and if possible, turn it chars into lowercase
    int i = 0;
    while (input[i] != '\0'){
        if (isalpha(input[i]))
            input[i] = tolower(input[i]);
        i++;
    }

    //Splice the array
    char command[i + 1];
    char arg[i + 1];

    char* delimiter = " ";
    char* first_token = strtok(input, delimiter);
    if (first_token != NULL)
        strcpy(command, first_token);
    else
        return INT_MIN;

    char* second_token = strtok(NULL, delimiter);
    if (second_token != NULL)
        strcpy(arg, second_token);
    int arg_int = strtol(arg, NULL, 10);

    //Recognition
    if (strcmp(command, "raise") == 0){
        //You can't raise by 0, so the 0 from non-integer parameters passed to atoi defaults to returning 1
        return arg_int <= 0 ? 1 : arg_int;
    }
    else if (strcmp(command, "check") == 0 || strcmp(command, "call") == 0){
        return 0;
    }
    else if (strcmp(command, "fold") == 0){
        return -1;
    }
    else if (strcmp(command, "tapout") == 0){
        return -2;
    }
    return INT_MIN;
}

/**
 *  \brief Checks that the player is allowed to do their action. Function meant for human players and their custom inputs.
 *  \return True, if the player is allowed. False, otherwise.
 *  \warning Make sure that the response array is always the length of MESSAGES_MAX_MSG_LENGTH!
 */
bool checkPlayerDecisionValidity(const Player* _player, const GameState* state, const GameRuleSet* rules, int player_decision){
    //For raises:
    if (player_decision > 0){
        if (rules->limit_fixed){
            //Has the raise limit been reacher?
            if (state->raises_performed >= MAX_BETS_PER_ROUND){
                MSG_SHOWN(GLOBAL_MSGS, "PIO_CPDV_RAISE_LIMIT");
                return 0;
            }
            //The game's fixed-limit and we are in the first half of it. Can player afford the raise by small blind amount?
            if (state->betting_round <= 1 && _player->funds < state->bet + rules->small_blind){
                MSG_SHOWN(GLOBAL_MSGS, "PIO_CPDV_RAISE_SB_AMOUNT");
                return 0;
            }
            //The game's fixed-limit and we are in the second half of it. Can player afford the raise by big blind amount?
            if (state->betting_round > 1 && _player->funds < state->bet + rules->big_blind){
                MSG_SHOWN(GLOBAL_MSGS, "PIO_CPDV_RAISE_BB_AMOUNT");
                return 0;
            }
        }
        else {
            //The game's no limit, can player even afford their decision?
            if (_player->funds < player_decision){
                MSG_SHOWN(GLOBAL_MSGS, "PIO_CPDV_RAISE_GENERIC");
                return 0;
            }
            //Is player trying to lower the bet?
            if (player_decision <= state->bet){
                MSG_SHOWN(GLOBAL_MSGS, "PIO_CPDV_RAISE_TOLOWER");
                return 0;
            }
        }
    }
    //For calls/checks:
    else if (player_decision == 0){
        //Can the player afford to call?
        if (_player->funds < state->bet){
            MSG_SHOWN(GLOBAL_MSGS, "PIO_CPDV_CALL");
            return 0;
        }
    }
    //For folds, no checks need to be performed
    //For tap outs:
    else if (player_decision == -2){
        if (_player->funds >= state->bet){
            MSG_SHOWN(GLOBAL_MSGS, "PIO_CPDV_TAPOUT");
            return 0;
        }
    }
    //For unrecognized player decision, it's an automatic false
    else if (player_decision == INT_MIN){
        MSG_SHOWN(GLOBAL_MSGS, "PIO_CPDV_UNKNOWN");
        return 0;
    }

    return 1;
}

/**
 *  \brief Prints the UI header which includes the current round and amount of turns left in it.
 */
void printHeader(const GameState* state){
    MSG_SHOWVN(GLOBAL_MSGS, "PIO_HEADER", state->betting_round + 1, MAX_ROUNDS_PER_GAME, state->turns_left - 1);
}

/**
 *  \brief Prints the table showing information about all the players, including their funds, last decision, etc.
 */
void printPlayers(const GameRuleSet* rules, const GameState* state, const Player* players[]){
    //For each greater row
    int tableRows = ceilf(rules->player_count / 4.0);
    for (int i = 0; i < tableRows; i++){
        int playersToPrint = MIN(rules->player_count - (i * 4), 4);

        //PIO_PLAYER_NAME
        for (int j = 0; j < playersToPrint; j++){
            int currentPlayer = i * 4 + j;
            char* playerStatus;
            if (state->b_blind_player == currentPlayer){
                playerStatus = "(BB)";
            }
            else if (state->s_blind_player == currentPlayer){
                playerStatus = "(SB)";
            }
            else if (state->dealer_player == currentPlayer){
                playerStatus = "(DL)";
            }
            else {
                playerStatus = "    ";
            }
            MSG_SHOWVS(GLOBAL_MSGS, "PIO_PLAYER_NAME",
                      currentPlayer + 1,
                      players[currentPlayer]->isHuman ? "(H)" : "(AI)",
                      playerStatus);
        }
        printf("|\n");

        //PIO_PLAYER_LASTDECISION
        for (int j = 0; j < playersToPrint; j++){
            int currentPlayer = i * 4 + j;
            char* lastDecision;
            if (state->current_player == currentPlayer){
                lastDecision = "Deciding...";
            }
            else if (players[currentPlayer]->folded){
                lastDecision = "Folded";
            }
            else if (players[currentPlayer]->tappedout){
                lastDecision = "Tappedout";
            }
            else {
                //TODO: choice of a literal in this scope should depend on player's last recorded decision
                lastDecision = "TODO!";
            }
            MSG_SHOWVS(GLOBAL_MSGS, "PIO_PLAYER_LASTDECISION", lastDecision);
        }
        printf("|\n");

        //PIO_PLAYER_FUNDS
        for (int j = 0; j < playersToPrint; j++){
            int currentPlayer = i * 4 + j;
            MSG_SHOWVS(GLOBAL_MSGS, "PIO_PLAYER_FUNDS", players[currentPlayer]->funds);
        }
        printf("|\n");
        if (i != tableRows - 1){
            MSG_SHOWN(GLOBAL_MSGS, "DIVIDER_4COL");
        }
    }
}

/**
 *  \brief Prints the intermediary header which shows the raises left, current pot and bet.
 */
void printRaisesPotBet(const GameRuleSet* rules, const GameState* state){
    char* bet_variant;
    if (rules->limit_fixed){
        bet_variant = state->betting_round < 2 ? "(Small Blind)" : "(Big Blind)";
    }
    else {
        bet_variant = "(No limit)";
    }
    MSG_SHOWVN(GLOBAL_MSGS, "PIO_PLAYER_NUMBERS",
               MAX_BETS_PER_ROUND - state->raises_performed,
               bet_variant,
               state->pot,
               state->bet);
}

/**
 *  \brief Prints the second table showing revealed community cards and current player's hole cards.
 *  \param player If this parameter is NULL, the right column with player's cards won't be printed. Only community cards will be visible.
 */
void printCards(const Player* player, const PlayingCard* comm_cards[], const int revealed_cards){
    MSG_SHOWVN(GLOBAL_MSGS, "PIO_CARDS_HEADER", player != NULL ? "| Your cards:" : "" );
    if (player != NULL){
        //Print community cards and player cards
        for (int i = 0; i < CARDS_PER_PLAYER; i++){
            char commCard[CARDNAME_MAX_LENGTH];
            char holeCard[CARDNAME_MAX_LENGTH];
            if (i < revealed_cards){
                getCardName(comm_cards[i], commCard, CARDNAME_MAX_LENGTH);
            }
            else {
                strcpy(commCard, "???");
            }
            getCardName(player->current_hand[i], holeCard, CARDNAME_MAX_LENGTH);
            MSG_SHOWVN(GLOBAL_MSGS, "PIO_CARDS_ROW_DOUBLE",
                       commCard,
                       holeCard);
        }
        for (int i = CARDS_PER_PLAYER; i < COMM_CARDS_COUNT; i++){
            char commCard[CARDNAME_MAX_LENGTH];
            if (i < revealed_cards){
                getCardName(comm_cards[i], commCard, CARDNAME_MAX_LENGTH);
            }
            else {
                strcpy(commCard, "???");
            }
            MSG_SHOWVN(GLOBAL_MSGS, "PIO_CARDS_ROW_SINGLE", commCard, '|');
        }
    }
    else {
        //Print only community cards
        for (int i = 0; i < COMM_CARDS_COUNT; i++){
            char commCard[CARDNAME_MAX_LENGTH];
            if (i < revealed_cards){
                getCardName(comm_cards[i], commCard, CARDNAME_MAX_LENGTH);
            }
            else {
                strcpy(commCard, "???");
            }
            MSG_SHOWVN(GLOBAL_MSGS, "PIO_CARDS_ROW_SINGLE", commCard, ' ');
        }
    }
}

void printShowdownResults(const GameState* state, const Player* players[], const int winners[], const int winners_count){
    MSG_SHOWVN(GLOBAL_MSGS, "PIO_SHOWDOWN_HEADER", state->pot);
    MSG_SHOWN(GLOBAL_MSGS, "DIVIDER_1COL");
    for (int i = 0; i < winners_count; i++){
        char firstCard[CARDNAME_MAX_LENGTH];
        char secondCard[CARDNAME_MAX_LENGTH];
        Player* currentPlayer = players[winners[i]];
        int highestNonzeroScoreTier = 0;
        if (state->all_but_one_folded){
            highestNonzeroScoreTier = -1;
        }
        else {
            while (highestNonzeroScoreTier < SCORE_TABLE_SIZE && currentPlayer->scores[highestNonzeroScoreTier] == 0){
                highestNonzeroScoreTier++;
            }
        }
        char* highestHandrank;
        switch (highestNonzeroScoreTier){
            case -1: highestHandrank ="Everyone folded"; break;
            case 0: highestHandrank = "Royal Flush"; break;
            case 1: highestHandrank = "Straight Flush"; break;
            case 2: highestHandrank = "Four-of-a-Kind"; break;
            case 3: highestHandrank = "Full House"; break;
            case 4: highestHandrank = "Flush"; break;
            case 5: highestHandrank = "Straight"; break;
            case 6: highestHandrank = "Three-of-a-Kind"; break;
            case 7: highestHandrank = "Two Pair"; break;
            case 8: highestHandrank = "Pair"; break;
            case 9: highestHandrank = "High Card"; break;
            default: highestHandrank = "None"; break;
        }
        getCardName(currentPlayer->current_hand[0], firstCard, CARDNAME_MAX_LENGTH);
        getCardName(currentPlayer->current_hand[1], secondCard, CARDNAME_MAX_LENGTH);
        MSG_SHOWVN(GLOBAL_MSGS, "PIO_SHOWDOWN_ROW",
                   winners[i] + 1,
                   highestHandrank,
                   firstCard,
                   secondCard);
    }
}
