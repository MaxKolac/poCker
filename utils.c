#include <stdlib.h>

/**
* \file     utils.c
* \brief    Contains a collection of miscellaneous functions.
*/

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

/** \brief  Randomizes an integer from a given inclusive range.
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
