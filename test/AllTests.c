#include "CuTest.h"

//CuSuite* StrUtilGetSuite();
CuSuite* UtilsGetSuite();
CuSuite* HandrankingGetSuite();

void RunAllTests(void) {
    CuString *output = CuStringNew();
    CuSuite* suite = CuSuiteNew();

    //CuSuiteAddSuite(suite, StrUtilGetSuite());
    CuSuiteAddSuite(suite, UtilsGetSuite());
    CuSuiteAddSuite(suite, HandrankingGetSuite());
    CuSuiteAddSuite(suite, DealerGetSuite());

    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);
    printf("%s\n", output->buffer);
}

int main(void) {
    RunAllTests();
}
