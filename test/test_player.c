//HARD TODO
#include "CuTest.h"
#include "../src/player.h"

void test_player_template(CuTest* ct){
    CuAssert(ct, "", false);
}

CuSuite* PlayerGetSuite(CuTest* ct){
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_player_template);
    return suite;
}
