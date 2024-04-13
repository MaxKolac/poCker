#include "io.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

/**
 *  \brief Prompts the user with a message for a positive non-zero integer value.
 *  \param max_length The maximum amount of input characters to consider
 *  \param msg The message to show to the user. The function appends a colon with a space at the end automatically.
 *  \returns The first input from user that can be considered a positive non-zero integer.
 *
 *  The function will not end until the user provides a valid string which can be considered an valid returnable integer.
 */
int promptInt(int max_length, char* msg){
    char input[max_length];
    int result = 0;
    do {
        printf("%s: ", msg);
        gets_s(&input, max_length);
        result = atoll(&input);
    } while (result <= 0);
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
        gets_s(&input, 5);
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
    int arg_int = atoi(arg);

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
 *  \param response The char array that will be filled with a message, in case the player tried to perform an illegal action.
 *  \return True, if the player is allowed. False, otherwise.
 *  \warning Make sure that the response array is always the length of PDVC_MSG_LENGTH!
 */
bool checkPlayerDecisionValidity(const Player* _player, const GameState* state, const GameRuleSet* rules, int player_decision, char response[]){
    for (int i = 0; i < IO_RESPONSE_LENGTH; ++i)
        response[i] = '\0';

    //For raises:
    if (player_decision > 0){
        if (rules->limit_fixed){
            //Has the raise limit been reacher?
            if (state->raises_performed >= MAX_BETS_PER_ROUND){
                strcpy(response, "The limit of raises per one betting round has already been reached.");
                return 0;
            }
            //The game's fixed-limit and we are in the first half of it. Can player afford the raise by small blind amount?
            if (state->betting_round <= 1 && _player->funds < state->bet + rules->small_blind){
                strcpy(response, "You cannot afford to raise the bet by the required small blind amount."); //greatest length here - 71 chars incl. null char
                return 0;
            }
            //The game's fixed-limit and we are in the second half of it. Can player afford the raise by big blind amount?
            if (state->betting_round > 1 && _player->funds < state->bet + rules->big_blind){
                strcpy(response, "You cannot afford to raise the bet by the required big blind amount.");
                return 0;
            }
        }
        else {
            //The game's no limit, can player even afford their decision?
            if (_player->funds < player_decision){
                strcpy(response, "You cannot afford to raise the bet by the specified amount.");
                return 0;
            }
            //Is player trying to lower the bet?
            if (player_decision <= state->bet){
                strcpy(response, "You cannot lower the bet, it can only be raised up.");
                return 0;
            }
        }
    }
    //For calls/checks:
    else if (player_decision == 0){
        //Can the player afford to call?
        if (_player->funds < state->bet){
            strcpy(response, "You cannot afford to call the bet.");
            return 0;
        }
    }
    //For tap outs:
    else if (player_decision == -2){
        if (_player->funds >= state->bet){
            strcpy(response, "You can still afford to call the current bet. You may not tap out just yet.");
            return 0;
        }
    }
    //For folds, no checks need to be performed
    return 1;
}
