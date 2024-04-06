#include <stdbool.h>
#include <stdio.h>
#include "CuTest.h"
#include "../src/playingcard.h"
#include "../src/playingcard_enums.h"

void test_playingcard_getCorrectCardName(CuTest* ct){
    const int buffer_size = 32;
    char actual[buffer_size];
    char expected[buffer_size];

    for (int s = 0; s < SUITS_COUNT; ++s){
        for (int p = 1; p <= PIPS_PER_SUIT; ++p){
            sprintf(&expected, "%s of %s", getPipName(p), getSuitName(s));
            PlayingCard card = {
                .suit = s,
                .pips = p
            };
            getCardName(&card, &actual, buffer_size);
            CuAssert(ct, &actual, strcmp(&actual, &expected) == 0);
            CuAssert(ct, &actual, strstr(&actual, "ERR") == NULL);
        }
    }
}

void test_playingcard_getCorrectPipName(CuTest* ct){
    enum Pip all_pips[PIPS_PER_SUIT] = {
        TWO,
        THREE,
        FOUR,
        FIVE,
        SIX,
        SEVEN,
        EIGHT,
        NINE,
        TEN,
        JACK,
        QUEEN,
        KING,
        ACE
    };
    const char pip_names[PIPS_PER_SUIT][6] = {
        "Two",
        "Three",
        "Four",
        "Five",
        "Six",
        "Seven",
        "Eight",
        "Nine",
        "Ten",
        "Jack",
        "Queen",
        "King",
        "Ace"
    };

    for (int i = 0; i < PIPS_PER_SUIT; ++i){
        char buffer[10];
        strcpy(&buffer, getPipName(all_pips[i]));
        CuAssert(ct, &buffer, strcmp(&buffer, &pip_names[i][0]) == 0);
    }
}

void test_playingcard_getCorrectSuitName(CuTest* ct){
    enum Suit all_suits[SUITS_COUNT] = {
        HEARTS,
        DIAMONDS,
        CLUBS,
        SPADES
    };
    const char suit_names[SUITS_COUNT][9] = {
        "Hearts",
        "Diamonds",
        "Clubs",
        "Spades"
    };

    for (int i = 0; i < SUITS_COUNT; ++i){
        char buffer[10];
        strcpy(&buffer, getSuitName(all_suits[i]));
        CuAssert(ct, &buffer, strcmp(&buffer, &suit_names[i][0]) == 0);
    }
}

void test_playingcard_getErrsOnIncorrectParams(CuTest* ct){
    char buffer[10];
    strcpy(&buffer, getPipName(0));
    CuAssert(ct, &buffer, strcmp(&buffer, "ERR") == 0);

    strcpy(&buffer, getPipName(PIPS_PER_SUIT + 1));
    CuAssert(ct, &buffer, strcmp(&buffer, "ERR") == 0);

    strcpy(&buffer, getSuitName(-1));
    CuAssert(ct, &buffer, strcmp(&buffer, "ERR") == 0);

    strcpy(&buffer, getSuitName(SUITS_COUNT));
    CuAssert(ct, &buffer, strcmp(&buffer, "ERR") == 0);
}

CuSuite* PlayingcardGetSuite(CuTest* ct){
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_playingcard_getCorrectCardName);
    SUITE_ADD_TEST(suite, test_playingcard_getCorrectPipName);
    SUITE_ADD_TEST(suite, test_playingcard_getCorrectSuitName);
    SUITE_ADD_TEST(suite, test_playingcard_getErrsOnIncorrectParams);
    return suite;
}

