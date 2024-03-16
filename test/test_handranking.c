#include <stdbool.h>
#include "CuTest.h"
#include "../src/playingcard.h"

void test_handrankingPerform(CuTest* ct){
    struct PlayingCard deck[DECK_LENGTH];
    buildDeck(&deck, true);

    test_royalFlushPresent(ct, deck);
    test_royalFlushNotPresent(ct, deck);
    test_straightFlushPresent(ct, deck);
    test_straightFlushNotPresent(ct, deck);
    test_FOaKPresent(ct, deck);
    test_FOaKNotPresent(ct, deck);
    test_fullHousePresent(ct, deck);
    test_fullHouseNotPresent(ct, deck);
    test_flushPresent(ct, deck);
    test_flushNotPresent(ct, deck);
    test_straightPresent(ct, deck);
    test_straightNotPresent(ct, deck);
}

void test_royalFlushPresent(CuTest* ct, struct PlayingCard deck[]){
    struct PlayingCard* cards[] = {
        &deck[PIPS_PER_SUIT * DIAMONDS + SEVEN - 1], //Seven of Diamonds
        &deck[PIPS_PER_SUIT * CLUBS + KING - 1], //King of Clubs
        &deck[PIPS_PER_SUIT * CLUBS + ACE - 1], //Ace of Clubs
        &deck[PIPS_PER_SUIT * HEARTS + TEN - 1], //Ten of Hearts
        &deck[PIPS_PER_SUIT * CLUBS + TEN - 1], //Ten of Clubs
        &deck[PIPS_PER_SUIT * CLUBS + QUEEN - 1], //Queen of Clubs
        &deck[PIPS_PER_SUIT * CLUBS + JACK - 1], //Jack of Clubs
    };
    int result = detectRoyalFlush(cards, 7);
    CuAssert(ct, "", result == 1);
}

void test_royalFlushNotPresent(CuTest* ct, struct PlayingCard deck[]){
    struct PlayingCard* cards[] = {
        &deck[PIPS_PER_SUIT * DIAMONDS + SEVEN - 1], //Seven of Diamonds
        &deck[PIPS_PER_SUIT * CLUBS + KING - 1], //King of Clubs
        &deck[PIPS_PER_SUIT * CLUBS + ACE - 1], //Ace of Clubs
        &deck[PIPS_PER_SUIT * HEARTS + TEN - 1], //Ten of Hearts
        &deck[PIPS_PER_SUIT * CLUBS + TEN - 1], //Ten of Clubs
        &deck[PIPS_PER_SUIT * SPADES + QUEEN - 1], //Queen of Spades
        &deck[PIPS_PER_SUIT * CLUBS + JACK - 1], //Jack of Clubs
    };
    int result = detectRoyalFlush(cards, 7);
    CuAssert(ct, "", result == 0);
}

void test_straightFlushPresent(CuTest* ct, struct PlayingCard deck[]){
    struct PlayingCard* cards1[] = {
        &deck[PIPS_PER_SUIT * DIAMONDS + EIGHT - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + TEN - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + NINE - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + SIX - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + SEVEN - 1]
    };
    struct PlayingCard* cards2[] = {
        &deck[PIPS_PER_SUIT * SPADES + TWO - 1],
        &deck[PIPS_PER_SUIT * CLUBS + THREE - 1],
        &deck[PIPS_PER_SUIT * SPADES + THREE - 1],
        &deck[PIPS_PER_SUIT * SPADES + FOUR - 1],
        &deck[PIPS_PER_SUIT * SPADES + SIX - 1],
        &deck[PIPS_PER_SUIT * SPADES + FIVE - 1]
    };
    struct PlayingCard* cards3[] = {
        &deck[PIPS_PER_SUIT * HEARTS + QUEEN - 1],
        &deck[PIPS_PER_SUIT * HEARTS + TEN - 1],
        &deck[PIPS_PER_SUIT * CLUBS + TEN - 1],
        &deck[PIPS_PER_SUIT * SPADES + THREE - 1],
        &deck[PIPS_PER_SUIT * HEARTS + JACK - 1],
        &deck[PIPS_PER_SUIT * HEARTS + EIGHT - 1],
        &deck[PIPS_PER_SUIT * HEARTS + NINE - 1]
    };
    int result1 = detectStraightFlush(cards1, 5);
    int result2 = detectStraightFlush(cards2, 6);
    int result3 = detectStraightFlush(cards3, 7);
    CuAssert(ct, "", result1 == TEN);
    CuAssert(ct, "", result2 == SIX);
    CuAssert(ct, "", result3 == QUEEN);
}

void test_straightFlushNotPresent(CuTest* ct, struct PlayingCard deck[]){
    struct PlayingCard* cards1[] = {
        &deck[PIPS_PER_SUIT * HEARTS + EIGHT - 1],
        &deck[PIPS_PER_SUIT * HEARTS + TEN - 1],
        &deck[PIPS_PER_SUIT * CLUBS + NINE - 1],
        &deck[PIPS_PER_SUIT * HEARTS + SEVEN - 1],
        &deck[PIPS_PER_SUIT * HEARTS + SIX - 1]
    };
    struct PlayingCard* cards2[] = {
        &deck[PIPS_PER_SUIT * DIAMONDS + FIVE - 1],
        &deck[PIPS_PER_SUIT * CLUBS + EIGHT - 1],
        &deck[PIPS_PER_SUIT * CLUBS + NINE - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + SEVEN - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + SIX - 1],
        &deck[PIPS_PER_SUIT * CLUBS + TEN - 1]
    };
    struct PlayingCard* cards3[] = {
        &deck[PIPS_PER_SUIT * SPADES + EIGHT - 1],
        &deck[PIPS_PER_SUIT * SPADES + TEN - 1],
        &deck[PIPS_PER_SUIT * SPADES + NINE - 1],
        &deck[PIPS_PER_SUIT * SPADES + SEVEN - 1],
        &deck[PIPS_PER_SUIT * SPADES + FIVE - 1],
        &deck[PIPS_PER_SUIT * SPADES + THREE - 1],
        &deck[PIPS_PER_SUIT * SPADES + FOUR - 1]
    };
    int result1 = detectStraightFlush(cards1, 5);
    int result2 = detectStraightFlush(cards2, 6);
    int result3 = detectStraightFlush(cards3, 7);
    CuAssert(ct, "", result1 == 0);
    CuAssert(ct, "", result2 == 0);
    CuAssert(ct, "", result3 == 0);
}

void test_FOaKPresent(CuTest* ct, struct PlayingCard deck[]){
    struct PlayingCard* cards1[] = {
        &deck[PIPS_PER_SUIT * HEARTS + FOUR - 1],
        &deck[PIPS_PER_SUIT * CLUBS + TEN - 1],
        &deck[PIPS_PER_SUIT * CLUBS + FOUR - 1],
        &deck[PIPS_PER_SUIT * SPADES + FOUR - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + FOUR - 1]
    };
    struct PlayingCard* cards2[] = {
        &deck[PIPS_PER_SUIT * HEARTS + FIVE - 1],
        &deck[PIPS_PER_SUIT * CLUBS + EIGHT - 1],
        &deck[PIPS_PER_SUIT * CLUBS + NINE - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + FIVE - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + SIX - 1],
        &deck[PIPS_PER_SUIT * CLUBS + EIGHT - 1],
        &deck[PIPS_PER_SUIT * SPADES + FIVE - 1],
        &deck[PIPS_PER_SUIT * SPADES + FIVE - 1],
        &deck[PIPS_PER_SUIT * CLUBS + EIGHT - 1],
        &deck[PIPS_PER_SUIT * CLUBS + EIGHT - 1]
    };
    struct PlayingCard* cards3[] = {
        &deck[PIPS_PER_SUIT * HEARTS + KING - 1],
        &deck[PIPS_PER_SUIT * SPADES + ACE - 1],
        &deck[PIPS_PER_SUIT * SPADES + TWO - 1],
        &deck[PIPS_PER_SUIT * HEARTS + TWO - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + ACE - 1],
        &deck[PIPS_PER_SUIT * SPADES + KING - 1],
        &deck[PIPS_PER_SUIT * HEARTS + ACE - 1],
        &deck[PIPS_PER_SUIT * CLUBS + TWO - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + TWO - 1],
        &deck[PIPS_PER_SUIT * CLUBS + KING - 1]
    };
    int result1 = detectFOaK(cards1, 5);
    int result2 = detectFOaK(cards2, 10);
    int result3 = detectFOaK(cards3, 10);
    CuAssert(ct, "", result1 == 309);
    CuAssert(ct, "", result2 == 708);
    CuAssert(ct, "", result3 == 113);
}

void test_FOaKNotPresent(CuTest* ct, struct PlayingCard deck[]){
    struct PlayingCard* cards1[] = {
        &deck[PIPS_PER_SUIT * HEARTS + FOUR - 1],
        &deck[PIPS_PER_SUIT * CLUBS + TEN - 1],
        &deck[PIPS_PER_SUIT * CLUBS + FOUR - 1],
        &deck[PIPS_PER_SUIT * SPADES + FIVE - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + FOUR - 1]
    };
    struct PlayingCard* cards2[] = {
        &deck[PIPS_PER_SUIT * HEARTS + FIVE - 1],
        &deck[PIPS_PER_SUIT * CLUBS + EIGHT - 1],
        &deck[PIPS_PER_SUIT * CLUBS + NINE - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + ACE - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + SIX - 1],
        &deck[PIPS_PER_SUIT * CLUBS + EIGHT - 1],
        &deck[PIPS_PER_SUIT * SPADES + ACE - 1],
        &deck[PIPS_PER_SUIT * SPADES + FIVE - 1],
        &deck[PIPS_PER_SUIT * CLUBS + EIGHT - 1],
        &deck[PIPS_PER_SUIT * CLUBS + FIVE - 1]
    };
    struct PlayingCard* cards3[] = {
        &deck[PIPS_PER_SUIT * HEARTS + KING - 1],
        &deck[PIPS_PER_SUIT * SPADES + ACE - 1],
        &deck[PIPS_PER_SUIT * SPADES + THREE - 1],
        &deck[PIPS_PER_SUIT * HEARTS + TWO - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + ACE - 1],
        &deck[PIPS_PER_SUIT * SPADES + KING - 1],
        &deck[PIPS_PER_SUIT * HEARTS + ACE - 1],
        &deck[PIPS_PER_SUIT * CLUBS + TWO - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + TWO - 1],
        &deck[PIPS_PER_SUIT * CLUBS + KING - 1]
    };
    int result1 = detectFOaK(cards1, 5);
    int result2 = detectFOaK(cards2, 10);
    int result3 = detectFOaK(cards3, 10);
    CuAssert(ct, "", result1 == 0);
    CuAssert(ct, "", result2 == 0);
    CuAssert(ct, "", result3 == 0);
}

void test_fullHousePresent(CuTest* ct, struct PlayingCard* deck[]){
    struct PlayingCard* cards1[] = {
        &deck[PIPS_PER_SUIT * HEARTS + FOUR - 1],
        &deck[PIPS_PER_SUIT * CLUBS + ACE - 1],
        &deck[PIPS_PER_SUIT * SPADES + FOUR - 1],
        &deck[PIPS_PER_SUIT * SPADES + ACE - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + FOUR - 1]
    };
    struct PlayingCard* cards2[] = {
        &deck[PIPS_PER_SUIT * HEARTS + FIVE - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + FIVE - 1],
        &deck[PIPS_PER_SUIT * HEARTS + EIGHT - 1],
        &deck[PIPS_PER_SUIT * SPADES + FIVE - 1],
        &deck[PIPS_PER_SUIT * HEARTS + TEN - 1],
        &deck[PIPS_PER_SUIT * CLUBS + TEN - 1],
        &deck[PIPS_PER_SUIT * CLUBS + FIVE - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + TEN - 1]
    };
    //Check it properly detects the highest possible Full House
    struct PlayingCard* cards3[] = {
        &deck[PIPS_PER_SUIT * HEARTS + KING - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + KING - 1],
        &deck[PIPS_PER_SUIT * SPADES + KING - 1],
        &deck[PIPS_PER_SUIT * CLUBS + QUEEN - 1],
        &deck[PIPS_PER_SUIT * SPADES + QUEEN - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + TWO - 1],
        &deck[PIPS_PER_SUIT * HEARTS + TWO - 1],
        &deck[PIPS_PER_SUIT * CLUBS + KING - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + QUEEN - 1],
        &deck[PIPS_PER_SUIT * HEARTS + QUEEN - 1]
    };
    int result1 = detectFullHouse(cards1, 5);
    int result2 = detectFullHouse(cards2, 8);
    int result3 = detectFullHouse(cards3, 10);
    CuAssert(ct, "", result1 == 313);
    CuAssert(ct, "", result2 == 904);
    CuAssert(ct, "", result3 == 1211);
}

void test_fullHouseNotPresent(CuTest* ct, struct PlayingCard* deck[]){
    //Check that a simple TOaK is not treated as Full House
    struct PlayingCard* cards1[] = {
        &deck[PIPS_PER_SUIT * HEARTS + KING - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + KING - 1],
        &deck[PIPS_PER_SUIT * SPADES + KING - 1]
    };
    int result1 = detectFullHouse(cards1, 3);
    CuAssert(ct, "", result1 == 0);
}

void test_flushPresent(CuTest* ct, struct PlayingCard* deck[]){
    //Check for a simple Flush
    struct PlayingCard* cards1[] = {
        &deck[PIPS_PER_SUIT * HEARTS + KING - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + KING - 1],
        &deck[PIPS_PER_SUIT * HEARTS + SEVEN - 1],
        &deck[PIPS_PER_SUIT * HEARTS + NINE - 1],
        &deck[PIPS_PER_SUIT * CLUBS + THREE - 1],
        &deck[PIPS_PER_SUIT * HEARTS + JACK - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + EIGHT - 1],
        &deck[PIPS_PER_SUIT * HEARTS + FOUR - 1]
        //Score = 12 * 20^4 + 10 * 20^3 + 8 * 20^2 + 6 * 20 + 3
        //Score = 12 * 160 000 + 10 * 8 000 + 8 * 400 + 6 * 20 + 3
        //Score = 1 920 000 + 80 000 + 3 200 + 120 + 3
        //Score = 2 003 323
    };
    //Check that the better Flush has higher priority when both Flushes share the same suit
    struct PlayingCard* cards2[] = {
        &deck[PIPS_PER_SUIT * CLUBS + ACE - 1],
        &deck[PIPS_PER_SUIT * CLUBS + KING - 1],
        &deck[PIPS_PER_SUIT * CLUBS + QUEEN - 1],
        &deck[PIPS_PER_SUIT * CLUBS + JACK - 1],
        &deck[PIPS_PER_SUIT * CLUBS + TEN - 1],
        &deck[PIPS_PER_SUIT * CLUBS + EIGHT - 1],
        &deck[PIPS_PER_SUIT * CLUBS + SIX - 1]
        //Score = 13 * 160 000 + 12 * 8 000 + 11 * 400 + 10 * 20 + 9
        //Score = 2 080 000 + 96 000 + 4400 + 200 + 9
        //Score = 2 180 609
    };
    //Check that the better Flush has higher priority when both Flushes have different suits
    struct PlayingCard* cards3[] = {
        &deck[PIPS_PER_SUIT * DIAMONDS + NINE - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + EIGHT - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + SIX - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + FIVE - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + THREE - 1],
        &deck[PIPS_PER_SUIT * CLUBS + JACK - 1],
        &deck[PIPS_PER_SUIT * CLUBS + TEN - 1],
        &deck[PIPS_PER_SUIT * CLUBS + EIGHT - 1],
        &deck[PIPS_PER_SUIT * CLUBS + SEVEN - 1],
        &deck[PIPS_PER_SUIT * CLUBS + SIX - 1]
        //Score = 10 * 160 000 + 9 * 8 000 + 7 * 400 + 6 * 20 + 5
        //Score = 1 600 000 + 72 000 + 2 800 + 120 + 5
        //Score = 1 674 925
    };
    int result1 = detectFlush(cards1, 8);
    int result2 = detectFlush(cards2, 7);
    int result3 = detectFlush(cards3, 10);
    CuAssert(ct, "", result1 == 2003323);
    CuAssert(ct, "", result2 == 2180609);
    CuAssert(ct, "", result3 == 1674925);
}

void test_flushNotPresent(CuTest* ct, struct PlayingCard* deck[]){
    //Duplicate cards do not count towards a Flush
    struct PlayingCard* cards1[] = {
        &deck[PIPS_PER_SUIT * HEARTS + KING - 1],
        &deck[PIPS_PER_SUIT * HEARTS + JACK - 1],
        &deck[PIPS_PER_SUIT * HEARTS + NINE - 1],
        &deck[PIPS_PER_SUIT * HEARTS + NINE - 1],
        &deck[PIPS_PER_SUIT * HEARTS + SEVEN - 1],
        &deck[PIPS_PER_SUIT * HEARTS + FOUR - 1]
    };
    //No Flush present returns a zero
    struct PlayingCard* cards2[] = {
        &deck[PIPS_PER_SUIT * HEARTS + KING - 1],
        &deck[PIPS_PER_SUIT * HEARTS + JACK - 1],
        &deck[PIPS_PER_SUIT * HEARTS + TEN - 1],
        &deck[PIPS_PER_SUIT * HEARTS + NINE - 1],
        &deck[PIPS_PER_SUIT * SPADES + SEVEN - 1],
        &deck[PIPS_PER_SUIT * SPADES + FOUR - 1],
        &deck[PIPS_PER_SUIT * SPADES + FIVE - 1],
        &deck[PIPS_PER_SUIT * SPADES + QUEEN - 1]
    };
    int result1 = detectFlush(cards1, 6);
    int result2 = detectFlush(cards2, 8);
    CuAssert(ct, "", result1 == 0);
    CuAssert(ct, "", result2 == 0);
}

void test_straightPresent(CuTest* ct, struct PlayingCard* deck[]){
    //Detect a simple Straight from among a mixed hand of cards
    struct PlayingCard* cards1[] = {
        &deck[PIPS_PER_SUIT * SPADES + SEVEN - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + SIX - 1],
        &deck[PIPS_PER_SUIT * HEARTS + ACE - 1],
        &deck[PIPS_PER_SUIT * CLUBS + EIGHT - 1],
        &deck[PIPS_PER_SUIT * HEARTS + FOUR - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + FIVE - 1]
    };
    //Make sure that multiple cards of the same value don't break the consecutivity
    struct PlayingCard* cards2[] = {
        &deck[PIPS_PER_SUIT * HEARTS + KING - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + QUEEN - 1],
        &deck[PIPS_PER_SUIT * SPADES + QUEEN - 1],
        &deck[PIPS_PER_SUIT * HEARTS + QUEEN - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + JACK - 1],
        &deck[PIPS_PER_SUIT * CLUBS + TEN - 1],
        &deck[PIPS_PER_SUIT * SPADES + NINE - 1]
    };
    //Make sure that multiple duplicates don't cause the incorrect card being returned
    struct PlayingCard* cards3[] = {
        &deck[PIPS_PER_SUIT * DIAMONDS + KING - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + QUEEN - 1],
        &deck[PIPS_PER_SUIT * SPADES + QUEEN - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + JACK - 1],
        &deck[PIPS_PER_SUIT * SPADES + JACK - 1],
        &deck[PIPS_PER_SUIT * CLUBS + JACK - 1],
        &deck[PIPS_PER_SUIT * CLUBS + TEN - 1],
        &deck[PIPS_PER_SUIT * HEARTS + TEN - 1],
        &deck[PIPS_PER_SUIT * SPADES + NINE - 1]
    };
    //Detect the highest card of a Straight
    struct PlayingCard* cards4[] = {
        &deck[PIPS_PER_SUIT * SPADES + FOUR - 1],
        &deck[PIPS_PER_SUIT * SPADES + TWO - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + SIX - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + THREE - 1],
        &deck[PIPS_PER_SUIT * SPADES + EIGHT - 1],
        &deck[PIPS_PER_SUIT * HEARTS + FIVE - 1],
        &deck[PIPS_PER_SUIT * CLUBS + SEVEN - 1],
        &deck[PIPS_PER_SUIT * CLUBS + NINE - 1]
    };
    int result1 = detectStraight(cards1, 6);
    int result2 = detectStraight(cards2, 7);
    int result3 = detectStraight(cards3, 9);
    int result4 = detectStraight(cards4, 8);
    CuAssert(ct, "", result1 == EIGHT);
    CuAssert(ct, "", result2 == KING);
    CuAssert(ct, "", result3 == KING);
    CuAssert(ct, "", result4 == NINE);
}

void test_straightNotPresent(CuTest* ct, struct PlayingCard* deck[]){
    //5 cards of the same value does not constitute a Straight
    struct PlayingCard* cards1[] = {
        &deck[PIPS_PER_SUIT * DIAMONDS + FOUR - 1],
        &deck[PIPS_PER_SUIT * CLUBS + FOUR - 1],
        &deck[PIPS_PER_SUIT * HEARTS + FOUR - 1],
        &deck[PIPS_PER_SUIT * SPADES + FOUR - 1],
        &deck[PIPS_PER_SUIT * SPADES + FOUR - 1],
    };
    int result1 = detectStraight(cards1, 5);
    CuAssert(ct, "", result1 == 0);
}

void test_TOaKPresent(CuTest* ct, struct PlayingCard* deck[]){
    //Detect a simple TOaK
    struct PlayingCard* cards1[] = {
        &deck[PIPS_PER_SUIT * CLUBS + FOUR - 1],
        &deck[PIPS_PER_SUIT * HEARTS + FOUR - 1],
        &deck[PIPS_PER_SUIT * SPADES + FOUR - 1]
    };
    //Detect the highest TOaK
    struct PlayingCard* cards2[] = {
        &deck[PIPS_PER_SUIT * DIAMONDS + FOUR - 1],
        &deck[PIPS_PER_SUIT * CLUBS + FOUR - 1],
        &deck[PIPS_PER_SUIT * HEARTS + FIVE - 1],
        &deck[PIPS_PER_SUIT * SPADES + FOUR - 1],
        &deck[PIPS_PER_SUIT * CLUBS + FIVE - 1],
        &deck[PIPS_PER_SUIT * SPADES + FIVE - 1]
    };
    int result1 = detectTOaK(cards1, 3);
    int result2 = detectTOaK(cards2, 6);
    CuAssert(ct, "", result1 == FOUR);
    CuAssert(ct, "", result2 == FIVE);
}

void test_TOaKNotPresent(CuTest* ct, struct PlayingCard* deck[]){
    //For now, just make sure the PC does not explode
    struct PlayingCard* cards1[] = {
        &deck[PIPS_PER_SUIT * DIAMONDS + FOUR - 1],
        &deck[PIPS_PER_SUIT * CLUBS + THREE - 1],
        &deck[PIPS_PER_SUIT * HEARTS + FIVE - 1],
        &deck[PIPS_PER_SUIT * SPADES + FOUR - 1],
        &deck[PIPS_PER_SUIT * CLUBS + FIVE - 1],
        &deck[PIPS_PER_SUIT * SPADES + ACE - 1]
    };
    int result1 = detectTOaK(cards1, 6);
    CuAssert(ct, "", result1 == 0);
}

CuSuite* HandrankingGetSuite(){
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_handrankingPerform);
    return suite;
}
