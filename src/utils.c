#include "utils.h"

#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
*   \brief Clamps a value to be constrained in a range.
*   \param val The value to clamp.
*   \param min The minimum value that can be returned.
*   \param max The maximum value that can be returned.
*   \returns The value if it's within the specified range. Otherwise, it returns the closest range boundary.
*/
int mathClamp(int val, int min, int max){
    if (val <= min)
        return min;
    else if (val >= max)
        return max;
    else
        return val;
}

/**
 *  \brief Randomizes an integer from a given inclusive range.
 *  \param lower_inc_bound Lower inclusive bound, clamped to be in range of (0, upper_inc_bound); both ends inclusive.
 *  \param upper_inc_bound Upper inclusive bound, clamped to be in range of (lower_inc_bound, RAND_MAX); both ends inclusive.
 *  \return A random integer from range of (lower_inc_bound, upper_inc_bound), both ends inclusive.
 *
 *  Make sure you call 'srand(time(NULL))' once and before ever calling this method!
 */
int randRange(int lower_inc_bound, int upper_inc_bound){
    lower_inc_bound = mathClamp(lower_inc_bound, 0, upper_inc_bound);
    upper_inc_bound = mathClamp(upper_inc_bound, lower_inc_bound, RAND_MAX);
    return lower_inc_bound + (rand() % (upper_inc_bound - lower_inc_bound));
}

/**
 *  \brief Returns the greatest integer provided in arguments.
 *  \param count Amount of integers in the arguments.
 *  \param ... Integers to compare.
 */
int mathMax(int count, ...) {
    //Declare a variadic arguments list
    va_list args;
    //Macro that returns integer's minimum value
    int max = INT_MIN;

    //Populate the args list with 'count' arguments
    va_start(args, count);
    for (int i = 0; i < count; i++) {
        // Retrieve the next argument
        int value = va_arg(args, int);
        if (value > max) {
            max = value;
        }
    }
    //Clean up the va_list
    va_end(args);

    return max;
}

/**
 *  \brief Returns the smallest integer provided in arguments.
 *  \param count Amount of integers in the arguments.
 *  \param ... Integers to compare.
 */
int mathMin(int count, ...){
    va_list args;
    int min = INT_MAX;
    va_start(args, count);
    for (int i = 0; i < count; i++){
        int value = va_arg(args, int);
        if (value < min)
            min = value;
    }
    va_end(args);
    return min;
}

/**
 *  \brief Prompts the user with a message for a positive non-zero integer value.
 *  \param max_length The maximum amount of input characters to consider
 *  \param msg The message to show to the user. The function appends a colon with a space at the end automatically.
 *  \returns The first input from user that can be considered a positive non-zero integer.
 *
 *  The function will not end until the user provides a valid string which can be considered an valid returnable integer.
 */
int prompt_i(int max_length, char* msg){
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
bool prompt_b(char* msg){
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
