#include "CuTest.h"
#include "../src/player.h"

static void test_player_checkConstructor(CuTest* ct){
    Player* player = playerCreateNew();
    CuAssert(ct, "", !player->folded);
    CuAssert(ct, "", !player->tappedout);
    CuAssert(ct, "", player->funds == 0);
    CuAssert(ct, "", !player->isHuman);
    for (int i = 0; i < SCORE_TABLE_SIZE; ++i){
        CuAssert(ct, "", player->scores[i] == 0);
    }
    for (int i = 0; i < CARDS_PER_PLAYER; ++i){
        CuAssert(ct, "", player->current_hand[i] == NULL);
    }
}

static void test_player_checkConstructorWithParameters(CuTest* ct){
    const int funds = 1000;
    Player* player = playerCreateNewWithFunds(funds);
    CuAssert(ct, "", !player->folded);
    CuAssert(ct, "", !player->tappedout);
    CuAssert(ct, "", player->funds == funds);
    CuAssert(ct, "", !player->isHuman);
    for (int i = 0; i < SCORE_TABLE_SIZE; ++i){
        CuAssert(ct, "", player->scores[i] == 0);
    }
    for (int i = 0; i < CARDS_PER_PLAYER; ++i){
        CuAssert(ct, "", player->current_hand[i] == NULL);
    }
}

static void test_player_checkResettingScoresWorks(CuTest* ct){
    Player* player = playerCreateNew();
    player->scores[0] = 1;
    player->scores[1] = 100;
    player->scores[4] = -100;
    player->scores[5] = 9;
    player->scores[6] = 42;
    player->scores[8] = 190;
    player->scores[9] = 1;

    resetScores(player);

    for (int i = 0; i < SCORE_TABLE_SIZE; ++i){
        CuAssert(ct, "", player->scores[i] == 0);
    }
}

CuSuite* PlayerGetSuite(CuTest* ct){
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_player_checkConstructor);
    SUITE_ADD_TEST(suite, test_player_checkConstructorWithParameters);
    SUITE_ADD_TEST(suite, test_player_checkResettingScoresWorks);
    return suite;
}
