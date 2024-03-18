#include "CuTest.h"
#include "../src/utils.h"

void test_mathMax(CuTest* ct){
    int result = mathMax(6, -1, 0, 10, 20, 30, 40);
    CuAssert(ct, "", result == 40);
}

void test_mathMin(CuTest* ct){
    int result = mathMin(5, -5, -4, 0, 1, 2);
    CuAssert(ct, "", result == -5);
}

void test_mathClamp(CuTest* ct){
    //val, min, max, expectedResult
    int values[][4] = {
        { -2, 2, 4, 2 },
        { 10, 0, 6, 6 },
        { 12,24,26,24 },
        { 30,24,26,26 },
        { -2,-4, 4,-2 }
    };
    for (int i = 0; i < 5; i++){
        CuAssert(ct, "", mathClamp(values[i][0], values[i][1], values[i][2]) == values[i][3]);
    }
}

CuSuite* UtilsGetSuite() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_mathMax);
    SUITE_ADD_TEST(suite, test_mathMin);
    SUITE_ADD_TEST(suite, test_mathClamp);
    return suite;
}
