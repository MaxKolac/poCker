#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <stdbool.h>
#include "constants.h"
#include "gamerules.h"
#include "player.h"
#include "playingcard.h"

typedef struct {
    int revealed_comm_cards;
    int dealer_player;
    int s_blind_player;
    int b_blind_player;
    int current_player;
    int betting_round;
    int turns_left;
    bool all_but_one_folded;
    unsigned int pot;
    unsigned int bet;
} GameState;

GameState* gsCreateNew();
void gsAdvancePlayerTurn();
void gsSetUpBettingRound(GameState*, Player*[], const GameRuleSet*);
void gsConcludeBettingRound(GameState*);
void gsPerformShowdown(GameState*, Player*[], const PlayingCard*[]);
bool gsCheckGameOverCondition(GameState*, Player*[]);
void gsPassDealerButton(GameState*);

#endif // GAMESTATE_H