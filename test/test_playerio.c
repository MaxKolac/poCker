#include <limits.h>
#include "CuTest.h"
#include "../src/playerio.h"
#include "../src/messages.h"

static void setup_globalMsgsInitialization(CuTest* ct){
    msgInitFromFile(MESSAGES_FILENAME);
    for (int i = 0; i < MESSAGES_COUNT; ++i){
        CuAssert(ct, "", strcmp(GLOBAL_MSGS[i].key, "") != 0);
        CuAssert(ct, "", strcmp(GLOBAL_MSGS[i].message, "") != 0);
    }
}

static void test_decisionRecognition_callCheck(CuTest* ct){
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

static void test_decisionRecognition_folding(CuTest* ct){
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

static void test_decisionRecognition_tapouts(CuTest* ct){
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

static void test_decisionRecognition_raises(CuTest* ct){
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

static void test_decisionRecognition_invalidInputs(CuTest* ct){
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

static void test_checkPlayerDecisionValidity_CallsChecks_scenario1(CuTest* ct){
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
    char response[MESSAGES_MAX_MSG_LENGTH];

    bool result = checkPlayerDecisionValidity(_player, &state, &rules, decision, response);

    CuAssert(ct, response, result);
    CuAssertStrEquals(ct, "", response);
}

static void test_checkPlayerDecisionValidity_CallsChecks_scenario2(CuTest* ct){
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
    char response[MESSAGES_MAX_MSG_LENGTH];

    bool result = checkPlayerDecisionValidity(_player, &state, &rules, decision, response);

    CuAssert(ct, response, !result);
    CuAssertStrEquals(ct, "You cannot afford to call the bet.\n", response);
    CuAssertStrEquals(ct, msgGetn(GLOBAL_MSGS, "PIO_CPDV_CALL"), response);
}

static void test_checkPlayerDecisionValidity_RaisesInFixedGame_scenario1(CuTest* ct){
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
    char response[MESSAGES_MAX_MSG_LENGTH];

    bool result = checkPlayerDecisionValidity(_player, &state, &rules, decision, response);

    CuAssert(ct, response, result);
    CuAssertStrEquals(ct, "", response);
}

static void test_checkPlayerDecisionValidity_RaisesInFixedGame_scenario2(CuTest* ct){
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
    char response[MESSAGES_MAX_MSG_LENGTH];

    bool result = checkPlayerDecisionValidity(_player, &state, &rules, decision, response);

    CuAssert(ct, response, !result);
    CuAssertStrEquals(ct, "You cannot afford to raise the bet by the required small blind amount.\n", response);
    CuAssertStrEquals(ct, msgGetn(GLOBAL_MSGS, "PIO_CPDV_RAISE_SB_AMOUNT"), response);
}

static void test_checkPlayerDecisionValidity_RaisesInFixedGame_scenario3(CuTest* ct){
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
    char response[MESSAGES_MAX_MSG_LENGTH];

    bool result = checkPlayerDecisionValidity(_player, &state, &rules, decision, response);

    CuAssert(ct, response, !result);
    CuAssertStrEquals(ct, "The limit of raises per one betting round has already been reached.\n", response);
    CuAssertStrEquals(ct, msgGetn(GLOBAL_MSGS, "PIO_CPDV_RAISE_LIMIT"), response);
}

static void test_checkPlayerDecisionValidity_RaisesInFixedGame_scenario4(CuTest* ct){
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
    char response[MESSAGES_MAX_MSG_LENGTH];

    bool result = checkPlayerDecisionValidity(_player, &state, &rules, decision, response);

    CuAssert(ct, response, !result);
    CuAssertStrEquals(ct, "You cannot afford to raise the bet by the required big blind amount.\n", response);
    CuAssertStrEquals(ct, msgGetn(GLOBAL_MSGS, "PIO_CPDV_RAISE_BB_AMOUNT"), response);
}

static void test_checkPlayerDecisionValidity_RaisesInNoLimitGame_scenario1(CuTest* ct){
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
    char response[MESSAGES_MAX_MSG_LENGTH];

    bool result = checkPlayerDecisionValidity(_player, &state, &rules, decision, response);

    CuAssert(ct, response, result);
    CuAssertStrEquals(ct, "", response);
}

static void test_checkPlayerDecisionValidity_RaisesInNoLimitGame_scenario2(CuTest* ct){
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
    char response[MESSAGES_MAX_MSG_LENGTH];

    bool result = checkPlayerDecisionValidity(_player, &state, &rules, decision, response);

    CuAssert(ct, response, !result);
    CuAssertStrEquals(ct, "You cannot lower the bet, it can only be raised up.\n", response);
    CuAssertStrEquals(ct, msgGetn(GLOBAL_MSGS, "PIO_CPDV_RAISE_TOLOWER"), response);
}

static void test_checkPlayerDecisionValidity_RaisesInNoLimitGame_scenario3(CuTest* ct){
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
    char response[MESSAGES_MAX_MSG_LENGTH];

    bool result = checkPlayerDecisionValidity(_player, &state, &rules, decision, response);

    CuAssert(ct, response, !result);
    CuAssertStrEquals(ct, "You cannot afford to raise the bet by the specified amount.\n", response);
    CuAssertStrEquals(ct, msgGetn(GLOBAL_MSGS, "PIO_CPDV_RAISE_GENERIC"), response);
}

static void test_checkPlayerDecisionValidity_TapOuts_scenario1(CuTest* ct){
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
    char response[MESSAGES_MAX_MSG_LENGTH];

    bool result = checkPlayerDecisionValidity(_player, &state, &rules, decision, response);

    CuAssert(ct, response, result);
    CuAssertStrEquals(ct, "", response);
}

static void test_checkPlayerDecisionValidity_TapOuts_scenario2(CuTest* ct){
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
    char response[MESSAGES_MAX_MSG_LENGTH];

    bool result = checkPlayerDecisionValidity(_player, &state, &rules, decision, response);

    CuAssert(ct, response, !result);
    char* expectedResponse = "You can still afford to call the current bet. You may not tap out just yet.\n";
    char* actualResponse = msgGetn(GLOBAL_MSGS, "PIO_CPDV_TAPOUT");
    //This CuAssert function seems to corrupt the actualResponse variable.
    //Investigeting it thoroughly with debugger had shown no anomalies before entering this function.
    //CuAssertStrEquals(ct, actualResponse, response);

    //If CuAssertStrEquals is called BEFORE the one with strcmp, the actualResponse char array will become corrupted at its end
    //Not sure why or how, since actualResponse is not passed into any CuTest function before
    //And yes, it does get corrupted once control returns from CuAssertStrEquals
    CuAssert(ct, "", strcmp(actualResponse, response) == 0);
    CuAssertStrEquals(ct, expectedResponse, response);
}


CuSuite* PlayerIoGetSuite() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, setup_globalMsgsInitialization);
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

