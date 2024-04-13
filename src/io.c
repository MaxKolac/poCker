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

