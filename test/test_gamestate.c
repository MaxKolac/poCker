#include "CuTest.h"
#include "../src/gamestate.h"
#include "../src/playingcard.h"

static void test_checkConstructor(CuTest* ct){
    const GameRuleSet ruleSet = {
        .player_count = 6,
        .funds_per_player = 1200,
        .limit_fixed = false,
        .big_blind = 40,
        .small_blind = 20
    };

    GameState* state = gsCreateNew(&ruleSet);

    CuAssert(ct, "", state->revealed_comm_cards == 0);
    CuAssert(ct, "", state->dealer_player == 0);
    CuAssert(ct, "", state->s_blind_player == 1);
    CuAssert(ct, "", state->b_blind_player == 2);
    CuAssert(ct, "", state->current_player == 0);
    CuAssert(ct, "", state->turns_left == ruleSet.player_count - 1);
    CuAssert(ct, "", state->pot == 0);
    CuAssert(ct, "", state->bet == 0);
    CuAssert(ct, "", state->raises_performed == 0);
    CuAssert(ct, "", !(state->all_but_one_folded));
}

//  --  Advancing player turns  --

static void test_raiseRightAfterStartingGame(CuTest* ct){
    const GameRuleSet ruleSet = {
        .player_count = 8,
        .funds_per_player = 2500,
        .limit_fixed = false,
        .big_blind = 20,
        .small_blind = 10
    };
    GameState* state = gsCreateNew(&ruleSet);
    Player* players[8];
    for (int i = 0; i < 8; ++i)
        players[i] = playerCreateNewWithFunds(ruleSet.funds_per_player);

    gsSetUpBettingRound(state, players, &ruleSet);
    int decision = 25;
    gsAdvancePlayerTurn(state, players, &ruleSet, &decision);

    CuAssert(ct, "", state->revealed_comm_cards == 0);
    CuAssert(ct, "", state->dealer_player == 0);
    CuAssert(ct, "", state->s_blind_player == 1);
    CuAssert(ct, "", state->b_blind_player == 2);
    CuAssert(ct, "", state->current_player == 4);
    CuAssert(ct, "", state->betting_round == 0);
    CuAssert(ct, "", state->turns_left == 7);
    CuAssert(ct, "", state->raises_performed == 1);
    CuAssert(ct, "", !(state->all_but_one_folded));
    CuAssert(ct, "", state->pot == 10 + 20 + 25);
    CuAssert(ct, "", state->bet == 25);

    CuAssert(ct, "", players[state->dealer_player]->funds == 2500);
    CuAssert(ct, "", players[state->s_blind_player]->funds == 2500 - 10);
    CuAssert(ct, "", players[state->b_blind_player]->funds == 2500 - 20);
    CuAssert(ct, "", players[3]->funds == 2500 - 25);
}

static void test_advancePlayerTurnsThroughRoundWithNoRaises(CuTest* ct){
    const GameRuleSet ruleSet = {
        .player_count = 5,
        .funds_per_player = 4500,
        .limit_fixed = false,
        .big_blind = 30,
        .small_blind = 15
    };
    GameState* state = gsCreateNew(&ruleSet);
    Player* players[5];
    for (int i = 0; i < 5; ++i)
        players[i] = playerCreateNewWithFunds(ruleSet.funds_per_player);

    gsSetUpBettingRound(state, players, &ruleSet);
    int decision = 0;
    gsAdvancePlayerTurn(state, players, &ruleSet, &decision);
    gsAdvancePlayerTurn(state, players, &ruleSet, &decision);
    gsAdvancePlayerTurn(state, players, &ruleSet, &decision);
    gsAdvancePlayerTurn(state, players, &ruleSet, &decision);

    CuAssert(ct, "", state->revealed_comm_cards == 0);
    CuAssert(ct, "", state->dealer_player == 0);
    CuAssert(ct, "", state->s_blind_player == 1);
    CuAssert(ct, "", state->b_blind_player == 2);
    CuAssert(ct, "", state->current_player == 2);
    CuAssert(ct, "", state->betting_round == 0);
    CuAssert(ct, "", state->turns_left == 0);
    CuAssert(ct, "", state->raises_performed == 0);
    CuAssert(ct, "", !(state->all_but_one_folded));
    CuAssert(ct, "", state->pot == (15 + 30) + 30 + 30 + 30 + 30);
    CuAssert(ct, "", state->bet == 30);

    CuAssert(ct, "", players[state->s_blind_player]->funds == 4500 - 15 - 30);
    CuAssert(ct, "", players[state->b_blind_player]->funds == 4500 - 30);
    CuAssert(ct, "", players[3]->funds == 4500 - 30);
    CuAssert(ct, "", players[4]->funds == 4500 - 30);
    CuAssert(ct, "", players[state->dealer_player]->funds == 4500 - 30);
}

static void test_advancePlayerTurnsThroughRoundWithSomeLowFixedRaises(CuTest* ct){
    const GameRuleSet ruleSet = {
        .player_count = 5,
        .funds_per_player = 5000,
        .limit_fixed = true,
        .big_blind = 20,
        .small_blind = 10
    };
    GameState* state = gsCreateNew(&ruleSet);
    Player* players[ruleSet.player_count];
    for (int i = 0; i < ruleSet.player_count; ++i)
        players[i] = playerCreateNewWithFunds(ruleSet.funds_per_player);

    //dealer is 0, small b is 1, big b is 2
    gsSetUpBettingRound(state, players, &ruleSet);
    CuAssert(ct, "", state->pot == 30);
    CuAssert(ct, "", state->bet == 20);

    //player[3]
    int decision = 0;
    gsAdvancePlayerTurn(state, players, &ruleSet, &decision);
    CuAssert(ct, "", state->pot == 50);
    CuAssert(ct, "", state->bet == 20);

    //player[4]
    decision = 1;
    gsAdvancePlayerTurn(state, players, &ruleSet, &decision);
    CuAssert(ct, "", state->pot == 80);
    CuAssert(ct, "", state->bet == 30);
    CuAssert(ct, "", state->raises_performed == 1);
    CuAssert(ct, "", state->turns_left == 4);

    //player[0]
    decision = 0;
    gsAdvancePlayerTurn(state, players, &ruleSet, &decision);
    CuAssert(ct, "", state->pot == 110);
    CuAssert(ct, "", state->bet == 30);
    CuAssert(ct, "", state->turns_left == 3);

    //player[1]
    decision = 50;
    gsAdvancePlayerTurn(state, players, &ruleSet, &decision);
    CuAssert(ct, "", state->pot == 150);
    CuAssert(ct, "", state->bet == 40);
    CuAssert(ct, "", state->raises_performed == 2);
    CuAssert(ct, "", state->turns_left == 4);

    //player[2]
    decision = 0;
    gsAdvancePlayerTurn(state, players, &ruleSet, &decision);
    CuAssert(ct, "", state->pot == 190);
    CuAssert(ct, "", state->bet == 40);
    CuAssert(ct, "", state->turns_left == 3);

    //player[3]
    decision = 123;
    gsAdvancePlayerTurn(state, players, &ruleSet, &decision);
    CuAssert(ct, "", state->pot == 240);
    CuAssert(ct, "", state->bet == 50);
    CuAssert(ct, "", state->turns_left == 4);

    //player[4]
    decision = 0;
    gsAdvancePlayerTurn(state, players, &ruleSet, &decision);
    CuAssert(ct, "", state->pot == 290);
    CuAssert(ct, "", state->bet == 50);
    CuAssert(ct, "", state->turns_left == 3);

    //player[0]
    decision = 0;
    gsAdvancePlayerTurn(state, players, &ruleSet, &decision);
    CuAssert(ct, "", state->pot == 340);
    CuAssert(ct, "", state->bet == 50);
    CuAssert(ct, "", state->turns_left == 2);

    //player[1]
    decision = 0;
    gsAdvancePlayerTurn(state, players, &ruleSet, &decision);
    CuAssert(ct, "", state->pot == 390);
    CuAssert(ct, "", state->bet == 50);
    CuAssert(ct, "", state->turns_left == 1);

    //player[2]
    decision = 0;
    gsAdvancePlayerTurn(state, players, &ruleSet, &decision);
    CuAssert(ct, "", state->pot == 440);
    CuAssert(ct, "", state->bet == 50);
    CuAssert(ct, "", state->turns_left == 0);
    CuAssert(ct, "", state->current_player == 3);

    CuAssert(ct, "", players[state->s_blind_player]->funds == 5000 - 10 - 40 - 50);
    CuAssert(ct, "", players[state->b_blind_player]->funds == 5000 - 20 - 40 - 50);
    CuAssert(ct, "", players[3]->funds == 5000 - 20 - 50);
    CuAssert(ct, "", players[4]->funds == 5000 - 30 - 50);
    CuAssert(ct, "", players[state->dealer_player]->funds == 5000 - 30 - 50);
}

static void test_advancePlayerTurnsThroughRoundWithSomeHighFixedRaises(CuTest* ct){
    const GameRuleSet ruleSet = {
        .player_count = 6,
        .funds_per_player = 10000,
        .limit_fixed = true,
        .big_blind = 30,
        .small_blind = 15
    };
    GameState* state = gsCreateNew(&ruleSet);
    state->betting_round = 2;
    Player* players[ruleSet.player_count];
    for (int i = 0; i < ruleSet.player_count; ++i)
        players[i] = playerCreateNewWithFunds(ruleSet.funds_per_player);

    //dealer is 0, small b is 1, big b is 2
    gsSetUpBettingRound(state, players, &ruleSet);
    CuAssert(ct, "", state->pot == 0);
    CuAssert(ct, "", state->bet == 0);

    //player[1]
    int decision = 4;
    gsAdvancePlayerTurn(state, players, &ruleSet, &decision);
    CuAssert(ct, "", state->pot == 30);
    CuAssert(ct, "", state->bet == 30);
    CuAssert(ct, "", state->raises_performed == 1);
    CuAssert(ct, "", state->turns_left == 5);

    //player[2]
    decision = 1;
    gsAdvancePlayerTurn(state, players, &ruleSet, &decision);
    CuAssert(ct, "", state->pot == 90);
    CuAssert(ct, "", state->bet == 60);
    CuAssert(ct, "", state->raises_performed == 2);
    CuAssert(ct, "", state->turns_left == 5);

    //player[3]
    decision = 0;
    gsAdvancePlayerTurn(state, players, &ruleSet, &decision);
    CuAssert(ct, "", state->pot == 150);
    CuAssert(ct, "", state->bet == 60);
    CuAssert(ct, "", state->turns_left == 4);

    //player[4]
    decision = 5000;
    gsAdvancePlayerTurn(state, players, &ruleSet, &decision);
    CuAssert(ct, "", state->pot == 240);
    CuAssert(ct, "", state->bet == 90);
    CuAssert(ct, "", state->raises_performed == 3);
    CuAssert(ct, "", state->turns_left == 5);

    //player[5]
    decision = 0;
    gsAdvancePlayerTurn(state, players, &ruleSet, &decision);
    CuAssert(ct, "", state->pot == 330);
    CuAssert(ct, "", state->bet == 90);
    CuAssert(ct, "", state->turns_left == 4);

    //player[0]
    decision = 0;
    gsAdvancePlayerTurn(state, players, &ruleSet, &decision);
    CuAssert(ct, "", state->pot == 420);
    CuAssert(ct, "", state->bet == 90);
    CuAssert(ct, "", state->turns_left == 3);

    //player[1]
    decision = 0;
    gsAdvancePlayerTurn(state, players, &ruleSet, &decision);
    CuAssert(ct, "", state->pot == 510);
    CuAssert(ct, "", state->bet == 90);
    CuAssert(ct, "", state->turns_left == 2);

    //player[2]
    decision = 0;
    gsAdvancePlayerTurn(state, players, &ruleSet, &decision);
    CuAssert(ct, "", state->pot == 600);
    CuAssert(ct, "", state->bet == 90);
    CuAssert(ct, "", state->turns_left == 1);//player[2]

    //player[3]
    decision = 0;
    gsAdvancePlayerTurn(state, players, &ruleSet, &decision);
    CuAssert(ct, "", state->pot == 690);
    CuAssert(ct, "", state->bet == 90);

    CuAssert(ct, "", state->turns_left == 0);
    CuAssert(ct, "", state->current_player == 4);

    CuAssert(ct, "", players[state->s_blind_player]->funds == 10000 - 30 - 90);
    CuAssert(ct, "", players[state->b_blind_player]->funds == 10000 - 60 - 90);
    CuAssert(ct, "", players[3]->funds == 10000 - 60 - 90);
    CuAssert(ct, "", players[4]->funds == 10000 - 90);
    CuAssert(ct, "", players[5]->funds == 10000 - 90);
    CuAssert(ct, "", players[state->dealer_player]->funds == 10000 - 90);
}

static void test_advancePlayerTurnsThroughRoundWithSomeUnlimitedRaises(CuTest* ct){
    const GameRuleSet ruleSet = {
        .player_count = 6,
        .funds_per_player = 10000,
        .limit_fixed = false,
        .big_blind = 30,
        .small_blind = 15
    };
    GameState* state = gsCreateNew(&ruleSet);
    Player* players[ruleSet.player_count];
    for (int i = 0; i < ruleSet.player_count; ++i)
        players[i] = playerCreateNewWithFunds(ruleSet.funds_per_player);

    //dealer is 0, small b is 1, big b is 2
    gsSetUpBettingRound(state, players, &ruleSet);
    CuAssert(ct, "", state->pot == 45);
    CuAssert(ct, "", state->bet == 30);

    //player[3]
    int decision = 0;
    gsAdvancePlayerTurn(state, players, &ruleSet, &decision);
    CuAssert(ct, "", state->pot == 75);
    CuAssert(ct, "", state->bet == 30);

    //player[4]
    decision = 61;
    gsAdvancePlayerTurn(state, players, &ruleSet, &decision);
    CuAssert(ct, "", state->pot == 136);
    CuAssert(ct, "", state->bet == 61);
    CuAssert(ct, "", state->raises_performed == 1);
    CuAssert(ct, "", state->turns_left == 5);

    //player[5]
    decision = 0;
    gsAdvancePlayerTurn(state, players, &ruleSet, &decision);
    CuAssert(ct, "", state->pot == 197);
    CuAssert(ct, "", state->bet == 61);
    CuAssert(ct, "", state->turns_left == 4);

    //player[0]
    decision = 0;
    gsAdvancePlayerTurn(state, players, &ruleSet, &decision);
    CuAssert(ct, "", state->pot == 258);
    CuAssert(ct, "", state->bet == 61);
    CuAssert(ct, "", state->turns_left == 3);

    //player[1]
    decision = 62;
    gsAdvancePlayerTurn(state, players, &ruleSet, &decision);
    CuAssert(ct, "", state->pot == 320);
    CuAssert(ct, "", state->bet == 62);
    CuAssert(ct, "", state->turns_left == 5);
    CuAssert(ct, "", state->raises_performed == 2);

    //player[2]
    decision = 0;
    gsAdvancePlayerTurn(state, players, &ruleSet, &decision);
    CuAssert(ct, "", state->pot == 382);
    CuAssert(ct, "", state->bet == 62);
    CuAssert(ct, "", state->turns_left == 4);

    //player[3]
    decision = 0;
    gsAdvancePlayerTurn(state, players, &ruleSet, &decision);
    CuAssert(ct, "", state->pot == 444);
    CuAssert(ct, "", state->bet == 62);
    CuAssert(ct, "", state->turns_left == 3);

    //player[4]
    decision = 0;
    gsAdvancePlayerTurn(state, players, &ruleSet, &decision);
    CuAssert(ct, "", state->pot == 506);
    CuAssert(ct, "", state->bet == 62);
    CuAssert(ct, "", state->turns_left == 2);

    //player[5]
    decision = 0;
    gsAdvancePlayerTurn(state, players, &ruleSet, &decision);
    CuAssert(ct, "", state->pot == 568);
    CuAssert(ct, "", state->bet == 62);
    CuAssert(ct, "", state->turns_left == 1);

    //player[0]
    decision = 0;
    gsAdvancePlayerTurn(state, players, &ruleSet, &decision);
    CuAssert(ct, "", state->pot == 630);
    CuAssert(ct, "", state->bet == 62);

    CuAssert(ct, "", state->turns_left == 0);
    CuAssert(ct, "", state->current_player == 1);

    CuAssert(ct, "", players[state->s_blind_player]->funds == 10000 - 15 - 62);
    CuAssert(ct, "", players[state->b_blind_player]->funds == 10000 - 30 - 62);
    CuAssert(ct, "", players[3]->funds == 10000 - 30 - 62);
    CuAssert(ct, "", players[4]->funds == 10000 - 61 - 62);
    CuAssert(ct, "", players[5]->funds == 10000 - 61 - 62);
    CuAssert(ct, "", players[state->dealer_player]->funds == 10000 - 61 - 62);
}

static void test_advancePlayerTurnsThroughRoundWithOnlyRaises(CuTest* ct){
    const GameRuleSet ruleSet = {
        .player_count = 7,
        .funds_per_player = 5000,
        .limit_fixed = false,
        .big_blind = 24,
        .small_blind = 12
    };
    GameState* state = gsCreateNew(&ruleSet);
    Player* players[7];
    for (int i = 0; i < 7; ++i)
        players[i] = playerCreateNewWithFunds(ruleSet.funds_per_player);

    //dealer is 0, small b is 1, big b is 2
    gsSetUpBettingRound(state, players, &ruleSet);
    CuAssert(ct, "", state->pot == 36);
    CuAssert(ct, "", state->bet == 24);

    //player[3]
    int decision = 40;
    gsAdvancePlayerTurn(state, players, &ruleSet, &decision);
    CuAssert(ct, "", state->pot == 76);
    CuAssert(ct, "", state->bet == 40);
    CuAssert(ct, "", state->raises_performed == 1);

    //player[4]
    decision = 45;
    gsAdvancePlayerTurn(state, players, &ruleSet, &decision);
    CuAssert(ct, "", state->pot == 121);
    CuAssert(ct, "", state->bet == 45);
    CuAssert(ct, "", state->raises_performed == 2);

    //player[5]
    decision = 55;
    gsAdvancePlayerTurn(state, players, &ruleSet, &decision);
    CuAssert(ct, "", state->pot == 176);
    CuAssert(ct, "", state->bet == 55);
    CuAssert(ct, "", state->raises_performed == 3);

    //player[6]
    decision = 70;
    gsAdvancePlayerTurn(state, players, &ruleSet, &decision);
    CuAssert(ct, "", state->pot == 246);
    CuAssert(ct, "", state->bet == 70);
    CuAssert(ct, "", state->raises_performed == 4);

    //player[0]
    decision = 82;
    gsAdvancePlayerTurn(state, players, &ruleSet, &decision);
    CuAssert(ct, "", state->pot == 328);
    CuAssert(ct, "", state->bet == 82);
    CuAssert(ct, "", state->raises_performed == 5);

    //player[1]
    decision = 90;
    gsAdvancePlayerTurn(state, players, &ruleSet, &decision);
    CuAssert(ct, "", state->pot == 418);
    CuAssert(ct, "", state->bet == 90);
    CuAssert(ct, "", state->raises_performed == 6);

    //player[2]
    decision = 100;
    gsAdvancePlayerTurn(state, players, &ruleSet, &decision);
    CuAssert(ct, "", state->pot == 518);
    CuAssert(ct, "", state->bet == 100);

    CuAssert(ct, "", state->current_player == 3);
    CuAssert(ct, "", state->turns_left == ruleSet.player_count - 1);
    CuAssert(ct, "", state->raises_performed == ruleSet.player_count);

    CuAssert(ct, "", players[state->s_blind_player]->funds == 5000 - 12 - 90);
    CuAssert(ct, "", players[state->b_blind_player]->funds == 5000 - 24 - 100);
    CuAssert(ct, "", players[3]->funds == 5000 - 40);
    CuAssert(ct, "", players[4]->funds == 5000 - 45);
    CuAssert(ct, "", players[5]->funds == 5000 - 55);
    CuAssert(ct, "", players[6]->funds == 5000 - 70);
    CuAssert(ct, "", players[state->dealer_player]->funds == 5000 - 82);
}

static void test_checkTheWhileLoopInMainForPlayerTurnAdvancing(CuTest* ct){
    //The purpose of this UT was to see if this loop from main.c works properly
    const GameRuleSet rules = {
        .player_count = 7,
        .funds_per_player = 10000,
        .limit_fixed = true,
        .big_blind = 20,
        .small_blind = 10
    };
    GameState* state = gsCreateNew(&rules);
    Player* players[rules.player_count];
    for (int i = 0; i < rules.player_count; ++i)
        players[i] = playerCreateNewWithFunds(rules.funds_per_player);

    gsSetUpBettingRound(state, players, &rules);
    while (state->turns_left > 0){
        gsAdvancePlayerTurn(state, players, &rules, 0);
    }

    //Current player should be the one after the last player who acted, therefore big_blind
    CuAssert(ct, "", state->current_player == state->b_blind_player);
    CuAssert(ct, "", state->turns_left == 0);
    CuAssert(ct, "", state->bet == 20);
    CuAssert(ct, "", state->pot == 10 + (20 * 7));
    gsConcludeBettingRound(state);

    gsSetUpBettingRound(state, players, &rules);
    while (state->turns_left > 0){
        gsAdvancePlayerTurn(state, players, &rules, 0);
    }

    //Current player should be the one after the last player who acted, therefore s_blind
    CuAssert(ct, "", state->current_player == state->s_blind_player);
    CuAssert(ct, "", state->turns_left == 0);
    CuAssert(ct, "", state->bet == 0);
    CuAssert(ct, "", state->pot == 10 + (20 * 7));
}

static void test_allButOneFoldedConditionCheckForAdvancePlayerTurn(CuTest* ct){
    const GameRuleSet rules = {
        .player_count = 6,
    };
    Player* players[rules.player_count];
    for (int i = 0; i < rules.player_count; ++i)
        players[i] = playerCreateNew();
    GameState* state = gsCreateNew(&rules);
    state->turns_left = 3;
    state->current_player = 2;

    players[0]->folded = true;
    players[1]->folded = true;
    players[4]->folded = true;
    players[5]->folded = true;

    CuAssert(ct, "", !(state->all_but_one_folded));

    int decision = -1;
    gsAdvancePlayerTurn(state, players, &rules, &decision);

    CuAssert(ct, "", state->all_but_one_folded);
}

//  --  Setting up betting rounds   --

static void test_settingUpPreFlopRound(CuTest* ct){
    const GameRuleSet ruleSet = {
        .player_count = 4,
        .funds_per_player = 10000,
        .limit_fixed = false,
        .big_blind = 10,
        .small_blind = 5
    };
    GameState* state = gsCreateNew(&ruleSet);
    Player* players[4];
    for (int i = 0; i < 4; ++i)
        players[i] = playerCreateNewWithFunds(ruleSet.funds_per_player);

    gsSetUpBettingRound(state, players, &ruleSet);

    CuAssert(ct, "", state->revealed_comm_cards == 0);
    CuAssert(ct, "", state->dealer_player == 0);
    CuAssert(ct, "", state->s_blind_player == 1);
    CuAssert(ct, "", state->b_blind_player == 2);
    CuAssert(ct, "", state->current_player == 3);
    CuAssert(ct, "", state->betting_round == 0);
    CuAssert(ct, "", state->turns_left == 3);
    CuAssert(ct, "", state->raises_performed == 0);
    CuAssert(ct, "", !(state->all_but_one_folded));
    CuAssert(ct, "", state->pot == 15);
    CuAssert(ct, "", state->bet == 10);

    CuAssert(ct, "", players[state->dealer_player]->funds == 10000);
    CuAssert(ct, "", players[state->s_blind_player]->funds == 9995);
    CuAssert(ct, "", players[state->b_blind_player]->funds == 9990);
}

static void test_settingUpFlopRound(CuTest* ct){
    const GameRuleSet ruleSet = {
        .player_count = 6,
        .funds_per_player = 5500,
        .limit_fixed = false,
        .big_blind = 20,
        .small_blind = 10
    };
    GameState* state = gsCreateNew(&ruleSet);
    Player* players[ruleSet.player_count];
    for (int i = 0; i < ruleSet.player_count; ++i)
        players[i] = playerCreateNewWithFunds(ruleSet.funds_per_player);

    gsSetUpBettingRound(state, players, &ruleSet);
    gsConcludeBettingRound(state);
    gsSetUpBettingRound(state, players, &ruleSet);

    CuAssert(ct, "", state->revealed_comm_cards == 3);
    CuAssert(ct, "", state->dealer_player == 0);
    CuAssert(ct, "", state->s_blind_player == 1);
    CuAssert(ct, "", state->b_blind_player == 2);
    CuAssert(ct, "", state->current_player == 1);
    CuAssert(ct, "", state->betting_round == 1);
    CuAssert(ct, "", state->turns_left == ruleSet.player_count);
    CuAssert(ct, "", state->raises_performed == 0);
    CuAssert(ct, "", !(state->all_but_one_folded));
    CuAssert(ct, "", state->pot == 30);
    CuAssert(ct, "", state->bet == 0);

    CuAssert(ct, "", players[state->dealer_player]->funds == 5500);
    CuAssert(ct, "", players[state->s_blind_player]->funds == 5500 - 10);
    CuAssert(ct, "", players[state->b_blind_player]->funds == 5500 - 20);
}

static void test_settingUpTurnRound(CuTest* ct){
    const GameRuleSet ruleSet = {
        .player_count = 10,
        .funds_per_player = 12000,
        .limit_fixed = false,
        .big_blind = 50,
        .small_blind = 25
    };
    GameState* state = gsCreateNew(&ruleSet);
    Player* players[ruleSet.player_count];
    for (int i = 0; i < ruleSet.player_count; ++i)
        players[i] = playerCreateNewWithFunds(ruleSet.funds_per_player);

    gsSetUpBettingRound(state, players, &ruleSet);
    gsConcludeBettingRound(state);
    gsSetUpBettingRound(state, players, &ruleSet);
    gsConcludeBettingRound(state);
    gsSetUpBettingRound(state, players, &ruleSet);

    CuAssert(ct, "", state->revealed_comm_cards == 4);
    CuAssert(ct, "", state->dealer_player == 0);
    CuAssert(ct, "", state->s_blind_player == 1);
    CuAssert(ct, "", state->b_blind_player == 2);
    CuAssert(ct, "", state->current_player == 1);
    CuAssert(ct, "", state->betting_round == 2);
    CuAssert(ct, "", state->turns_left == ruleSet.player_count);
    CuAssert(ct, "", state->raises_performed == 0);
    CuAssert(ct, "", !(state->all_but_one_folded));
    CuAssert(ct, "", state->pot == 75);
    CuAssert(ct, "", state->bet == 0);

    CuAssert(ct, "", players[state->dealer_player]->funds == 12000);
    CuAssert(ct, "", players[state->s_blind_player]->funds == 12000 - 25);
    CuAssert(ct, "", players[state->b_blind_player]->funds == 12000 - 50);
}

static void test_settingUpRiverRound(CuTest* ct){
    const GameRuleSet ruleSet = {
        .player_count = 4,
        .funds_per_player = 1200,
        .limit_fixed = false,
        .big_blind = 10,
        .small_blind = 5
    };
    GameState* state = gsCreateNew(&ruleSet);
    Player* players[ruleSet.player_count];
    for (int i = 0; i < ruleSet.player_count; ++i)
        players[i] = playerCreateNewWithFunds(ruleSet.funds_per_player);

    gsSetUpBettingRound(state, players, &ruleSet);
    gsConcludeBettingRound(state);
    gsSetUpBettingRound(state, players, &ruleSet);
    gsConcludeBettingRound(state);
    gsSetUpBettingRound(state, players, &ruleSet);
    gsConcludeBettingRound(state);
    gsSetUpBettingRound(state, players, &ruleSet);

    CuAssert(ct, "", state->revealed_comm_cards == 5);
    CuAssert(ct, "", state->dealer_player == 0);
    CuAssert(ct, "", state->s_blind_player == 1);
    CuAssert(ct, "", state->b_blind_player == 2);
    CuAssert(ct, "", state->current_player == 1);
    CuAssert(ct, "", state->betting_round == 3);
    CuAssert(ct, "", state->turns_left == ruleSet.player_count);
    CuAssert(ct, "", state->raises_performed == 0);
    CuAssert(ct, "", !(state->all_but_one_folded));
    CuAssert(ct, "", state->pot == 15);
    CuAssert(ct, "", state->bet == 0);

    CuAssert(ct, "", players[state->dealer_player]->funds == 1200);
    CuAssert(ct, "", players[state->s_blind_player]->funds == 1200 - 5);
    CuAssert(ct, "", players[state->b_blind_player]->funds == 1200 - 10);
}

static void test_settingUpPreFlopRoundWhereSomePlayersCantAffordForcedBets(CuTest* ct){
    const GameRuleSet ruleSet = {
        .player_count = 4,
        .funds_per_player = 1200,
        .limit_fixed = false,
        .big_blind = 10,
        .small_blind = 5
    };
    GameState* state = gsCreateNew(&ruleSet);
    Player* players[ruleSet.player_count];
    for (int i = 0; i < ruleSet.player_count; ++i)
        players[i] = playerCreateNewWithFunds(ruleSet.funds_per_player);
    players[state->s_blind_player]->funds = ruleSet.small_blind - 1;
    players[state->b_blind_player]->funds = ruleSet.big_blind - 1;

    gsSetUpBettingRound(state, players, &ruleSet);

    //These players will not be tapped out
    //Doing so would only allow them to win as much as they bet
    //Placing them in a lose-lose situation
    //Instead, they will go on without funds until its their time to act
    //Other player would have thrown some more funds into the pot
    //Only then, the player can be tapped-out, giving them a chance at winning more than they were forced to bet
    CuAssert(ct, "", players[state->s_blind_player]->funds == 0);
    CuAssert(ct, "", players[state->s_blind_player]->tappedout == false);
    CuAssert(ct, "", players[state->b_blind_player]->funds == 0);
    CuAssert(ct, "", players[state->b_blind_player]->tappedout == false);

    int zero = 0;
    int minustwo = -2;
    gsAdvancePlayerTurn(state, players, &ruleSet, &zero);
    gsAdvancePlayerTurn(state, players, &ruleSet, &zero);
    gsAdvancePlayerTurn(state, players, &ruleSet, &minustwo);
    gsConcludeBettingRound(state);
    gsSetUpBettingRound(state, players, &ruleSet);
    gsAdvancePlayerTurn(state, players, &ruleSet, &zero);
    gsAdvancePlayerTurn(state, players, &ruleSet, &minustwo);

    //pot should be: (4 + 9) + 10 + 10 + SB' Act || End of round || 0 + BB' Act
    CuAssert(ct, "", players[state->s_blind_player]->tappedout == true);
    CuAssert(ct, "", players[state->s_blind_player]->tappedout_funds == 33);
    CuAssert(ct, "", players[state->b_blind_player]->tappedout == true);
    CuAssert(ct, "", players[state->b_blind_player]->tappedout_funds == 33);
}

static void test_advancingToNextBettingRoundResetsTurnsLeftProperly(CuTest* ct){
    const GameRuleSet rules = {
        .player_count = 4,
        .funds_per_player = 10000
    };
    const GameState* state = gsCreateNew(&rules);
    Player* players[rules.player_count];
    for (int i = 0; i < rules.player_count; ++i)
        players[i] = playerCreateNewWithFunds(rules.funds_per_player);

    int decision = 0;
    //preflop
    gsSetUpBettingRound(state, players, &rules);
    CuAssert(ct, "", state->turns_left == rules.player_count - 1);
    gsAdvancePlayerTurn(state, players, &rules, &decision);
    gsConcludeBettingRound(state);
    CuAssert(ct, "", state->turns_left == rules.player_count - 2);

    //flop, turn, river
    for (int i = 0; i < 3; i++){
        gsSetUpBettingRound(state, players, &rules);
        CuAssert(ct, "", state->turns_left == rules.player_count);
        gsAdvancePlayerTurn(state, players, &rules, &decision);
        gsConcludeBettingRound(state);
        CuAssert(ct, "", state->turns_left == rules.player_count - 1);
    }
}

static void test_concludingSingleGame(CuTest* ct){
    const GameRuleSet rules = {
        .player_count = 6,
        .funds_per_player = 10000
    };
    const GameState* state = gsCreateNew(&rules);
    Player* players[rules.player_count];
    for (int i = 0; i < rules.player_count; ++i)
        players[i] = playerCreateNewWithFunds(rules.funds_per_player);

    for (int i = 0; i < MAX_ROUNDS_PER_GAME; ++i){
        gsSetUpBettingRound(state, players, &rules);
        gsConcludeBettingRound(state);
    }
    gsPassDealerButton(state, &rules);
    gsConcludeSingleGame(state);
    const GameState* freshState = gsCreateNew(&rules);

    CuAssert(ct, "", state->revealed_comm_cards == freshState->revealed_comm_cards);
    CuAssert(ct, "", state->dealer_player == freshState->dealer_player + 1);
    CuAssert(ct, "", state->s_blind_player == freshState->s_blind_player + 1);
    CuAssert(ct, "", state->b_blind_player == freshState->b_blind_player + 1);
    CuAssert(ct, "", state->betting_round == freshState->betting_round);
    CuAssert(ct, "", state->all_but_one_folded == freshState->all_but_one_folded);
}

//  --  Tap-outs   --

static void test_singleTapOut(CuTest* ct){
    GameRuleSet rules = {
        .player_count = 4,
        .funds_per_player = 1000,
        .limit_fixed = true,
        .big_blind = 50,
        .small_blind = 25,
    };
    GameState* state = gsCreateNew(&rules);
    state->revealed_comm_cards = 4;
    state->current_player = 1;
    state->betting_round = 2;
    state->raises_performed = MAX_BETS_PER_ROUND;
    state->turns_left = 2;
    state->pot = 500;
    state->bet = 150;

    Player* players[rules.player_count];
    for (int i = 0; i < rules.player_count; ++i)
        players[i] = playerCreateNewWithFunds(rules.funds_per_player);
    players[state->current_player]->funds = 140;

    int decision = -2;
    gsAdvancePlayerTurn(state, players, &rules, &decision);

    CuAssert(ct, "", players[1]->funds == 0);
    CuAssert(ct, "", players[1]->tappedout);
    CuAssert(ct, "", players[1]->tappedout_funds == 500 + 140);
    CuAssert(ct, "", state->pot == 500 + 140);
    CuAssert(ct, "", state->bet == 150);
}

static void test_multipleTapouts(CuTest* ct){
    GameRuleSet rules = {
        .player_count = 6,
        .funds_per_player = 25000,
        .limit_fixed = false,
        .big_blind = 20,
        .small_blind = 10,
    };
    GameState* state = gsCreateNew(&rules);
    state->revealed_comm_cards = 3;
    state->current_player = 0;
    state->betting_round = 1;
    state->raises_performed = MAX_BETS_PER_ROUND;
    state->turns_left = 4;
    state->pot = 210;
    state->bet = 80;

    Player* players[rules.player_count];
    for (int i = 0; i < rules.player_count; ++i)
        players[i] = playerCreateNewWithFunds(rules.funds_per_player);
    players[0]->funds = 70;
    players[1]->funds = 45;
    players[2]->funds = 10;

    int decision = -2;
    gsAdvancePlayerTurn(state, players, &rules, &decision);
    gsAdvancePlayerTurn(state, players, &rules, &decision);
    gsAdvancePlayerTurn(state, players, &rules, &decision);

    for (int i = 0; i < 3; ++i){
        CuAssert(ct, "", players[i]->funds == 0);
        CuAssert(ct, "", players[i]->tappedout);
    }
    CuAssert(ct, "", players[0]->tappedout_funds == 280);
    CuAssert(ct, "", players[1]->tappedout_funds == 325);
    CuAssert(ct, "", players[2]->tappedout_funds == 335);
    CuAssert(ct, "", state->pot == 335);
    CuAssert(ct, "", state->bet == 80);
}

//  --  Performing showdowns    --

static void test_standardShowdownWithSingleWinner(CuTest* ct){
    GameRuleSet ruleset = {
        .player_count = 7,
        .funds_per_player = 1200,
        .limit_fixed = true,
        .big_blind = 20,
        .small_blind = 10
    };
    Player* players[ruleset.player_count];
    for (int i = 0; i < ruleset.player_count; ++i)
        players[i] = playerCreateNewWithFunds(ruleset.funds_per_player);
    GameState* state = gsCreateNew(&ruleset);
    state->revealed_comm_cards = 5;
    state->betting_round = 4;
    state->all_but_one_folded = false;
    state->pot = 200;

    PlayingCard* deck[DECK_LENGTH];
    buildDeck(&deck, false);
    const PlayingCard* comm_cards[] = {
        &deck[PIPS_PER_SUIT * DIAMONDS + KING - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + QUEEN - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + JACK - 1],
        &deck[PIPS_PER_SUIT * CLUBS + TWO - 1],
        &deck[PIPS_PER_SUIT * HEARTS + THREE - 1]
    };
    //Winner
    players[0]->current_hand[0] = &deck[PIPS_PER_SUIT * DIAMONDS + TEN - 1];
    players[0]->current_hand[1] = &deck[PIPS_PER_SUIT * DIAMONDS + ACE - 1];

    players[1]->current_hand[0] = &deck[PIPS_PER_SUIT * CLUBS + TEN - 1];
    players[1]->current_hand[1] = &deck[PIPS_PER_SUIT * HEARTS + THREE - 1];

    players[2]->current_hand[0] = &deck[PIPS_PER_SUIT * SPADES + KING - 1];
    players[2]->current_hand[1] = &deck[PIPS_PER_SUIT * CLUBS + KING - 1];

    players[3]->current_hand[0] = &deck[PIPS_PER_SUIT * HEARTS + FIVE - 1];
    players[3]->current_hand[1] = &deck[PIPS_PER_SUIT * DIAMONDS + EIGHT - 1];

    players[4]->current_hand[0] = &deck[PIPS_PER_SUIT * SPADES + THREE - 1];
    players[4]->current_hand[1] = &deck[PIPS_PER_SUIT * SPADES + TWO - 1];

    players[5]->current_hand[0] = &deck[PIPS_PER_SUIT * HEARTS + KING - 1];
    players[5]->current_hand[1] = &deck[PIPS_PER_SUIT * CLUBS + QUEEN - 1];

    players[6]->current_hand[0] = &deck[PIPS_PER_SUIT * SPADES + QUEEN - 1];
    players[6]->current_hand[1] = &deck[PIPS_PER_SUIT * DIAMONDS + SEVEN - 1];

    int winners[] = { -1, -1, -1, -1, -1, -1, -1 };
    int winners_count = gsDetermineWinners(winners, &ruleset, state, players, comm_cards);

    CuAssert(ct, "", winners_count == 1);
    CuAssert(ct, "", winners[0] == 0);

    gsAwardPot(state, players, winners, winners_count);

    CuAssert(ct, "", players[0]->funds == ruleset.funds_per_player + 200);
    for (int i = 1; i < ruleset.player_count; ++i)
        CuAssert(ct, "", players[i]->funds == ruleset.funds_per_player);
    CuAssert(ct, "", state->pot == 0);
}

static void test_standardShowdownWithMultipleWinnersWithIndivisiblePot(CuTest* ct){
    GameRuleSet ruleset = {
        .player_count = 5,
        .funds_per_player = 8000,
        .limit_fixed = true,
        .big_blind = 25,
        .small_blind = 12
    };
    Player* players[ruleset.player_count];
    for (int i = 0; i < ruleset.player_count; ++i)
        players[i] = playerCreateNewWithFunds(ruleset.funds_per_player);
    GameState* state = gsCreateNew(&ruleset);
    state->s_blind_player = 1;
    state->revealed_comm_cards = 5;
    state->betting_round = 4;
    state->all_but_one_folded = false;
    state->pot = 501;

    PlayingCard* deck[DECK_LENGTH];
    buildDeck(&deck, false);
    const PlayingCard* comm_cards[] = {
        &deck[PIPS_PER_SUIT * DIAMONDS + ACE - 1],
        &deck[PIPS_PER_SUIT * HEARTS + QUEEN - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + TEN - 1],
        &deck[PIPS_PER_SUIT * CLUBS + TWO - 1],
        &deck[PIPS_PER_SUIT * HEARTS + THREE - 1]
    };
    //Winner #1 - Ace & Queen pair
    players[0]->current_hand[0] = &deck[PIPS_PER_SUIT * CLUBS + QUEEN - 1];
    players[0]->current_hand[1] = &deck[PIPS_PER_SUIT * CLUBS + ACE - 1];
    //Pair of tens
    players[1]->current_hand[0] = &deck[PIPS_PER_SUIT * CLUBS + TEN - 1];
    players[1]->current_hand[1] = &deck[PIPS_PER_SUIT * HEARTS + FOUR - 1];
    //No rank
    players[2]->current_hand[0] = &deck[PIPS_PER_SUIT * SPADES + KING - 1];
    players[2]->current_hand[1] = &deck[PIPS_PER_SUIT * CLUBS + NINE - 1];
    //No ranks
    players[3]->current_hand[0] = &deck[PIPS_PER_SUIT * HEARTS + FIVE - 1];
    players[3]->current_hand[1] = &deck[PIPS_PER_SUIT * DIAMONDS + EIGHT - 1];
    //Winner #2 - Ace & Queen pair
    players[4]->current_hand[0] = &deck[PIPS_PER_SUIT * SPADES + QUEEN - 1];
    players[4]->current_hand[1] = &deck[PIPS_PER_SUIT * SPADES + ACE - 1];

    int winners[] = { -1, -1, -1, -1, -1 };
    int winners_count = gsDetermineWinners(winners, &ruleset, state, players, comm_cards);

    CuAssert(ct, "", winners_count == 2);
    CuAssert(ct, "", winners[0] == 0);
    CuAssert(ct, "", winners[1] == 4);

    gsAwardPot(state, players, winners, winners_count);

    CuAssert(ct, "", players[0]->funds == ruleset.funds_per_player + 250);
    CuAssert(ct, "", players[1]->funds == ruleset.funds_per_player + 1);
    CuAssert(ct, "", players[2]->funds == ruleset.funds_per_player);
    CuAssert(ct, "", players[3]->funds == ruleset.funds_per_player);
    CuAssert(ct, "", players[4]->funds == ruleset.funds_per_player + 250);
    CuAssert(ct, "", state->pot == 0);
}

static void test_allButOneFoldedShowdown(CuTest* ct){
    GameRuleSet ruleset = {
        .player_count = 6,
        .funds_per_player = 8000,
        .limit_fixed = false,
        .big_blind = 30,
        .small_blind = 15
    };
    Player* players[ruleset.player_count];
    for (int i = 0; i < ruleset.player_count; ++i)
        players[i] = playerCreateNewWithFunds(ruleset.funds_per_player);
    GameState* state = gsCreateNew(&ruleset);
    state->revealed_comm_cards = 5;
    state->betting_round = 4;
    state->all_but_one_folded = true;
    state->pot = 125;

    for (int i = 0; i < ruleset.player_count; ++i){
        if (i != 3)
            players[i]->folded = true;
    }

    int winners[] = { -1, -1, -1, -1, -1, -1 };
    int winners_count = gsDetermineWinners(winners, &ruleset, state, players, NULL);

    CuAssert(ct, "", winners_count == 1);
    CuAssert(ct, "", winners[0] == 3);

    gsAwardPot(state, players, winners, winners_count);

    CuAssert(ct, "", players[3]->funds == ruleset.funds_per_player + 125);
    CuAssert(ct, "", state->pot == 0);
}

static void test_onlyOneTappedoutWinnerShowdown(CuTest* ct){
    GameRuleSet ruleset = {
        .player_count = 5,
        .funds_per_player = 1200,
        .limit_fixed = false,
        .big_blind = 20,
        .small_blind = 10
    };
    Player* players[ruleset.player_count];
    for (int i = 0; i < ruleset.player_count; ++i)
        players[i] = playerCreateNewWithFunds(ruleset.funds_per_player);
    GameState* state = gsCreateNew(&ruleset);
    state->s_blind_player = 3;
    state->revealed_comm_cards = 5;
    state->betting_round = 4;
    state->all_but_one_folded = false;
    state->pot = 550;

    PlayingCard* deck[DECK_LENGTH];
    buildDeck(&deck, false);
    PlayingCard* comm_cards[] = {
        &deck[PIPS_PER_SUIT * DIAMONDS + KING - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + QUEEN - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + JACK - 1],
        &deck[PIPS_PER_SUIT * CLUBS + TWO - 1],
        &deck[PIPS_PER_SUIT * HEARTS + THREE - 1]
    };
    //Winner, also tappedout
    players[0]->current_hand[0] = &deck[PIPS_PER_SUIT * DIAMONDS + TEN - 1];
    players[0]->current_hand[1] = &deck[PIPS_PER_SUIT * DIAMONDS + ACE - 1];
    players[0]->funds = 0;
    players[0]->tappedout_funds = 200;
    players[0]->tappedout = true;

    players[1]->current_hand[0] = &deck[PIPS_PER_SUIT * CLUBS + TEN - 1];
    players[1]->current_hand[1] = &deck[PIPS_PER_SUIT * HEARTS + THREE - 1];

    players[2]->current_hand[0] = &deck[PIPS_PER_SUIT * SPADES + KING - 1];
    players[2]->current_hand[1] = &deck[PIPS_PER_SUIT * CLUBS + KING - 1];

    players[3]->current_hand[0] = &deck[PIPS_PER_SUIT * HEARTS + FIVE - 1];
    players[3]->current_hand[1] = &deck[PIPS_PER_SUIT * DIAMONDS + EIGHT - 1];

    players[4]->current_hand[0] = &deck[PIPS_PER_SUIT * SPADES + THREE - 1];
    players[4]->current_hand[1] = &deck[PIPS_PER_SUIT * SPADES + TWO - 1];

    int winners[] = { -1, -1, -1, -1, -1 };
    int winners_count = gsDetermineWinners(winners, &ruleset, state, players, comm_cards);

    CuAssert(ct, "", winners_count == 1);
    CuAssert(ct, "", winners[0] == 0);

    gsAwardPot(state, players, winners, winners_count);

    CuAssert(ct, "", players[0]->funds == 200);
    CuAssert(ct, "", players[state->s_blind_player]->funds == ruleset.funds_per_player + (550 - 200));
}

static void test_multipleWinnersWhereOnlySomeAreTappedOut(CuTest* ct){
    GameRuleSet ruleset = {
        .player_count = 5,
        .funds_per_player = 1200,
        .limit_fixed = false,
        .big_blind = 20,
        .small_blind = 10
    };
    Player* players[ruleset.player_count];
    for (int i = 0; i < ruleset.player_count; ++i)
        players[i] = playerCreateNewWithFunds(ruleset.funds_per_player);
    GameState* state = gsCreateNew(&ruleset);
    state->s_blind_player = 3;
    state->revealed_comm_cards = 5;
    state->betting_round = 4;
    state->all_but_one_folded = false;
    state->pot = 550;

    PlayingCard* deck[DECK_LENGTH];
    buildDeck(&deck, false);
    const PlayingCard* comm_cards[] = {
        &deck[PIPS_PER_SUIT * DIAMONDS + ACE - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + QUEEN - 1],
        &deck[PIPS_PER_SUIT * CLUBS + TEN - 1],
        &deck[PIPS_PER_SUIT * CLUBS + TWO - 1],
        &deck[PIPS_PER_SUIT * HEARTS + THREE - 1]
    };
    //Winner #1 - Ace & Queen pairs + tappedout
    players[0]->current_hand[0] = &deck[PIPS_PER_SUIT * CLUBS + QUEEN - 1];
    players[0]->current_hand[1] = &deck[PIPS_PER_SUIT * CLUBS + ACE - 1];
    players[0]->funds = 0;
    players[0]->tappedout_funds = 120;
    players[0]->tappedout = true;
    //Pair of tens
    players[1]->current_hand[0] = &deck[PIPS_PER_SUIT * CLUBS + TEN - 1];
    players[1]->current_hand[1] = &deck[PIPS_PER_SUIT * HEARTS + FOUR - 1];
    //Winner #2 - Ace & Queen pairs
    players[2]->current_hand[0] = &deck[PIPS_PER_SUIT * HEARTS + QUEEN - 1];
    players[2]->current_hand[1] = &deck[PIPS_PER_SUIT * HEARTS + ACE - 1];
    //No ranks
    players[3]->current_hand[0] = &deck[PIPS_PER_SUIT * HEARTS + FIVE - 1];
    players[3]->current_hand[1] = &deck[PIPS_PER_SUIT * DIAMONDS + EIGHT - 1];
    //Winner #3 - Ace & Queen pairs + tappedout
    players[4]->current_hand[0] = &deck[PIPS_PER_SUIT * SPADES + QUEEN - 1];
    players[4]->current_hand[1] = &deck[PIPS_PER_SUIT * SPADES + ACE - 1];
    players[4]->funds = 0;
    players[4]->tappedout_funds = 65;
    players[4]->tappedout = true;

    int winners[] = { -1, -1, -1, -1, -1 };
    int winners_count = gsDetermineWinners(winners, &ruleset, state, players, comm_cards);

    CuAssert(ct, "", winners_count == 3);
    CuAssert(ct, "", winners[0] == 0);
    CuAssert(ct, "", winners[1] == 2);
    CuAssert(ct, "", winners[2] == 4);

    gsAwardPot(state, players, winners, winners_count);

    CuAssert(ct, "", players[0]->funds == 120);
    CuAssert(ct, "", players[2]->funds == ruleset.funds_per_player + (550 - 120 - 65));
    CuAssert(ct, "", players[state->s_blind_player]->funds == ruleset.funds_per_player);
    CuAssert(ct, "", players[4]->funds == 65);
}

static void test_showdownWhereEveryoneTappedOut(CuTest* ct){
    GameRuleSet ruleset = {
        .player_count = 6,
        .funds_per_player = 2400,
        .limit_fixed = true,
        .big_blind = 10,
        .small_blind = 5
    };
    Player* players[ruleset.player_count];
    for (int i = 0; i < ruleset.player_count; ++i)
        players[i] = playerCreateNewWithFunds(ruleset.funds_per_player);
    GameState* state = gsCreateNew(&ruleset);
    state->s_blind_player = 1;
    state->revealed_comm_cards = 5;
    state->betting_round = 4;
    state->all_but_one_folded = false;
    state->pot = 650;

    PlayingCard* deck[DECK_LENGTH];
    buildDeck(&deck, false);
    const PlayingCard* comm_cards[] = {
        &deck[PIPS_PER_SUIT * DIAMONDS + ACE - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + QUEEN - 1],
        &deck[PIPS_PER_SUIT * CLUBS + TEN - 1],
        &deck[PIPS_PER_SUIT * CLUBS + TWO - 1],
        &deck[PIPS_PER_SUIT * HEARTS + THREE - 1]
    };
    //Winner #1 - Ace & Queen pairs + tappedout
    players[0]->current_hand[0] = &deck[PIPS_PER_SUIT * CLUBS + QUEEN - 1];
    players[0]->current_hand[1] = &deck[PIPS_PER_SUIT * CLUBS + ACE - 1];
    players[0]->funds = 0;
    players[0]->tappedout_funds = 115;
    players[0]->tappedout = true;
    //Pair of tens
    players[1]->current_hand[0] = &deck[PIPS_PER_SUIT * CLUBS + TEN - 1];
    players[1]->current_hand[1] = &deck[PIPS_PER_SUIT * HEARTS + FOUR - 1];
    //Winner #2 - Ace & Queen pairs + tappedout
    players[2]->current_hand[0] = &deck[PIPS_PER_SUIT * HEARTS + QUEEN - 1];
    players[2]->current_hand[1] = &deck[PIPS_PER_SUIT * HEARTS + ACE - 1];
    players[2]->funds = 0;
    players[2]->tappedout_funds = 100;
    players[2]->tappedout = true;
    //No ranks
    players[3]->current_hand[0] = &deck[PIPS_PER_SUIT * HEARTS + FIVE - 1];
    players[3]->current_hand[1] = &deck[PIPS_PER_SUIT * DIAMONDS + EIGHT - 1];
    //Winner #3 - Ace & Queen pairs + tappedout
    players[4]->current_hand[0] = &deck[PIPS_PER_SUIT * SPADES + QUEEN - 1];
    players[4]->current_hand[1] = &deck[PIPS_PER_SUIT * SPADES + ACE - 1];
    players[4]->funds = 0;
    players[4]->tappedout_funds = 80;
    players[4]->tappedout = true;
    //No ranks
    players[5]->current_hand[0] = &deck[PIPS_PER_SUIT * HEARTS + FOUR - 1];
    players[5]->current_hand[1] = &deck[PIPS_PER_SUIT * DIAMONDS + SEVEN - 1];

    int winners[] = { -1, -1, -1, -1, -1, -1 };
    int winners_count = gsDetermineWinners(winners, &ruleset, state, players, comm_cards);

    CuAssert(ct, "", winners_count == 3);
    CuAssert(ct, "", winners[0] == 0);
    CuAssert(ct, "", winners[1] == 2);
    CuAssert(ct, "", winners[2] == 4);

    gsAwardPot(state, players, winners, winners_count);

    CuAssert(ct, "", players[0]->funds == 115);
    CuAssert(ct, "", players[state->s_blind_player]->funds == ruleset.funds_per_player + (650 - 115 - 100 - 80));
    CuAssert(ct, "", players[2]->funds == 100);
    CuAssert(ct, "", players[4]->funds == 80);
}

//  --  Checking for game overs --

static void test_markingBrokePlayers(CuTest* ct){
    GameRuleSet ruleset = {
        .player_count = 5
    };
    Player* players[ruleset.player_count];
    for (int i = 0; i < ruleset.player_count; ++i)
        players[i] = playerCreateNew();
    GameState* state = gsCreateNew(&ruleset);

    players[0]->funds = 900;
    players[0]->folded = true;

    players[1]->funds = 0;
    players[1]->folded = true;

    players[2]->funds = 0;
    players[2]->folded = false;

    players[3]->funds = 1800;
    players[3]->folded = false;

    players[4]->funds = 0;
    players[4]->folded = false;
    players[4]->tappedout = true;

    gsCheckGameOverCondition(players, &ruleset);

    CuAssert(ct, "", !(players[0]->folded));
    CuAssert(ct, "", players[1]->folded);
    CuAssert(ct, "", players[2]->folded);
    CuAssert(ct, "", !(players[3]->folded));
    CuAssert(ct, "", players[4]->folded);
    CuAssert(ct, "", !(players[4]->tappedout));
}

static void test_everyoneButOnePlayerIsBroke(CuTest* ct){
    GameRuleSet ruleset = {
        .player_count = 5
    };
    Player* players[ruleset.player_count];
    for (int i = 0; i < ruleset.player_count; ++i)
        players[i] = playerCreateNewWithFunds(0);
    GameState* state = gsCreateNew(&ruleset);

    players[2]->funds = 1;
    CuAssert(ct, "", gsCheckGameOverCondition(players, &ruleset));

    players[3]->funds = 1;
    CuAssert(ct, "", !gsCheckGameOverCondition(players, &ruleset));
}

//  --  Others  --

static void test_passingAroundDealersButton(CuTest* ct){
    const GameRuleSet ruleSet = {
        .player_count = 4,
        .funds_per_player = 10000,
        .limit_fixed = false,
        .big_blind = 10,
        .small_blind = 5
    };
    GameState* state = gsCreateNew(&ruleSet);

    CuAssert(ct, "", state->dealer_player == 0);
    CuAssert(ct, "", state->s_blind_player == 1);
    CuAssert(ct, "", state->b_blind_player == 2);

    gsPassDealerButton(state, &ruleSet);

    CuAssert(ct, "", state->dealer_player == 1);
    CuAssert(ct, "", state->s_blind_player == 2);
    CuAssert(ct, "", state->b_blind_player == 3);

    gsPassDealerButton(state, &ruleSet);

    CuAssert(ct, "", state->dealer_player == 2);
    CuAssert(ct, "", state->s_blind_player == 3);
    CuAssert(ct, "", state->b_blind_player == 0);

    gsPassDealerButton(state, &ruleSet);

    CuAssert(ct, "", state->dealer_player == 3);
    CuAssert(ct, "", state->s_blind_player == 0);
    CuAssert(ct, "", state->b_blind_player == 1);
}

CuSuite* GamestateGetSuite(CuTest* ct){
    CuSuite* suite = CuSuiteNew();
    msgInitFromFile("loc.txt");
    SUITE_ADD_TEST(suite, test_checkConstructor);

    SUITE_ADD_TEST(suite, test_raiseRightAfterStartingGame);
    SUITE_ADD_TEST(suite, test_advancePlayerTurnsThroughRoundWithNoRaises);
    SUITE_ADD_TEST(suite, test_advancePlayerTurnsThroughRoundWithSomeLowFixedRaises);
    SUITE_ADD_TEST(suite, test_advancePlayerTurnsThroughRoundWithSomeHighFixedRaises);
    SUITE_ADD_TEST(suite, test_advancePlayerTurnsThroughRoundWithSomeUnlimitedRaises);
    SUITE_ADD_TEST(suite, test_advancePlayerTurnsThroughRoundWithOnlyRaises);
    SUITE_ADD_TEST(suite, test_checkTheWhileLoopInMainForPlayerTurnAdvancing);
    SUITE_ADD_TEST(suite, test_allButOneFoldedConditionCheckForAdvancePlayerTurn);
    SUITE_ADD_TEST(suite, test_advancingToNextBettingRoundResetsTurnsLeftProperly);
    SUITE_ADD_TEST(suite, test_concludingSingleGame);

    SUITE_ADD_TEST(suite, test_settingUpPreFlopRound);
    SUITE_ADD_TEST(suite, test_settingUpFlopRound);
    SUITE_ADD_TEST(suite, test_settingUpTurnRound);
    SUITE_ADD_TEST(suite, test_settingUpRiverRound);
    SUITE_ADD_TEST(suite, test_settingUpPreFlopRoundWhereSomePlayersCantAffordForcedBets);

    SUITE_ADD_TEST(suite, test_singleTapOut);
    SUITE_ADD_TEST(suite, test_multipleTapouts);

    SUITE_ADD_TEST(suite, test_standardShowdownWithSingleWinner);
    SUITE_ADD_TEST(suite, test_standardShowdownWithMultipleWinnersWithIndivisiblePot);
    SUITE_ADD_TEST(suite, test_allButOneFoldedShowdown);
    SUITE_ADD_TEST(suite, test_onlyOneTappedoutWinnerShowdown);
    SUITE_ADD_TEST(suite, test_multipleWinnersWhereOnlySomeAreTappedOut);
    SUITE_ADD_TEST(suite, test_showdownWhereEveryoneTappedOut);

    SUITE_ADD_TEST(suite, test_markingBrokePlayers);
    SUITE_ADD_TEST(suite, test_everyoneButOnePlayerIsBroke);

    SUITE_ADD_TEST(suite, test_passingAroundDealersButton);
    return suite;
}
