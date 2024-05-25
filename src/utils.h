#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

/**
 *  \file   utils.h
 *  \brief  Contains a collection of miscellaneous functions.
 */

/** \brief Quick generic macro for getting the larger number. */
#define MAX(a,b) (a > b ? a : b)
/** \brief Quick generic macro for getting the smaller number. */
#define MIN(a,b) (a < b ? a : b)

int mathClamp(int, int, int);
int randRange(int, int);
int mathMax(int, ...);
int mathMin(int, ...);

#endif  //UTILS_H
