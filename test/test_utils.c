#include "CuTest.h"
#include "../src/utils.h"

void TestMathMax(CuTest* ct){
    int result = mathMax(6, -1, 0, 10, 20, 30, 40);
    CuAssert(ct, "", result == 40);
}

CuSuite* UtilsGetSuite() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, TestMathMax);
    return suite;
}
