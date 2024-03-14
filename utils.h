#include <ctype.h>
#include <stdarg.h>

/**
* \file     utils.h
* \brief    Contains a collection of miscellaneous functions.
*/

int isNumber(char[]);
int mathClamp(int, int, int);
int randRange(int, int);
int mathMax(int, ...);
int prompt(int);

/** \brief  Checks if the given char array can be considered a number.
 * \param   s A char array to check.
 * \return  1 if all characters were digits, 0 if at least one was not.
 *
 * Checks each element of the array with isdigit() method until it reaches null character.
 */
int isNumber(char s[]){
    for (int i = 0; s[i] != '\0'; i++){
        if (isdigit(s[i]) == 0){
            return 0;
        }
    }
    return 1;
}

/** \brief  Clamps a value to be constrained in a range.
* \param    val The value to clamp.
* \param    min The minimum value that can be returned.
* \param    max The maximum value that can be returned.
* \returns  The value if it's within the specified range. Otherwise, it returns the closest range boundary.
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
 * \brief  Randomizes an integer from a given inclusive range.
 * \param   lower_inc_bound Lower inclusive bound, clamped to be in range of (0, upper_inc_bound); both ends inclusive.
 * \param   upper_inc_bound Upper inclusive bound, clamped to be in range of (lower_inc_bound, RAND_MAX); both ends inclusive.
 * \return  A random integer from range of (lower_inc_bound, upper_inc_bound), both ends inclusive.
 *
 * Make sure you call 'srand(time(NULL))' once and before ever calling this method!
 */
int randRange(int lower_inc_bound, int upper_inc_bound){
    lower_inc_bound = mathClamp(lower_inc_bound, 0, upper_inc_bound);
    upper_inc_bound = mathClamp(upper_inc_bound, lower_inc_bound, RAND_MAX);
    return lower_inc_bound + (rand() % (upper_inc_bound - lower_inc_bound));
}

/**
 * \brief Returns the greatest integer provided in arguments.
 * \param count Amount of integers in the arguments.
 * \param ... Integers to compare.
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
 * \brief Prompts the user for an integer.
 * \param max_length The maximum length of the integer.
 * \return The first input from user which was a valid integer.
 *
 *  This function will not return for as long as user is giving strings which are not valid integers.
 */
int prompt(int max_length){
    char input[max_length];
    int result = 0;
    do {
        gets_s(&input, max_length);
        if (isNumber(input)){
            result = atoll(&input);
        }
    } while (!isNumber(input));
    return result;
}
