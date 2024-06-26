#ifndef PLAYERIO_H
#define PLAYERIO_H

#include <stdbool.h>
#include "gamerules.h"
#include "gamestate.h"
#include "player.h"

/**
 *  \file playerio.h
 *  \brief Contains functions responsible for two-way communication with the human player through console inputs and outputs.
 */

/** \brief Maximum allowed length for user's decision. */
#define PLAYER_DECISION_LENGTH 16
/** \brief Quick macro for cleaning the screen. Works for Windows, not 100% sure for Linux. */
#define clearScreen() (system("@cls||clear"))

int promptInt(int, char*);
bool promptBool(char*);
void promptNull(char*);
int recognizeDecision(char*);
bool checkPlayerDecisionValidity(const Player*, const GameState*, const GameRuleSet*, int);

void printTitleScreen();
void printHeader(const GameState*);
void printPlayers(const GameRuleSet*, const GameState*, const Player*[]);
void printRaisesPotBet(const GameRuleSet*, const GameState*);
void printCards(const Player*, const PlayingCard*[], const int);
void printShowdownResults(const GameState*, const Player*[], const int[], const int);

#endif // PLAYERIO_H
