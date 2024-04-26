#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>
#include <time.h>
#include "constants.h"
#include "dealer.h"
#include "gamerules.h"
#include "gamestate.h"
#include "handranking.h"
#include "messages.h"
#include "player.h"
#include "playerio.h"
#include "playingcard.h"
#include "utils.h"

int main()
{
    //  -- Loading messages --
    msgInitFromFile("loc.txt");

    //  --  Settings    --
    GameRuleSet globalRules;
    promptPlayerCount(&globalRules);
    promptAIPlayersCount(&globalRules);
    promptFundsPerPlayer(&globalRules);
    promptLimitFixed(&globalRules);
    promptBigBlind(&globalRules);

    //  --  Game setup   --
    //Seeds the random number generator with current time since epoch.
    //Should be called ONLY once.
    srand(time(NULL));
    PlayingCard deck[DECK_LENGTH];
    buildDeck(deck, false);
    PlayingCard* comm_cards[COMM_CARDS_COUNT];
    Player* players[globalRules.player_count];
    for (int i = 0; i < globalRules.player_count; ++i)
        players[i] = playerCreateNewWithFunds(globalRules.funds_per_player);
    for (int i = globalRules.ai_player_count; i < globalRules.player_count; ++i)
        players[i]->isHuman = true;

    //  --  Game loop   --
    GameState* globalState;
    bool gameOver = false;
    do {
        globalState = gsCreateNew(&globalRules);
        distributeCards(deck, &players, comm_cards, &globalRules);

        //Create a new table of tapout pot status records
        unsigned int tapout_pot_statuses[globalRules.player_count];
        for (int i = 0; i < globalRules.player_count; ++i)
            tapout_pot_statuses[i] = 0;

        //Play four betting rounds: pre-flop, flop, turn, river
        while (globalState->betting_round < 4){
            gsSetUpBettingRound(globalState, players, &globalRules);

            //  --  Single round of betting loop  --
            while (globalState->turns_left > 0){
                printf("---\n");
                printGameState(globalState);
                printCommunityCards(comm_cards, globalState->revealed_comm_cards);
                printHoleCards(players[globalState->current_player]);
                gsAdvancePlayerTurn(globalState, players, tapout_pot_statuses, &globalRules, NULL);
            }

            //If a betting round was suddenly ended by everyone but one player folding, get to pot payout right away
            if (globalState->all_but_one_folded)
                break;

            //Debug
            printf("\t--- END OF BETTING ROUND %d ---\n", globalState->betting_round);
            gsConcludeBettingRound(globalState);
        }

        //Time for showdown and deciding the winners of the pot
        int winners[globalRules.player_count];
        int winners_count = gsDetermineWinners(winners, &globalRules, globalState, players, comm_cards);
        printCommunityCards(comm_cards, globalState->revealed_comm_cards);
        printShowdownResults(winners, winners_count, players);
        gsAwardPot(globalState, players, tapout_pot_statuses, winners, winners_count);
        gameOver = gsCheckGameOverCondition(globalState, players, &globalRules);
        gsPassDealerButton(globalState, &globalRules);
    } while (!gameOver);

    //Post-game results
    //TODO

    return 0;
}
