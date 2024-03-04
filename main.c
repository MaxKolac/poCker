#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "constants.h"
#include "utils.h"
#include "playingcard.h"
#include "dealer.h"
#include "handranking.h"

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
    //Debug
    //for (int i = 0; i < deck_length; i++){
    //    printf("%s of %s\n", getPipName(deck[i].pips), getSuitName(deck[i].suit));
    //}

    struct Player* players[PLAYER_COUNT];
    struct PlayingCard* comm_cards[COMM_CARDS_COUNT];

    //Seeds the random number generator with current time since epoch.
    //Should be called ONLY once.
    srand(time(NULL));

    //Game loop
    for (;;){
        //Setup
        char input[10];
        bool input_is_valid = false;

        //Prompt the user for how many funds should all players start with.
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
        //Debug
        //printf("Initial funds per player: %d", ini_funds_per_player);

        distributeCards(deck, players, comm_cards);
        //Debug
        //for (int i = 0; i < PLAYER_COUNT; i++){
        //    printf("Player %d's cards:\n", i);
        //    for (int j = 0; j < CARDS_PER_PLAYER; j++){
        //        printf("\t%s of %s\n", getPipName(player_hands[i][j]->pips), getSuitName(player_hands[i][j]->suit));
        //    }
        //}
        //printf("Community cards:\n");
        //for (int i = 0; i < COMM_CARDS_COUNT; i++){
        //    printf("\t%s of %s\n", getPipName(comm_cards[i]->pips), getSuitName(comm_cards[i]->suit));
        //}
    }

    return 0;
}
