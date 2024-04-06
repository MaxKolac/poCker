#include "CuTest.h"
#include "../src/player.h"

void test_player_checkConstructor(CuTest* ct){
    Player* player = playerCreateNew();
    CuAssert(ct, "Player starts with folded status", !player->folded);
    CuAssert(ct, "Player starts with nonzero funds", player->funds == 0);
    CuAssert(ct, "Player starts marked as human", !player->isHuman);
    for (int i = 0; i < SCORE_TABLE_SIZE; ++i){
        CuAssert(ct, "", player->scores[i] == 0);
    }
    for (int i = 0; i < CARDS_PER_PLAYER; ++i){
        CuAssert(ct, "", player->current_hand[i] == NULL);
    }
}

void test_player_checkResettingScoresWorks(CuTest* ct){
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
    SUITE_ADD_TEST(suite, test_player_checkResettingScoresWorks);
    return suite;
}
