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
    //Uncomment this to run unit-tests
    test_handrankingPerform();
    return 0;

    //  --  Settings    --
    //Initial funds per player
    int ini_funds_per_player = -1;
    do {
        printf("Set the initial amount of funds for each player. Min - %d, Max - %d: ", MIN_FUNDS_PER_PLAYER, MAX_FUNDS_PER_PLAYER);
        ini_funds_per_player = prompt(5);
    } while (ini_funds_per_player < MIN_FUNDS_PER_PLAYER || MAX_FUNDS_PER_PLAYER < ini_funds_per_player);
    //Debug
    //printf("Initial funds per player: %d", ini_funds_per_player);

    //  --  Game setup   --
    //Seeds the random number generator with current time since epoch.
    //Should be called ONLY once.
    srand(time(NULL));
    struct PlayingCard deck[DECK_LENGTH];
    struct PlayingCard* comm_cards[COMM_CARDS_COUNT];
    struct Player* players[PLAYER_COUNT];
    buildDeck(&deck, false);

    //  --  Game loop   --
    bool gameOver = false;
    do {
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

        //TODO: Should all player pay a mandatory fee upon a new round?

        //  --  Round loop  --
        int turns_limit = PLAYER_COUNT;
        int player_index = 0;
        unsigned int pot = 0;
        for (int turn = 0; turn < turns_limit; turn++){
            //Player chooses an action based on the cards

            //Consequence of their action
            //If someone raises, raise turns_limit

            player_index = (player_index + 1) % PLAYER_COUNT;
        }

        //Compare cards

        //Decide the winner(s)

        //Split the pot

        //Check if everyone but one player has money left - gameOver condition
    } while (gameOver);
    return 0;
}
