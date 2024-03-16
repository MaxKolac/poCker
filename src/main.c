#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>
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
    //test_handrankingPerform();
    //return 0;

    //  --  Settings    --
    //Initial funds per player
    //Amount of funds each player gets at the beginning of the game.
    int ini_funds_per_player = -1;
    do {
        char msg[128];
        snprintf(msg,
                 sizeof(msg),
                 "Set the initial amount of funds for each player. Min - %d, Max - %d",
                 MIN_FUNDS_PER_PLAYER,
                 MAX_FUNDS_PER_PLAYER);
        ini_funds_per_player = prompt_i(6, msg);
    } while (ini_funds_per_player < MIN_FUNDS_PER_PLAYER || MAX_FUNDS_PER_PLAYER < ini_funds_per_player);
    //Debug
    printf("Initial funds per player: %d\n", ini_funds_per_player);

    //Fixed-limit or no-limit
    //No-limit game means that the maximum amount of the bet isn't set, meaning players can bet as much as all of their funds.
    //There is only the minimum amount of bet, equaling the big blind amount.
    //Fixed-limit game restricts players to two fixes bet amounts, small limit which equals the big blind, and high limit
    //which equals two times the big blind.
    bool limit_fixed = prompt_b("Should the betting limits be fixed?");
    //Debug
    printf("Limits are fixed: %s\n", limit_fixed ? "true" : "false");

    //Big blind amount
    //This amount will influence the minimum bet amount and the pot's initial amount.
    //Small blind amount is automatically set to be half of it, rounded down.
    //Player first to the dealer has to pay the small blind, and the next of him has to pay the big blind.
    //Action starts on the third player and (assuming no raises) ends on the big blind player.
    int big_blind = -1;
    do {
        big_blind = prompt_i(6, "Set the big blind amount. Minimum is 2, maximum is 10% of funds per player");
    } while (big_blind < 2 || floorf(ini_funds_per_player * 0.1) < big_blind);
    int small_blind = floorf(big_blind / 2);
    //Debug
    printf("Big blind: %d, Small blind: %d\n", big_blind, small_blind);

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
    int dealer_player = 0;
    int s_blind_player = 1;
    int b_blind_player = 2;
    assert(PLAYER_COUNT >= 3);
    do {
        int current_player = 0;
        int revealed_community_cards = 0;
        unsigned int pot = 0;

        distributeCards(deck, players, comm_cards);

        //Play four betting rounds: pre-flop, flop, turn, river
        for (int betting_round = 0; betting_round < 4; betting_round++){
            unsigned int bet = 0;
            //In each round, small blind is the first to act
            current_player = (dealer_player + 1) % PLAYER_COUNT;

            //If it's the pre-flop round, force blind players to bet in, without affecting the turns variable
            if (betting_round == 0){
                players[s_blind_player]->funds -= small_blind;
                pot -= small_blind;
                players[b_blind_player]->funds -= big_blind;
                pot += big_blind;
                bet = big_blind;
                current_player = (b_blind_player + 1) % PLAYER_COUNT;
            }

            //  --  Single round of betting loop  --
            for (int turns = PLAYER_COUNT; turns > 0; turns--){ //This condition is shady, this loop will likely be changed for something else
                //Player chooses an action based on the cards

                //Consequence of their action
                //If someone raises ???

                //Move onto the next player who hasn't folded
                do {
                    current_player = (current_player + 1) % PLAYER_COUNT;
                } while (players[current_player]->folded);
            }

            //Reveal community cards
            switch (betting_round){
            case 0:
                revealed_community_cards = 3;
                break;
            case 1:
            case 2:
                revealed_community_cards++;
                break;
            }
        }

        //Compare cards, decide the winner(s) and split the pot
        for (int i = 0; i < PLAYER_COUNT; i++){
            scorePlayersHand(players[i], comm_cards, revealed_community_cards);
        }

        //Check if everyone but one player has money left - gameOver condition

        //Pass the dealer button to the next player
        //This also causes the blind player status to move
        dealer_player = (dealer_player + 1) % PLAYER_COUNT;
        s_blind_player = (s_blind_player + 1) % PLAYER_COUNT;
        b_blind_player = (b_blind_player + 1) % PLAYER_COUNT;
    } while (gameOver);
    return 0;
}
