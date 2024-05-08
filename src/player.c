#include "player.h"

#include <stdlib.h>

const int SCORE_TABLE_SIZE_OBJ = SCORE_TABLE_SIZE;

Player* playerCreateNew(){
    Player* self = ((Player*)malloc(sizeof(Player)));
    self->funds = 0;
    self->folded = false;
    self->tappedout = false;
    self->tappedout_funds = 0;
    resetScores(self);
    self->current_hand[0] = NULL;
    self->current_hand[1] = NULL;
    self->isHuman = false;
    self->past_decisions_size = 0;
    return self;
}

Player* playerCreateNewWithFunds(int initial_funds){
    Player* self = playerCreateNew();
    self->funds = initial_funds;
    return self;
}

/**
 *  \brief Resets player's scores to an array of zeros.
 *
 *  Remember that C does not have zero-initialization. The scores array WILL be filled with gibberish data.
 */
void resetScores(Player* self){
    for (int i = 0; i < SCORE_TABLE_SIZE; i++){
        self->scores[i] = 0;
    }
}
