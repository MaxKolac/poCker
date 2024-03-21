#include "player.h"
//First time a source file needs an explicit reference to its own header
//Things will likely break in the future, I can smell it

/**
 *  \brief TODO
 */
 int takeAction(struct Player *self){
    return 0;
 }


/**
 *  \brief Resets player's scores to an array of zeros.
 *  Remember that C does not have zero-initialization. The scores array WILL be filled with gibberish data.
 */
void resetScores(struct Player *self){
    for (int i = 0; i < 10; i++){
        self->scores[i] = 0;
    }
}
