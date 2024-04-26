#ifndef IO_H
#define IO_H

#include <stdbool.h>
#include "gamerules.h"
#include "gamestate.h"
#include "player.h"

/**
 *  \file io.h
 *  \brief Contains functions responsible for two-way communication with the human player through console inputs and outputs.
 */

#define IO_DECISION_LENGTH 16
#define IO_RESPONSE_LENGTH 72

int promptInt(int, char*);
bool promptBool(char*);
int recognizeDecision(char*);
bool checkPlayerDecisionValidity(const Player*, const GameState*, const GameRuleSet*, int, char[]);

void printGameState(const GameState*);
void printCommunityCards(const PlayingCard*[], const int);
void printHoleCards(const Player*);
void printShowdownResults(const int[], const int, const Player*[]);

#endif // IO_H
