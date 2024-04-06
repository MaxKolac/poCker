#include <stdio.h>
#include "CuTest.h"

CuSuite* DealerGetSuite();
CuSuite* GamerulesGetSuite();
CuSuite* GamestateGetSuite();
CuSuite* HandrankingGetSuite();
CuSuite* PlayerGetSuite();
CuSuite* PlayingcardGetSuite();
CuSuite* UtilsGetSuite();

void RunAllTests(void) {
    CuString *output = CuStringNew();
    CuSuite* suite = CuSuiteNew();

    CuSuiteAddSuite(suite, DealerGetSuite());
    CuSuiteAddSuite(suite, GamerulesGetSuite());
    CuSuiteAddSuite(suite, GamestateGetSuite());
    CuSuiteAddSuite(suite, HandrankingGetSuite());
    CuSuiteAddSuite(suite, PlayerGetSuite());
    CuSuiteAddSuite(suite, PlayingcardGetSuite());
    CuSuiteAddSuite(suite, UtilsGetSuite());

    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);
    printf("%s\n", output->buffer);
}

int main(void) {
    RunAllTests();
}
