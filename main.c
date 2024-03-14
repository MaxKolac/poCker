#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "constants.h"
#include "utils.h"
#include "playingcard.h"
#include "player.h"
#include "dealer.h"
#include "handranking.h"
#include "test/test_handranking.h"

int main()
{
    //Builds a full playing deck
    struct PlayingCard deck[DECK_LENGTH];
    buildDeck(&deck);

    //Uncomment this to run unit-tests
    test_handrankingPerform(deck);
    return 0;

    struct Player* players[PLAYER_COUNT];
    struct PlayingCard* comm_cards[COMM_CARDS_COUNT];

    //Seeds the random number generator with current time since epoch.
    //Should be called ONLY once.
    srand(time(NULL));

    //Game loop
    for (;;){
        //     --   Setup   --
        //User setting - initial funds per player
        int ini_funds_per_player = -1;
        do {
            printf("Set the initial amount of funds for each player. Min - %d, Max - %d: ", MIN_FUNDS_PER_PLAYER, MAX_FUNDS_PER_PLAYER);
            ini_funds_per_player = prompt(5);
        } while (ini_funds_per_player < MIN_FUNDS_PER_PLAYER || MAX_FUNDS_PER_PLAYER < ini_funds_per_player);
        //Debug
        //printf("Initial funds per player: %d", ini_funds_per_player);

        distributeCards(deck, players, comm_cards);
        //Debug
        //for (int i = 0; i < PLAYER_COUNT; i++){
        //    printf("Player %d's cards:\n", i);
        //    for (int j = 0; j < CARDS_PER_PLAYER; j++){
        //        printf("\t%s of %s\n", getPipName(players[i]->current_hand[j]->pips), getSuitName(players[i]->current_hand[j]->suit));
        //    }
        //}
        //printf("Community cards:\n");
        //for (int i = 0; i < COMM_CARDS_COUNT; i++){
        //    printf("\t%s of %s\n", getPipName(comm_cards[i]->pips), getSuitName(comm_cards[i]->suit));
        //}
    }

    return 0;
}
