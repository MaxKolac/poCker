//HARD TODO
#include <stdbool.h>
#include "CuTest.h"
#include "../src/playingcard.h"
#include "../src/playingcard_enums.h"

void test_playingcard_template(CuTest* ct){
    CuAssert(ct, "", false);
}

CuSuite* PlayingcardGetSuite(CuTest* ct){
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_playingcard_template);
    return suite;
}

