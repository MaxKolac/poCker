#include "CuTest.h"
#include "../src/gamestate.h"

void test_gamestate_checkConstructor(CuTest* ct){
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
void test_gamestate_raiseRightAfterStartingGame(CuTest* ct){
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
    unsigned int tapout_pot_statuses[ruleSet.player_count];
    for (int i = 0; i < ruleSet.player_count; ++i)
        tapout_pot_statuses[i] = 0;

    gsSetUpBettingRound(state, players, &ruleSet);
    int decision = 25;
    gsAdvancePlayerTurn(state, players, tapout_pot_statuses, &ruleSet, &decision);

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

void test_gamestate_advancePlayerTurnsThroughRoundWithNoRaises(CuTest* ct){
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
    unsigned int tapout_pot_statuses[ruleSet.player_count];
    for (int i = 0; i < ruleSet.player_count; ++i)
        tapout_pot_statuses[i] = 0;

    gsSetUpBettingRound(state, players, &ruleSet);
    int decision = 0;
    gsAdvancePlayerTurn(state, players, tapout_pot_statuses, &ruleSet, &decision);
    gsAdvancePlayerTurn(state, players, tapout_pot_statuses, &ruleSet, &decision);
    gsAdvancePlayerTurn(state, players, tapout_pot_statuses, &ruleSet, &decision);
    gsAdvancePlayerTurn(state, players, tapout_pot_statuses, &ruleSet, &decision);

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

void test_gamestate_advancePlayerTurnsThroughRoundWithSomeRaises(CuTest* ct){
    CuAssert(ct, "TODO", false);
}

void test_gamestate_advancePlayerTurnsThroughRoundWithOnlyRaises(CuTest* ct){
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
    unsigned int tapout_pot_statuses[ruleSet.player_count];
    for (int i = 0; i < ruleSet.player_count; ++i)
        tapout_pot_statuses[i] = 0;

    //dealer is 0, small b is 1, big b is 2
    gsSetUpBettingRound(state, players, &ruleSet);
    CuAssert(ct, "", state->pot == 36);
    CuAssert(ct, "", state->bet == 24);

    //player[3]
    int decision = 40;
    gsAdvancePlayerTurn(state, players, tapout_pot_statuses, &ruleSet, &decision);
    CuAssert(ct, "", state->pot == 76);
    CuAssert(ct, "", state->bet == 40);

    //player[4]
    decision = 45;
    gsAdvancePlayerTurn(state, players, tapout_pot_statuses, &ruleSet, &decision);
    CuAssert(ct, "", state->pot == 121);
    CuAssert(ct, "", state->bet == 45);

    //player[5]
    decision = 55;
    gsAdvancePlayerTurn(state, players, tapout_pot_statuses, &ruleSet, &decision);
    CuAssert(ct, "", state->pot == 176);
    CuAssert(ct, "", state->bet == 55);

    //player[6]
    decision = 70;
    gsAdvancePlayerTurn(state, players, tapout_pot_statuses, &ruleSet, &decision);
    CuAssert(ct, "", state->pot == 246);
    CuAssert(ct, "", state->bet == 70);

    //player[0]
    decision = 82;
    gsAdvancePlayerTurn(state, players, tapout_pot_statuses, &ruleSet, &decision);
    CuAssert(ct, "", state->pot == 328);
    CuAssert(ct, "", state->bet == 82);

    //player[1]
    decision = 90;
    gsAdvancePlayerTurn(state, players, tapout_pot_statuses, &ruleSet, &decision);
    CuAssert(ct, "", state->pot == 418);
    CuAssert(ct, "", state->bet == 90);

    //player[2]
    decision = 100;
    gsAdvancePlayerTurn(state, players, tapout_pot_statuses, &ruleSet, &decision);
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

void test_gamestate_checkTheWhileLoopInMainForPlayerTurnAdvancing(CuTest* ct){
    CuAssert(ct, "TODO", false);
    //The purpose of this UT was to see if this loop from main.c works properly
    //while (state->turns_left > 0)
    //    gsAdvancePlayerTurn(globalState, players, tapout_pot_statuses, &globalRules, NULL);

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
    CuAssert(ct, "", state->turns_left == 3);
    CuAssert(ct, "", state->raises_performed == 0);
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
    CuAssert(ct, "", state->turns_left == 5);
    CuAssert(ct, "", state->raises_performed == 0);
    CuAssert(ct, "", !(state->all_but_one_folded));
    CuAssert(ct, "", state->pot == 30);
    CuAssert(ct, "", state->bet == 0);

    CuAssert(ct, "", players[state->dealer_player]->funds == 5500);
    CuAssert(ct, "", players[state->s_blind_player]->funds == 5500 - 10);
    CuAssert(ct, "", players[state->b_blind_player]->funds == 5500 - 20);
}

void test_gamestate_settingUpTurnRound(CuTest* ct){
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
    CuAssert(ct, "", state->turns_left == 9);
    CuAssert(ct, "", state->raises_performed == 0);
    CuAssert(ct, "", !(state->all_but_one_folded));
    CuAssert(ct, "", state->pot == 75);
    CuAssert(ct, "", state->bet == 0);

    CuAssert(ct, "", players[state->dealer_player]->funds == 12000);
    CuAssert(ct, "", players[state->s_blind_player]->funds == 12000 - 25);
    CuAssert(ct, "", players[state->b_blind_player]->funds == 12000 - 50);
}

void test_gamestate_settingUpRiverRound(CuTest* ct){
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
    CuAssert(ct, "", state->turns_left == 3);
    CuAssert(ct, "", state->raises_performed == 0);
    CuAssert(ct, "", !(state->all_but_one_folded));
    CuAssert(ct, "", state->pot == 15);
    CuAssert(ct, "", state->bet == 0);

    CuAssert(ct, "", players[state->dealer_player]->funds == 1200);
    CuAssert(ct, "", players[state->s_blind_player]->funds == 1200 - 5);
    CuAssert(ct, "", players[state->b_blind_player]->funds == 1200 - 10);
}

//  --  Tap-out scenarios   --

void test_gamestate_singleTapOutAtEndOfaGame(CuTest* ct){
    CuAssert(ct, "TODO", false);
}

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
    CuAssert(ct, "TODO", false);
}

CuSuite* GamestateGetSuite(CuTest* ct){
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_gamestate_checkConstructor);

    SUITE_ADD_TEST(suite, test_gamestate_raiseRightAfterStartingGame);

    SUITE_ADD_TEST(suite, test_gamestate_advancePlayerTurnsThroughRoundWithNoRaises);
    SUITE_ADD_TEST(suite, test_gamestate_advancePlayerTurnsThroughRoundWithSomeRaises);
    SUITE_ADD_TEST(suite, test_gamestate_advancePlayerTurnsThroughRoundWithOnlyRaises);
    SUITE_ADD_TEST(suite, test_gamestate_checkTheWhileLoopInMainForPlayerTurnAdvancing);

    SUITE_ADD_TEST(suite, test_gamestate_settingUpPreFlopRound);
    SUITE_ADD_TEST(suite, test_gamestate_settingUpFlopRound);
    SUITE_ADD_TEST(suite, test_gamestate_settingUpTurnRound);
    SUITE_ADD_TEST(suite, test_gamestate_settingUpRiverRound);

    SUITE_ADD_TEST(suite, test_gamestate_singleTapOutAtEndOfaGame);

    SUITE_ADD_TEST(suite, test_gamestate_passingAroundDealersButton);
    SUITE_ADD_TEST(suite, test_gamestate_simulateWholeOneGame);
    return suite;
}
