#ifndef IO_H
#define IO_H

#include <stdbool.h>

/**
 *  \file io.h
 *  \brief Contains functions responsible for two-communication with the human player through console inputs and outputs.
 */

int promptInt(int, char*);
bool promptBool(char*);
int recognizeDecision(char*);

#endif // IO_H
