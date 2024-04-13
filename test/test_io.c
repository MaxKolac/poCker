#include <limits.h>
#include "CuTest.h"
#include "../src/io.h"

void test_decisionRecognition_callCheck(CuTest* ct){
    char input1[] = "call";
    int result = recognizeDecision(input1);
    CuAssert(ct, "", result == 0);

    char input2[] = "check";
    result = recognizeDecision(input2);
    CuAssert(ct, "", result == 0);

    char input3[] = "check  ";
    result = recognizeDecision(input3);
    CuAssert(ct, "", result == 0);

    char input4[] = "check 1000";
    result = recognizeDecision(input4);
    CuAssert(ct, "", result == 0);

    char input5[] = "Check";
    result = recognizeDecision(input5);
    CuAssert(ct, "", result == 0);

    char input6[] = "  check check chec";
    int result6 = recognizeDecision(input6);
    CuAssert(ct, "", result6 == 0);
}

void test_decisionRecognition_folding(CuTest* ct){
    char input1[] = "fold";
    int result = recognizeDecision(input1);
    CuAssert(ct, "", result == -1);

    char input2[] = "fold out";
    result = recognizeDecision(input2);
    CuAssert(ct, "", result == -1);

    char input3[] = "fold 20";
    result = recognizeDecision(input3);
    CuAssert(ct, "", result == -1);

    char input4[] = "FOld";
    result = recognizeDecision(input4);
    CuAssert(ct, "", result == -1);
}

void test_decisionRecognition_tapouts(CuTest* ct){
    char input1[] = "tapout";
    int result = recognizeDecision(input1);
    CuAssert(ct, "", result == -2);

    char input2[] = "tapout 100";
    result = recognizeDecision(input2);
    CuAssert(ct, "", result == -2);

    char input3[] = "TApOut";
    result = recognizeDecision(input3);
    CuAssert(ct, "", result == -2);
}

void test_decisionRecognition_raises(CuTest* ct){
    char input1[] = "raise 100";
    int result = recognizeDecision(input1);
    CuAssert(ct, "", result == 100);

    char input2[] = "RAise";
    result = recognizeDecision(input2);
    CuAssert(ct, "", result == 1);

    char input3[] = "Raise -100";
    result = recognizeDecision(input3);
    CuAssert(ct, "", result == 1);

    char input4[] = "raise 125 25";
    result = recognizeDecision(input4);
    CuAssert(ct, "", result == 125);
}

void test_decisionRecognition_invalidInputs(CuTest* ct){
    char input1[] = "tap out";
    int result1 = recognizeDecision(input1);
    CuAssert(ct, "", result1 == INT_MIN);

    char input2[] = "ca ll";
    int result2 = recognizeDecision(input2);
    CuAssert(ct, "", result2 == INT_MIN);

    char input3[] = "flod";
    int result3 = recognizeDecision(input3);
    CuAssert(ct, "", result3 == INT_MIN);

    char input4[] = "";
    int result4 = recognizeDecision(input4);
    CuAssert(ct, "", result4 == INT_MIN);

    char input5[] = "     ";
    int result5 = recognizeDecision(input5);
    CuAssert(ct, "", result5 == INT_MIN);

    char input6[] = " ,  ";
    int result6 = recognizeDecision(input6);
    CuAssert(ct, "", result6 == INT_MIN);

    char input7[] = "100";
    int result7 = recognizeDecision(input7);
    CuAssert(ct, "", result7 == INT_MIN);
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
    char response[IO_RESPONSE_LENGTH];

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
    char response[IO_RESPONSE_LENGTH];

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
    char response[IO_RESPONSE_LENGTH];

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
    char response[IO_RESPONSE_LENGTH];

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
    char response[IO_RESPONSE_LENGTH];

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
    char response[IO_RESPONSE_LENGTH];

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
    char response[IO_RESPONSE_LENGTH];

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
    char response[IO_RESPONSE_LENGTH];

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
    char response[IO_RESPONSE_LENGTH];

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
    char response[IO_RESPONSE_LENGTH];

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
    char response[IO_RESPONSE_LENGTH];

    bool result = checkPlayerDecisionValidity(_player, &state, &rules, decision, response);

    CuAssert(ct, response, !result);
    CuAssertStrEquals(ct, "You can still afford to call the current bet. You may not tap out just yet.", response);
}


CuSuite* IoGetSuite() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_decisionRecognition_callCheck);
    SUITE_ADD_TEST(suite, test_decisionRecognition_folding);
    SUITE_ADD_TEST(suite, test_decisionRecognition_tapouts);
    SUITE_ADD_TEST(suite, test_decisionRecognition_raises);
    SUITE_ADD_TEST(suite, test_decisionRecognition_invalidInputs);

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

