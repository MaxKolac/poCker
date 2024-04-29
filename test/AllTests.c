#include <stdio.h>
#include "CuTest.h"

CuSuite* DealerGetSuite();
CuSuite* GamestateGetSuite();
CuSuite* HandrankingGetSuite();
CuSuite* MessagesGetSuite();
CuSuite* PlayerIoGetSuite();
CuSuite* PlayerGetSuite();
CuSuite* PlayingcardGetSuite();
CuSuite* UtilsGetSuite();

void RunAllTests(void) {
    CuString *output = CuStringNew();
    CuSuite* suite = CuSuiteNew();

    CuSuiteAddSuite(suite, DealerGetSuite());
    CuSuiteAddSuite(suite, GamestateGetSuite());
    CuSuiteAddSuite(suite, HandrankingGetSuite());
    CuSuiteAddSuite(suite, MessagesGetSuite());
    CuSuiteAddSuite(suite, PlayerIoGetSuite());
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
