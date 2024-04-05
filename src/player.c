#include "player.h"

#include <stdlib.h>

Player* PlayerCreateNew(){
    Player* self = ((Player*)malloc(sizeof(Player)));
    self->funds = 0;
    self->folded = false;
    resetScores(self);
    self->isHuman = false;
    return self;
}

/**
 *  \brief TODO
 */
 int takeAction(Player *self){
    return 0;
 }


/**
 *  \brief Resets player's scores to an array of zeros.
 *  Remember that C does not have zero-initialization. The scores array WILL be filled with gibberish data.
 */
void resetScores(Player *self){
    for (int i = 0; i < 10; i++){
        self->scores[i] = 0;
    }
}
