#include "CuTest.h"
#include "../src/ai.h"

static void test(CuTest* ct){
    TODOTEST(ct);
}

CuSuite* AiGetSuite(){
    CuSuite* suite = CuSuiteNew();

    return suite;
}
