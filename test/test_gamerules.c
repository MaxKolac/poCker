#include "CuTest.h"
#include "../src/gamerules.h"

void test_gamerules_template(CuTest* ct){
    CuAssert(ct, "", false);
}

CuSuite* GamerulesGetSuite(CuTest* ct){
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_gamerules_template);
    return suite;
}
