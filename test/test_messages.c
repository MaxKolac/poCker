#include "CuTest.h"
#include "../src/messages.h"
#include "../src/playingcard.h"
#include "../src/gamestate.h"
#include "../src/gamerules.h"
#include <stdbool.h>
#include <stdio.h>

static void setup_globalMsgsInitialization(CuTest* ct){
    msgInitFromFile(MESSAGES_FILENAME);
    for (int i = 0; i < MESSAGES_COUNT; ++i){
        CuAssert(ct, "", strcmp(GLOBAL_MSGS[i].key, "") != 0);
        CuAssert(ct, "", strcmp(GLOBAL_MSGS[i].message, "") != 0);
    }
}

static void test_invalidKeyReturnsErrorMsg(CuTest* ct){
    char* key = "someBogusKey";
    char* expected = MESSAGES_NOKEYFOUND;
    char* actual = msgGet(GLOBAL_MSGS, key);
    CuAssertStrEquals(ct, expected, actual);
}

static void test_retrieveTestString(CuTest* ct){
    char* expected = "Hello world!";
    char* actual = msgGet(GLOBAL_MSGS, "TEST");
    CuAssertStrEquals(ct, expected, actual);
}

static void test_retrieveParametrizedTestString(CuTest* ct){
    char* expected = "This is a digit %d";
    char* actual = msgGet(GLOBAL_MSGS, "TESTPARAM");
    CuAssertStrEquals(ct, expected, actual);

    char* expectedWithParam = "This is a digit 12345";
    char actualWithParam[MESSAGES_MAX_MSG_LENGTH];
    snprintf(actualWithParam, MESSAGES_MAX_MSG_LENGTH, expected, 12345);
    CuAssertStrEquals(ct, expectedWithParam, actualWithParam);
}

//This is less of a unit-test and more of a ready-to-use function
//Uncomment the line that adds it to the suit, change variables to suit your needs
//And check the test project's console output for any incorrect behaviours
static void test_fullPrinting(CuTest* ct){
    //Full game initialization
    msgInitFromFile("loc.txt");
    const PlayingCard deck[DECK_LENGTH];
    buildDeck(deck, false);
    const GameRuleSet rules = {
        //Adjust gamerules here
        .player_count = 10,
        .ai_player_count = 6,
        .funds_per_player = 1200,
        .limit_fixed = false,
        .big_blind = 4,
        .small_blind = 2
    };
    GameState* state = gsCreateNew(&rules);
    Player* players[rules.player_count];
    for (int i = 0; i < rules.player_count; i++){
        players[i] = playerCreateNewWithFunds(rules.funds_per_player);
    }
    for (int i = rules.ai_player_count; i < rules.player_count; i++){
        players[i]->isHuman = true;
    }
    PlayingCard* comm_cards[COMM_CARDS_COUNT];
    distributeCards(deck, players, comm_cards, &rules);

    //Change variables here
    players[5]->folded = true;
    players[3]->tappedout = true;
    players[6]->funds = 1234567890;
    state->pot = 12 * 10000;
    state->betting_round = 1;
    state->revealed_comm_cards = 5;

    //Prints
    MSG_SHOWN(GLOBAL_MSGS, "DIVIDER_1COL");
    printHeader(state);
    MSG_SHOWN(GLOBAL_MSGS, "DIVIDER_2COL");
    printPlayers(&rules, state, players);
    MSG_SHOWN(GLOBAL_MSGS, "DIVIDER_2COL");
    printRaisesPotBet(&rules, state);
    MSG_SHOWN(GLOBAL_MSGS, "DIVIDER_1COL");
    printCards(players[0], comm_cards, state->revealed_comm_cards);
    MSG_SHOWN(GLOBAL_MSGS, "DIVIDER_1COL");
}

CuSuite* MessagesGetSuite(){
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, setup_globalMsgsInitialization);
    SUITE_ADD_TEST(suite, test_invalidKeyReturnsErrorMsg);
    SUITE_ADD_TEST(suite, test_retrieveTestString);
    SUITE_ADD_TEST(suite, test_retrieveParametrizedTestString);
    //SUITE_ADD_TEST(suite, test_fullPrinting);
    return suite;
}
