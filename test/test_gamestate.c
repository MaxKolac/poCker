//HARD TODO
#include "CuTest.h"
#include "../src/gamestate.h"

void test_gamestate_template(CuTest* ct){
    CuAssert(ct, "", false);
}

CuSuite* GamestateGetSuite(CuTest* ct){
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_gamestate_template);
    return suite;
}
