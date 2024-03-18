#include "player.h"
//First time a source file needs an explicit reference to its own header
//Things will likely break in the future, I can smell it

/**
 *  \brief Resets player's scores to an array of zeros.
 */
void resetScores(struct Player _player){
    for (int i = 0; i < 10; i++){
        _player.scores[i] = 0;
    }
}
