#include <limits.h>
#include "CuTest.h"
#include "../src/io.h"

void test_decisionRecognition_callCheck(CuTest* ct){
    char input1[] = "call";
    int result = recognizeDecision(input1);
    CuAssert(ct, "", result == 0);

    char input2[] = "check";
    result = recognizeDecision(input2);
    CuAssert(ct, "", result == 0);

    char input3[] = "check  ";
    result = recognizeDecision(input3);
    CuAssert(ct, "", result == 0);

    char input4[] = "check 1000";
    result = recognizeDecision(input4);
    CuAssert(ct, "", result == 0);

    char input5[] = "Check";
    result = recognizeDecision(input5);
    CuAssert(ct, "", result == 0);

    char input6[] = "  check check chec";
    int result6 = recognizeDecision(input6);
    CuAssert(ct, "", result6 == 0);
}

void test_decisionRecognition_folding(CuTest* ct){
    char input1[] = "fold";
    int result = recognizeDecision(input1);
    CuAssert(ct, "", result == -1);

    char input2[] = "fold out";
    result = recognizeDecision(input2);
    CuAssert(ct, "", result == -1);

    char input3[] = "fold 20";
    result = recognizeDecision(input3);
    CuAssert(ct, "", result == -1);

    char input4[] = "FOld";
    result = recognizeDecision(input4);
    CuAssert(ct, "", result == -1);
}

void test_decisionRecognition_tapouts(CuTest* ct){
    char input1[] = "tapout";
    int result = recognizeDecision(input1);
    CuAssert(ct, "", result == -2);

    char input2[] = "tapout 100";
    result = recognizeDecision(input2);
    CuAssert(ct, "", result == -2);

    char input3[] = "TApOut";
    result = recognizeDecision(input3);
    CuAssert(ct, "", result == -2);
}

void test_decisionRecognition_raises(CuTest* ct){
    char input1[] = "raise 100";
    int result = recognizeDecision(input1);
    CuAssert(ct, "", result == 100);

    char input2[] = "RAise";
    result = recognizeDecision(input2);
    CuAssert(ct, "", result == 1);

    char input3[] = "Raise -100";
    result = recognizeDecision(input3);
    CuAssert(ct, "", result == 1);

    char input4[] = "raise 125 25";
    result = recognizeDecision(input4);
    CuAssert(ct, "", result == 125);
}

void test_decisionRecognition_invalidInputs(CuTest* ct){
    char input1[] = "tap out";
    int result1 = recognizeDecision(input1);
    CuAssert(ct, "", result1 == INT_MIN);

    char input2[] = "ca ll";
    int result2 = recognizeDecision(input2);
    CuAssert(ct, "", result2 == INT_MIN);

    char input3[] = "flod";
    int result3 = recognizeDecision(input3);
    CuAssert(ct, "", result3 == INT_MIN);

    char input4[] = "";
    int result4 = recognizeDecision(input4);
    CuAssert(ct, "", result4 == INT_MIN);

    char input5[] = "     ";
    int result5 = recognizeDecision(input5);
    CuAssert(ct, "", result5 == INT_MIN);

    char input6[] = " ,  ";
    int result6 = recognizeDecision(input6);
    CuAssert(ct, "", result6 == INT_MIN);

    char input7[] = "100";
    int result7 = recognizeDecision(input7);
    CuAssert(ct, "", result7 == INT_MIN);
}

CuSuite* IoGetSuite() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_decisionRecognition_callCheck);
    SUITE_ADD_TEST(suite, test_decisionRecognition_folding);
    SUITE_ADD_TEST(suite, test_decisionRecognition_tapouts);
    SUITE_ADD_TEST(suite, test_decisionRecognition_raises);
    SUITE_ADD_TEST(suite, test_decisionRecognition_invalidInputs);
    return suite;
}

