#include <stdbool.h>
#include "CuTest.h"
#include "../src/dealer.h"
#include "../src/player.h"

void test_decideWinners_obviousWinner(CuTest* ct){
    Player players[5];
    for (int i = 0; i < 5; i++){
        resetScores(&players[i]);
    }
    players[2].scores[0] = 1;

    int winners[5];
    int winners_count = decideWinners(players, 5, winners);

    CuAssert(ct, "", winners[0] == 2);
    CuAssert(ct, "", winners_count == 1);
}

void test_decideWinners_onePlayerHasGreaterScoreButInLowerTier(CuTest* ct){
    Player players[5];
    for (int i = 0; i < 5; i++){
        resetScores(&players[i]);
    }
    players[4].scores[2] = 1;
    players[2].scores[3] = 6000;

    int winners[5];
    int winners_count = decideWinners(players, 5, winners);

    CuAssert(ct, "", winners[0] == 4);
    CuAssert(ct, "", winners_count == 1);
}

void test_decideWinners_multiplePlayerSameScore(CuTest* ct){
    Player players[5];
    for (int i = 0; i < 5; i++){
        resetScores(&players[i]);
    }
    players[0].scores[3] = 4000;
    players[2].scores[3] = 6000;
    players[4].scores[3] = 6000;

    int winners[5];
    int winners_count = decideWinners(players, 5, winners);

    CuAssert(ct, "", winners[0] == 2);
    CuAssert(ct, "", winners[1] == 4);
    CuAssert(ct, "", winners_count == 2);
}

void test_decideWinners_correctlySelectWinner(CuTest* ct){
    Player players[5];
    for (int i = 0; i < 5; i++){
        resetScores(&players[i]);
    }
    players[0].scores[5] = 1000;
    players[1].scores[5] = 900;
    players[2].scores[5] = 800;
    players[3].scores[5] = 700;
    players[4].scores[5] = 2000;

    int winners[5];
    int winners_count = decideWinners(players, 5, winners);

    CuAssert(ct, "", winners[0] == 4);
    CuAssert(ct, "", winners_count == 1);
}

void test_decideWinners_largerScoreInLowerTiersDontWin(CuTest* ct){
    Player players[5];
    for (int i = 0; i < 5; i++){
        resetScores(&players[i]);
    }
    players[0].scores[2] = 100;
    players[1].scores[2] = 900;
    players[2].scores[2] = 800;
    players[3].scores[2] = 700;
    players[4].scores[2] = 900;
    players[1].scores[3] = 9000;

    int winners[5];
    int winners_count = decideWinners(players, 5, winners);

    CuAssert(ct, "", winners[0] == 1);
    CuAssert(ct, "", winners[1] == 4);
    CuAssert(ct, "", winners_count == 2);
}

void test_decideWinners_miscTest1(CuTest* ct){
    Player players[5];
    for (int i = 0; i < 5; i++){
        resetScores(&players[i]);
    }
    players[1].scores[8] = 100;
    players[2].scores[8] = 900;
    players[2].scores[10] = 1200;
    players[3].scores[8] = 800;
    players[3].scores[10] = 1200;

    int winners[5];
    int winners_count = decideWinners(players, 5, winners);

    CuAssert(ct, "", winners[0] == 2);
    CuAssert(ct, "", winners_count == 1);
}

void test_checkPlayerDecisionValidity_CallsChecks_scenario1(CuTest* ct){
    const Player* _player = playerCreateNewWithFunds(1000);
    const GameRuleSet rules = {
        .limit_fixed = true,
        .big_blind = 10,
        .small_blind = 5
    };
    const GameState state = {
        .betting_round = 1,
        .raises_performed = 1,
        .bet = 15
    };
    int decision = 0;
    char response[PDVC_RESPONSE_LENGTH];

    bool result = checkPlayerDecisionValidity(_player, &state, &rules, decision, response);

    CuAssert(ct, response, result);
    CuAssertStrEquals(ct, "", response);
}

void test_checkPlayerDecisionValidity_CallsChecks_scenario2(CuTest* ct){
    const Player* _player = playerCreateNewWithFunds(200);
    const GameRuleSet rules = {
        .limit_fixed = true,
        .big_blind = 100,
        .small_blind = 50
    };
    const GameState state = {
        .betting_round = 3,
        .raises_performed = MAX_BETS_PER_ROUND_OBJ,
        .bet = 300
    };
    int decision = 0;
    char response[PDVC_RESPONSE_LENGTH];

    bool result = checkPlayerDecisionValidity(_player, &state, &rules, decision, response);

    CuAssert(ct, response, !result);
    CuAssertStrEquals(ct, "You cannot afford to call the bet.", response);
}

void test_checkPlayerDecisionValidity_RaisesInFixedGame_scenario1(CuTest* ct){
    const Player* _player = playerCreateNewWithFunds(1000);
    const GameRuleSet rules = {
        .limit_fixed = true,
        .big_blind = 100,
        .small_blind = 50
    };
    const GameState state = {
        .betting_round = 0,
        .raises_performed = 0,
        .bet = 100
    };
    int decision = 1;
    char response[PDVC_RESPONSE_LENGTH];

    bool result = checkPlayerDecisionValidity(_player, &state, &rules, decision, response);

    CuAssert(ct, response, result);
    CuAssertStrEquals(ct, "", response);
}

void test_checkPlayerDecisionValidity_RaisesInFixedGame_scenario2(CuTest* ct){
    const Player* _player = playerCreateNewWithFunds(149);
    const GameRuleSet rules = {
        .limit_fixed = true,
        .big_blind = 100,
        .small_blind = 50
    };
    const GameState state = {
        .betting_round = 0,
        .raises_performed = 0,
        .bet = 100
    };
    int decision = 1;
    char response[PDVC_RESPONSE_LENGTH];

    bool result = checkPlayerDecisionValidity(_player, &state, &rules, decision, response);

    CuAssert(ct, response, !result);
    CuAssertStrEquals(ct, "You cannot afford to raise the bet by the required small blind amount.", response);
}

void test_checkPlayerDecisionValidity_RaisesInFixedGame_scenario3(CuTest* ct){
    const Player* _player = playerCreateNewWithFunds(1000);
    const GameRuleSet rules = {
        .limit_fixed = true,
        .big_blind = 100,
        .small_blind = 50
    };
    const GameState state = {
        .betting_round = 1,
        .raises_performed = MAX_BETS_PER_ROUND_OBJ,
        .bet = 50 * MAX_BETS_PER_ROUND_OBJ
    };
    int decision = 1;
    char response[PDVC_RESPONSE_LENGTH];

    bool result = checkPlayerDecisionValidity(_player, &state, &rules, decision, response);

    CuAssert(ct, response, !result);
    CuAssertStrEquals(ct, "The limit of raises per one betting round has already been reached.", response);
}

void test_checkPlayerDecisionValidity_RaisesInFixedGame_scenario4(CuTest* ct){
    const Player* _player = playerCreateNewWithFunds(299);
    const GameRuleSet rules = {
        .limit_fixed = true,
        .big_blind = 100,
        .small_blind = 50
    };
    const GameState state = {
        .betting_round = 3,
        .raises_performed = 2,
        .bet = 200
    };
    int decision = 1;
    char response[PDVC_RESPONSE_LENGTH];

    bool result = checkPlayerDecisionValidity(_player, &state, &rules, decision, response);

    CuAssert(ct, response, !result);
    CuAssertStrEquals(ct, "You cannot afford to raise the bet by the required big blind amount.", response);
}

void test_checkPlayerDecisionValidity_RaisesInNoLimitGame_scenario1(CuTest* ct){
    const Player* _player = playerCreateNewWithFunds(1000);
    const GameRuleSet rules = {
        .limit_fixed = false,
        .big_blind = 100,
        .small_blind = 50
    };
    const GameState state = {
        .betting_round = 1,
        .raises_performed = 2,
        .bet = 240
    };
    int decision = 241;
    char response[PDVC_RESPONSE_LENGTH];

    bool result = checkPlayerDecisionValidity(_player, &state, &rules, decision, response);

    CuAssert(ct, response, result);
    CuAssertStrEquals(ct, "", response);
}

void test_checkPlayerDecisionValidity_RaisesInNoLimitGame_scenario2(CuTest* ct){
    const Player* _player = playerCreateNewWithFunds(1000);
    const GameRuleSet rules = {
        .limit_fixed = false,
        .big_blind = 100,
        .small_blind = 50
    };
    const GameState state = {
        .betting_round = 2,
        .raises_performed = 5,
        .bet = 550
    };
    int decision = 540;
    char response[PDVC_RESPONSE_LENGTH];

    bool result = checkPlayerDecisionValidity(_player, &state, &rules, decision, response);

    CuAssert(ct, response, !result);
    CuAssertStrEquals(ct, "You cannot lower the bet, it can only be raised up.", response);
}

void test_checkPlayerDecisionValidity_RaisesInNoLimitGame_scenario3(CuTest* ct){
    const Player* _player = playerCreateNewWithFunds(200);
    const GameRuleSet rules = {
        .limit_fixed = false,
        .big_blind = 100,
        .small_blind = 50
    };
    const GameState state = {
        .betting_round = 3,
        .raises_performed = 5,
        .bet = 200
    };
    int decision = 201;
    char response[PDVC_RESPONSE_LENGTH];

    bool result = checkPlayerDecisionValidity(_player, &state, &rules, decision, response);

    CuAssert(ct, response, !result);
    CuAssertStrEquals(ct, "You cannot afford to raise the bet by the specified amount.", response);
}

void test_checkPlayerDecisionValidity_TapOuts_scenario1(CuTest* ct){
    const Player* _player = playerCreateNewWithFunds(200);
    const GameRuleSet rules = {
        .limit_fixed = false,
        .big_blind = 100,
        .small_blind = 50
    };
    const GameState state = {
        .betting_round = 2,
        .raises_performed = 2,
        .bet = 210
    };
    int decision = -2;
    char response[PDVC_RESPONSE_LENGTH];

    bool result = checkPlayerDecisionValidity(_player, &state, &rules, decision, response);

    CuAssert(ct, response, result);
    CuAssertStrEquals(ct, "", response);
}

void test_checkPlayerDecisionValidity_TapOuts_scenario2(CuTest* ct){
    const Player* _player = playerCreateNewWithFunds(200);
    const GameRuleSet rules = {
        .limit_fixed = false,
        .big_blind = 100,
        .small_blind = 50
    };
    const GameState state = {
        .betting_round = 2,
        .raises_performed = 2,
        .bet = 200
    };
    int decision = -2;
    char response[PDVC_RESPONSE_LENGTH];

    bool result = checkPlayerDecisionValidity(_player, &state, &rules, decision, response);

    CuAssert(ct, response, !result);
    CuAssertStrEquals(ct, "You can still afford to call the current bet. You may not tap out just yet.", response);
}

CuSuite* DealerGetSuite(){
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_decideWinners_obviousWinner);
    SUITE_ADD_TEST(suite, test_decideWinners_onePlayerHasGreaterScoreButInLowerTier);
    SUITE_ADD_TEST(suite, test_decideWinners_multiplePlayerSameScore);
    SUITE_ADD_TEST(suite, test_decideWinners_correctlySelectWinner);
    SUITE_ADD_TEST(suite, test_decideWinners_largerScoreInLowerTiersDontWin);
    SUITE_ADD_TEST(suite, test_decideWinners_miscTest1);

    SUITE_ADD_TEST(suite, test_checkPlayerDecisionValidity_CallsChecks_scenario1);
    SUITE_ADD_TEST(suite, test_checkPlayerDecisionValidity_CallsChecks_scenario2);
    SUITE_ADD_TEST(suite, test_checkPlayerDecisionValidity_RaisesInFixedGame_scenario1);
    SUITE_ADD_TEST(suite, test_checkPlayerDecisionValidity_RaisesInFixedGame_scenario2);
    SUITE_ADD_TEST(suite, test_checkPlayerDecisionValidity_RaisesInFixedGame_scenario3);
    SUITE_ADD_TEST(suite, test_checkPlayerDecisionValidity_RaisesInFixedGame_scenario4);
    SUITE_ADD_TEST(suite, test_checkPlayerDecisionValidity_RaisesInNoLimitGame_scenario1);
    SUITE_ADD_TEST(suite, test_checkPlayerDecisionValidity_RaisesInNoLimitGame_scenario2);
    SUITE_ADD_TEST(suite, test_checkPlayerDecisionValidity_RaisesInNoLimitGame_scenario3);
    SUITE_ADD_TEST(suite, test_checkPlayerDecisionValidity_TapOuts_scenario1);
    SUITE_ADD_TEST(suite, test_checkPlayerDecisionValidity_TapOuts_scenario2);
    return suite;
}
