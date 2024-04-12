#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <stdbool.h>
#include "constants.h"
#include "gamerules.h"
#include "player.h"
#include "playingcard.h"

#define MAX_BETS_PER_ROUND 3

extern const int MAX_BETS_PER_ROUND_OBJ;

typedef struct {
    int revealed_comm_cards;
    int dealer_player;
    int s_blind_player;
    int b_blind_player;
    int current_player;
    int betting_round;
    int turns_left;
    int raises_performed;
    bool all_but_one_folded;
    unsigned int pot;
    unsigned int bet;
} GameState;

GameState* gsCreateNew(const GameRuleSet*);
void gsAdvancePlayerTurn(GameState*, Player*[], unsigned int[], const GameRuleSet*, const int*);
void gsSetUpBettingRound(GameState*, Player*[], const GameRuleSet*);
void gsConcludeBettingRound(GameState*);
void gsPerformShowdown(GameState*, Player*[], unsigned int[], const GameRuleSet*, const PlayingCard*[]);
bool gsCheckGameOverCondition(GameState*, Player*[], const GameRuleSet*);
void gsPassDealerButton(GameState*, const GameRuleSet*);

#endif // GAMESTATE_H
