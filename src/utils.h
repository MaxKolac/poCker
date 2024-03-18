#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

/**
 *  \file   utils.h
 *  \brief  Contains a collection of miscellaneous functions.
 */

int mathClamp(int, int, int);
int randRange(int, int);
int mathMax(int, ...);
int mathMin(int, ...);
int prompt_i(int, char*);
bool prompt_b(char*);

#endif  //UTILS_H
