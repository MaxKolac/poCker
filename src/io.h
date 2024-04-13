#ifndef IO_H
#define IO_H

#include <stdbool.h>
#include "gamerules.h"
#include "gamestate.h"
#include "player.h"

/**
 *  \file io.h
 *  \brief Contains functions responsible for two-communication with the human player through console inputs and outputs.
 */

#define IO_DECISION_LENGTH 16
#define IO_RESPONSE_LENGTH 72

int promptInt(int, char*);
bool promptBool(char*);
int recognizeDecision(char*);
bool checkPlayerDecisionValidity(const Player*, const GameState*, const GameRuleSet*, int, char[]);

#endif // IO_H
