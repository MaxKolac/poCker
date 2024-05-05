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
 *
 *  \warning This function is redundant because MAX(a,b) exists, but I'm keeping it in case I ever need an example of a function with variadic arguments.
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
 *
 *  \warning This function is redundant because MIN(a,b) exists, but I'm keeping it in case I ever need an example of a function with variadic arguments.
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
