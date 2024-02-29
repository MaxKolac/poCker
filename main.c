#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "constants.h"
#include "utils.h"
#include "playingcard.h"
#include "dealer.h"
//Note to self - #include declarations are basically copying those file contents here. Their order matters!

int main()
{
    //Builds a full playing deck
    struct PlayingCard deck[DECK_LENGTH];
    for (int i = 0; i < SUITS_COUNT; i++){
        for (int j = 0; j < PIPS_PER_SUIT; j++){
            deck[PIPS_PER_SUIT * i + j].suit = i;
            deck[PIPS_PER_SUIT * i + j].pips = j;
            //printf("%d\n", PIPS_PER_SUIT * i + j);
        }
    }
    //for (int i = 0; i < deck_length; i++){
    //    printf("%s of %s\n", getPipName(deck[i].pips), getSuitName(deck[i].suit));
    //}

    struct PlayingCard* player_hands[PLAYER_COUNT][CARDS_PER_PLAYER];
    struct PlayingCard* comm_cards[COMM_CARDS_COUNT];

    //random gen initialization, should be called only once
    srand(time(NULL));

    //Game loop
    for (;;){
        //Setup

        //Get from the user how many funds should all players start with.
        char input[10];
        bool input_is_valid = false;
        int ini_funds_per_player;
        do {
            printf("Set the initial amount of funds for each player. Min - %d, Max - %d: ", MIN_FUNDS_PER_PLAYER, MAX_FUNDS_PER_PLAYER);
            gets_s(&input, 10);
            if (isNumber(input)){
                int input_as_int = atoll(&input);
                input_is_valid = MIN_FUNDS_PER_PLAYER <= input_as_int && input_as_int <= MAX_FUNDS_PER_PLAYER;
                ini_funds_per_player = input_as_int;
            }
        } while (!input_is_valid);
        //printf("Initial funds per player: %d", ini_funds_per_player);

        distributeHoleCards(deck, player_hands, comm_cards);
    }

    return 0;
}
