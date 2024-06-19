#include "player.h"

#include <stdlib.h>
#include "ai.h"

/**
 *  \brief Creates a new Player struct.
 *  \return A pointer to it.
 */
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
    self->setStrategy = NULL;
    self->fake_hand[0] = NULL;
    self->fake_hand[1] = NULL;
    return self;
}

/**
 *  \brief Creates a new Player struct instance and sets its funds value.
 *  \param initial_funds The amount of funds to be assigned.
 *  \returns A pointer to the created struct.
 *
 *  A rude remainder on why lack of polymorphism is a bit of a bummer.
 */
Player* playerCreateNewWithFunds(int initial_funds){
    Player* self = playerCreateNew();
    self->funds = initial_funds;
    return self;
}

/**
 *  \brief Resets player's scores to an array of zeros.
 *  \param self The Player struct whose table should be reset.
 *
 *  Remember that C does not have zero-initialization. The scores array WILL be filled with gibberish data.
 */
void resetScores(Player* self){
    for (int i = 0; i < SCORE_TABLE_SIZE; i++){
        self->scores[i] = 0;
    }
}
