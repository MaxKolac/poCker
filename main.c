#include <stdio.h>
#include <stdlib.h>
#include "playingcard.h"

const int suits = 4;
const int pips_per_suit = 13;
const int deck_length = suits * pips_per_suit;

int main()
{
    struct PlayingCard deck[deck_length];

    for (int i = 0; i < suits; i++){
        for (int j = 0; j < pips_per_suit; j++){
            deck[pips_per_suit * i + j].suit = i;
            deck[pips_per_suit * i + j].pips = j;
            printf("%d\n", pips_per_suit * i + j);
        }
    }

    for (int i = 0; i < deck_length; i++){
        printf("%s of %s\n", getPipName(deck[i].pips), getSuitName(deck[i].suit));
    }
    return 0;
}
