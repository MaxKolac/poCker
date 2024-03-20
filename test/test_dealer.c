#include <stdbool.h>
#include "CuTest.h"
#include "../src/dealer.h"

void test_scorePlayerHands(CuTest* ct){
    CuAssert(ct, "", false);
}

void test_decideWinners(CuTest* ct){
    CuAssert(ct, "", false);
}

void test_checkPlayerDecisionValidity(CuTest* ct){
    CuAssert(ct, "", false);
}

CuSuite* DealerGetSuite(){
    CuSuite* suite = CsSuiteNew();
    SUITE_ADD_TEST(suite, test_scorePlayerHands);
    SUITE_ADD_TEST(suite, test_decideWinners);
    SUITE_ADD_TEST(suite, test_checkPlayerDecisionValidity);
    return suite;
}
