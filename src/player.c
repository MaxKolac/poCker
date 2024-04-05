#include "player.h"

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
