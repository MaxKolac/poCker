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

void test_checkPlayerDecisionValidity(CuTest* ct){
    CuAssert(ct, "TODO", false);
}

CuSuite* DealerGetSuite(){
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_checkPlayerDecisionValidity);
    SUITE_ADD_TEST(suite, test_decideWinners_obviousWinner);
    SUITE_ADD_TEST(suite, test_decideWinners_onePlayerHasGreaterScoreButInLowerTier);
    SUITE_ADD_TEST(suite, test_decideWinners_multiplePlayerSameScore);
    SUITE_ADD_TEST(suite, test_decideWinners_correctlySelectWinner);
    SUITE_ADD_TEST(suite, test_decideWinners_largerScoreInLowerTiersDontWin);
    SUITE_ADD_TEST(suite, test_decideWinners_miscTest1);
    return suite;
}
