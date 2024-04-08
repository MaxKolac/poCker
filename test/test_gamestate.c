#include "CuTest.h"
#include "../src/gamestate.h"

//  --  Advancing player turns  --
void test_gamestate_advancePlayerTurnRightAfterStartingGame(CuTest* ct){

}

void test_gamestate_advancePlayerTurnsThroughRoundWithNoRaises(CuTest* ct){

}

void test_gamestate_advancePlayerTurnsThroughRoundWithSomeRaises(CuTest* ct){

}

void test_gamestate_advancePlayerTurnsThroughRoundWithOnlyRaises(CuTest* ct){

}

void test_gamestate_advanceThroughAllPlayerTurns(CuTest* ct){

}

//  --  Setting up betting rounds   --
void test_gamestate_settingUpPreFlopRound(CuTest* ct){
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
    CuAssert(ct, "", state->turns_left == 4);
    CuAssert(ct, "", !(state->all_but_one_folded));
    CuAssert(ct, "", state->pot == 15);
    CuAssert(ct, "", state->bet == 10);

    CuAssert(ct, "", players[state->dealer_player]->funds == 10000);
    CuAssert(ct, "", players[state->s_blind_player]->funds == 9995);
    CuAssert(ct, "", players[state->b_blind_player]->funds == 9990);
}

void test_gamestate_settingUpFlopRound(CuTest* ct){
    const GameRuleSet ruleSet = {
        .player_count = 6,
        .funds_per_player = 5500,
        .limit_fixed = false,
        .big_blind = 20,
        .small_blind = 10
    };
    GameState* state = gsCreateNew(&ruleSet);
    Player* players[ruleSet->player_count];
    for (int i = 0; i < ruleSet->player_count; ++i)
        players[i] = playerCreateNewWithFunds(ruleSet.funds_per_player);

    gsSetUpBettingRound(state, players, &ruleSet);

    CuAssert(ct, "", state->revealed_comm_cards == 0);
    CuAssert(ct, "", state->dealer_player == 0);
    CuAssert(ct, "", state->s_blind_player == 1);
    CuAssert(ct, "", state->b_blind_player == 2);
    CuAssert(ct, "", state->current_player == 3);
    CuAssert(ct, "", state->betting_round == 0);
    CuAssert(ct, "", state->turns_left == 4);
    CuAssert(ct, "", !(state->all_but_one_folded));
    CuAssert(ct, "", state->pot == 15);
    CuAssert(ct, "", state->bet == 10);

    CuAssert(ct, "", players[state->dealer_player]->funds == 10000);
    CuAssert(ct, "", players[state->s_blind_player]->funds == 9995);
    CuAssert(ct, "", players[state->b_blind_player]->funds == 9990);
}

void test_gamestate_settingUpTurnRound(CuTest* ct){

}

void test_gamestate_settingUpRiverRound(CuTest* ct){

}

//  --  Concluding betting rounds   --


//  --  Performing showdowns    --

//  --  Checking for game overs --

void test_gamestate_passingAroundDealersButton(CuTest* ct){
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

void test_gamestate_simulateWholeOneGame(CuTest* ct){

}

CuSuite* GamestateGetSuite(CuTest* ct){
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_gamestate_advancePlayerTurnRightAfterStartingGame);
    SUITE_ADD_TEST(suite, test_gamestate_advancePlayerTurnsThroughRoundWithNoRaises);
    SUITE_ADD_TEST(suite, test_gamestate_advancePlayerTurnsThroughRoundWithSomeRaises);
    SUITE_ADD_TEST(suite, test_gamestate_advancePlayerTurnsThroughRoundWithOnlyRaises);
    SUITE_ADD_TEST(suite, test_gamestate_settingUpPreFlopRound);
    SUITE_ADD_TEST(suite, test_gamestate_settingUpFlopRound);
    SUITE_ADD_TEST(suite, test_gamestate_settingUpTurnRound);
    SUITE_ADD_TEST(suite, test_gamestate_settingUpRiverRound);
    SUITE_ADD_TEST(suite, test_gamestate_passingAroundDealersButton);
    SUITE_ADD_TEST(suite, test_gamestate_simulateWholeOneGame);
    return suite;
}
