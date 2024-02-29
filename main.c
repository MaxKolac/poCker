#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "playingcard.h"
#include "utils.h"

const int suits = 4;
const int pips_per_suit = 13;
const int deck_length = suits * pips_per_suit;
const int player_count = 4;
const int cards_per_player = 5;


int main()
{
    struct PlayingCard deck[deck_length];
    struct PlayingCard playerHands[player_count][cards_per_player];

    //Builds a full playing deck
    for (int i = 0; i < suits; i++){
        for (int j = 0; j < pips_per_suit; j++){
            deck[pips_per_suit * i + j].suit = i;
            deck[pips_per_suit * i + j].pips = j;
            //printf("%d\n", pips_per_suit * i + j);
        }
    }

    //for (int i = 0; i < deck_length; i++){
    //    printf("%s of %s\n", getPipName(deck[i].pips), getSuitName(deck[i].suit));
    //}

    //Game loop
    for (;;){
        //Setup

        //Get from the user how many funds should all players start with.
        char input[10];
        bool input_is_valid = false;
        int ini_funds_per_player;
        do {
            printf("Set the initial amount of funds for each player. Min - 100, Max - 10000: ");
            gets_s(&input, 10);
            if (isNumber(input)){
                int input_as_int = atoll(&input);
                input_is_valid = 100 <= input_as_int && input_as_int <= 10000;
                ini_funds_per_player = input_as_int;
            }
        } while (!input_is_valid);
        printf("Initial funds per player: %d", ini_funds_per_player);

    }

    return 0;
}
