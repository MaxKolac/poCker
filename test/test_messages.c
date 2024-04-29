#include "CuTest.h"
#include "../src/messages.h"
#include <stdbool.h>

static void setup_globalMsgsInitialization(CuTest* ct){
    msgInitFromFile(MESSAGES_FILENAME);
    for (int i = 0; i < MESSAGES_COUNT; ++i){
        CuAssert(ct, "", strcmp(GLOBAL_MSGS[i].key, "") != 0);
        CuAssert(ct, "", strcmp(GLOBAL_MSGS[i].message, "") != 0);
    }
}

static void test_invalidKeyReturnsErrorMsg(CuTest* ct){
    char* key = "someBogusKey";
    char* expected[MESSAGES_MAX_MSG_LENGTH];
    snprintf(expected, MESSAGES_MAX_MSG_LENGTH, MESSAGES_NOKEYFOUND_FORMAT, key);
    char* actual = msgGet(GLOBAL_MSGS, key);
    CuAssertStrEquals(ct, expected, actual);
}

static void test_retrieveTestString(CuTest* ct){
    char* expected = "Hello world!";
    char* actual = msgGet(GLOBAL_MSGS, "TEST");
    CuAssertStrEquals(ct, expected, actual);
}

static void test_retrieveParametrizedTestString(CuTest* ct){
    char* expected = "This is a digit %d";
    char* actual = msgGet(GLOBAL_MSGS, "TESTPARAM");
    CuAssertStrEquals(ct, expected, actual);

    char* expectedWithParam = "This is a digit 12345";
    char actualWithParam[MESSAGES_MAX_MSG_LENGTH];
    snprintf(actualWithParam, MESSAGES_MAX_MSG_LENGTH, expected, 12345);
    CuAssertStrEquals(ct, expectedWithParam, actualWithParam);
}

CuSuite* MessagesGetSuite(){
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, setup_globalMsgsInitialization);
    SUITE_ADD_TEST(suite, test_invalidKeyReturnsErrorMsg);
    SUITE_ADD_TEST(suite, test_retrieveTestString);
    SUITE_ADD_TEST(suite, test_retrieveParametrizedTestString);
    return suite;
}
