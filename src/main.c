#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>
#include <time.h>
#include "constants.h"
#include "dealer.h"
#include "player.h"
#include "playingcard.h"
#include "gamerules.h"
#include "handranking.h"
#include "utils.h"

int main()
{
    //  --  Settings    --
    struct GameRuleSet globalRules;
    promptFundsPerPlayer(&globalRules);
    promptLimitFixed(&globalRules);
    promptBigBlind(&globalRules);

    //  --  Game setup   --
    //Seeds the random number generator with current time since epoch.
    //Should be called ONLY once.
    srand(time(NULL));
    struct PlayingCard deck[DECK_LENGTH];
    struct PlayingCard* comm_cards[COMM_CARDS_COUNT];
    struct Player players[PLAYER_COUNT];
    buildDeck(deck, false);
    for (int i = 0; i < PLAYER_COUNT; i++){
        players[i].folded = false;
        players[i].funds = globalRules.funds_per_player;
        resetScores(&players[i]);
    }

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
        bool all_but_one_folded = false;

        distributeCards(deck, players, comm_cards);

        //Play four betting rounds: pre-flop, flop, turn, river
        for (int betting_round = 0; betting_round < 4; betting_round++){
            unsigned int bet = 0;
            //In each round, small blind (player to the left of dealer) is the first to act
            current_player = s_blind_player;

            //If it's the pre-flop round, force blind players to bet in, without affecting the turns variable
            if (betting_round == 0){
                players[s_blind_player].funds -= globalRules.small_blind;
                pot += globalRules.small_blind;
                players[b_blind_player].funds -= globalRules.big_blind;
                pot += globalRules.big_blind;
                bet = globalRules.big_blind;
                current_player = (b_blind_player + 1) % PLAYER_COUNT;
            }

            //  --  Single round of betting loop  --
            for (int turns = PLAYER_COUNT - 1; turns > 0; turns--){
                //This player has folded, skip his turn and move onto the next
                if (players[current_player].folded){
                    current_player = (current_player + 1) % PLAYER_COUNT;
                    continue;
                }

                //Player chooses an action and check if they even can do that
                //Validity checks should not be done by players themselves (they might cheat lol)
                int player_decision;
                bool decisionValid = false;
                do {
                    player_decision = takeAction(&players[current_player]);
                    player_decision = mathClamp(player_decision, -1, players[current_player].funds);
                    decisionValid = checkPlayerDecisionValidity(&players[current_player],
                                                                &globalRules,
                                                                player_decision,
                                                                bet);
                } while(!decisionValid);

                //Consequence of player's actions
                // 0 < player_decision signifies a RAISE by player_decision amount
                // 0 == player_decision signifies a CALL/CHECK
                // 0 > player_decision signifies a FOLD
                if (0 <= player_decision){
                    players[current_player].funds -= player_decision;
                    pot += player_decision;

                    if (0 < player_decision){
                        bet = player_decision;
                        turns = PLAYER_COUNT - 1;
                    }
                }
                else {
                    players[current_player].folded = true;
                    //Have everyone but one person folded?
                    //This would indicate an auto-win of current pot for last unfolded player
                    int folded_players = 0;
                    for (int i = 0; i < PLAYER_COUNT; i++){
                        if (players[i].folded){
                            folded_players++;
                        }
                    }
                    all_but_one_folded = folded_players == PLAYER_COUNT - 1;
                }
                if (all_but_one_folded){
                    break;
                }

                //Move onto the next player
                current_player = (current_player + 1) % PLAYER_COUNT;
            }

            //If a betting round was suddenly ended by everyone but one player folding, get to pot payout right away
            if (all_but_one_folded){
                break;
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

        //Time for showdown and deciding the winners of the pot
        int winners[PLAYER_COUNT];
        int winners_count = 0;
        if (all_but_one_folded){
            //Find the player who did not fold, give them the whole pot
            for (int i = 0; i < PLAYER_COUNT; i++){
                if (!players[i].folded){
                    winners[0] = i;
                    winners_count = 1;
                    break;
                }
            }
        }
        else {
            //Compare cards and decide the winner(s)
            for (int i = 0; i < PLAYER_COUNT; i++){
                scorePlayersHand(players[i], comm_cards, revealed_community_cards);
            }
            winners_count = decideWinners(players, PLAYER_COUNT, winners);
        }

        //If we have a single winner, they take the whole pot;
        if (winners_count == 1){
            players[winners[0]].funds += pot;
        }
        //Otherwise, pay it to individual winners evenly.
        //In the event of a pot indivisible by winner's count, the remainder is paid to the player to the left of dealer.
        else {
            for (int i = 0; i < winners_count; i++){
                int amount = floorf(pot / winners_count);
                players[winners[i]].funds += amount;
                pot -= amount;
            }
            if (pot > 0){
                players[s_blind_player].funds += pot;
            }
        }

        //Check if everyone but one player has money left - gameOver condition
        //Also, unmark players as folded and mark broke players as folded right away
        int broke_players = 0;
        for (int i = 0; i < PLAYER_COUNT; i++){
            if (players[i].funds > 0){
                players[i].folded = false;
            }
            else {
                players[i].folded = true;
                broke_players++;
            }
        }
        if (broke_players == PLAYER_COUNT - 1){
            gameOver = true;
        }

        //Pass the dealer button to the next player
        //This also causes the blind player status to move
        dealer_player = (dealer_player + 1) % PLAYER_COUNT;
        s_blind_player = (s_blind_player + 1) % PLAYER_COUNT;
        b_blind_player = (b_blind_player + 1) % PLAYER_COUNT;
    } while (gameOver);

    //Post-game results
    //TODO

    return 0;
}
