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
#include "gamestate.h"
#include "handranking.h"
#include "utils.h"

int main()
{
    //  --  Settings    --
    GameRuleSet globalRules;
    promptPlayerCount(&globalRules);
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
    Player* players[globalRules->player_count];
    for (int i = 0; i < globalRules->player_count; i++){
        players[i] = playerCreateNew();
    }

    //  --  Game loop   --
    GameState* globalState;
    bool gameOver = false;
    do {
        globalState = gsCreateNew();
        distributeCards(deck, *players, comm_cards);

        //Play four betting rounds: pre-flop, flop, turn, river
        while (globalState->betting_round < 4){
            gsSetUpBettingRound(globalState, players, &globalRules);

            //  --  Single round of betting loop  --
            while (globalState->turns_left > 0){
                gsAdvancePlayerTurn(globalState, players);
            }

            //If a betting round was suddenly ended by everyone but one player folding, get to pot payout right away
            if (globalState->all_but_one_folded){
                break;
            }
            gsConcludeBettingRound(globalState);
        }

        //Time for showdown and deciding the winners of the pot
        gsPerformShowdown(globalState, players, comm_cards);
        gameOver = gsCheckGameOverCondition(globalState, players);
        gsPassDealerButton(globalState);
    } while (gameOver);

    //Post-game results
    //TODO

    return 0;
}
