#include <math.h>
#include <stdbool.h>
#include "CuTest.h"
#include "../src/playingcard.h"

static void test_royalFlushPresent(CuTest* ct){
    PlayingCard deck[DECK_LENGTH];
    buildDeck(&deck, false);
    PlayingCard* cards[] = {
        &deck[PIPS_PER_SUIT * DIAMONDS + SEVEN - 1], //Seven of Diamonds
        &deck[PIPS_PER_SUIT * CLUBS + KING - 1], //King of Clubs
        &deck[PIPS_PER_SUIT * CLUBS + ACE - 1], //Ace of Clubs
        &deck[PIPS_PER_SUIT * HEARTS + TEN - 1], //Ten of Hearts
        &deck[PIPS_PER_SUIT * CLUBS + TEN - 1], //Ten of Clubs
        &deck[PIPS_PER_SUIT * CLUBS + QUEEN - 1], //Queen of Clubs
        &deck[PIPS_PER_SUIT * CLUBS + JACK - 1], //Jack of Clubs
    };
    //Case found when doing tests for gamestate.h
    PlayingCard* cards2[] = {
        &deck[PIPS_PER_SUIT * DIAMONDS + KING - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + QUEEN - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + JACK - 1],
        &deck[PIPS_PER_SUIT * CLUBS + TWO - 1],
        &deck[PIPS_PER_SUIT * HEARTS + THREE - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + TEN - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + ACE - 1]
    };

    int result = detectRoyalFlush(cards, 7);
    int result2 = detectRoyalFlush(cards2, 7);

    CuAssert(ct, "", result == 1);
    CuAssert(ct, "", result2 == 1);
}

static void test_royalFlushNotPresent(CuTest* ct){
    PlayingCard deck[DECK_LENGTH];
    buildDeck(&deck, false);
    PlayingCard* cards[] = {
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

static void test_straightFlushPresent(CuTest* ct){
    PlayingCard deck[DECK_LENGTH];
    buildDeck(&deck, false);
    PlayingCard* cards1[] = {
        &deck[PIPS_PER_SUIT * DIAMONDS + EIGHT - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + TEN - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + NINE - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + SIX - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + SEVEN - 1]
    };
    PlayingCard* cards2[] = {
        &deck[PIPS_PER_SUIT * SPADES + TWO - 1],
        &deck[PIPS_PER_SUIT * CLUBS + THREE - 1],
        &deck[PIPS_PER_SUIT * SPADES + THREE - 1],
        &deck[PIPS_PER_SUIT * SPADES + FOUR - 1],
        &deck[PIPS_PER_SUIT * SPADES + SIX - 1],
        &deck[PIPS_PER_SUIT * SPADES + FIVE - 1]
    };
    PlayingCard* cards3[] = {
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

static void test_straightFlushNotPresent(CuTest* ct){
    PlayingCard deck[DECK_LENGTH];
    buildDeck(&deck, false);
    PlayingCard* cards1[] = {
        &deck[PIPS_PER_SUIT * HEARTS + EIGHT - 1],
        &deck[PIPS_PER_SUIT * HEARTS + TEN - 1],
        &deck[PIPS_PER_SUIT * CLUBS + NINE - 1],
        &deck[PIPS_PER_SUIT * HEARTS + SEVEN - 1],
        &deck[PIPS_PER_SUIT * HEARTS + SIX - 1]
    };
    PlayingCard* cards2[] = {
        &deck[PIPS_PER_SUIT * DIAMONDS + FIVE - 1],
        &deck[PIPS_PER_SUIT * CLUBS + EIGHT - 1],
        &deck[PIPS_PER_SUIT * CLUBS + NINE - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + SEVEN - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + SIX - 1],
        &deck[PIPS_PER_SUIT * CLUBS + TEN - 1]
    };
    PlayingCard* cards3[] = {
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

static void test_FOaKPresent(CuTest* ct){
    PlayingCard deck[DECK_LENGTH];
    buildDeck(&deck, false);
    PlayingCard* cards1[] = {
        &deck[PIPS_PER_SUIT * HEARTS + FOUR - 1],
        &deck[PIPS_PER_SUIT * CLUBS + TEN - 1],
        &deck[PIPS_PER_SUIT * CLUBS + FOUR - 1],
        &deck[PIPS_PER_SUIT * SPADES + FOUR - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + FOUR - 1]
    };
    PlayingCard* cards2[] = {
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
    PlayingCard* cards3[] = {
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

static void test_FOaKNotPresent(CuTest* ct){
    PlayingCard deck[DECK_LENGTH];
    buildDeck(&deck, false);
    PlayingCard* cards1[] = {
        &deck[PIPS_PER_SUIT * HEARTS + FOUR - 1],
        &deck[PIPS_PER_SUIT * CLUBS + TEN - 1],
        &deck[PIPS_PER_SUIT * CLUBS + FOUR - 1],
        &deck[PIPS_PER_SUIT * SPADES + FIVE - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + FOUR - 1]
    };
    PlayingCard* cards2[] = {
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
    PlayingCard* cards3[] = {
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

static void test_fullHousePresent(CuTest* ct){
    PlayingCard deck[DECK_LENGTH];
    buildDeck(&deck, false);
    PlayingCard* cards1[] = {
        &deck[PIPS_PER_SUIT * HEARTS + FOUR - 1],
        &deck[PIPS_PER_SUIT * CLUBS + ACE - 1],
        &deck[PIPS_PER_SUIT * SPADES + FOUR - 1],
        &deck[PIPS_PER_SUIT * SPADES + ACE - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + FOUR - 1]
    };
    PlayingCard* cards2[] = {
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
    PlayingCard* cards3[] = {
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

static void test_fullHouseNotPresent(CuTest* ct){
    PlayingCard deck[DECK_LENGTH];
    buildDeck(&deck, false);
    //Check that a simple TOaK is not treated as Full House
    PlayingCard* cards1[] = {
        &deck[PIPS_PER_SUIT * HEARTS + KING - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + KING - 1],
        &deck[PIPS_PER_SUIT * SPADES + KING - 1]
    };
    int result1 = detectFullHouse(cards1, 3);
    CuAssert(ct, "", result1 == 0);
}

static void test_flushPresent(CuTest* ct){
    PlayingCard deck[DECK_LENGTH];
    buildDeck(&deck, false);
    //Check for a simple Flush
    PlayingCard* cards1[] = {
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
    PlayingCard* cards2[] = {
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
    PlayingCard* cards3[] = {
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

static void test_flushNotPresent(CuTest* ct){
    PlayingCard deck[DECK_LENGTH];
    buildDeck(&deck, false);
    //Duplicate cards do not count towards a Flush
    PlayingCard* cards1[] = {
        &deck[PIPS_PER_SUIT * HEARTS + KING - 1],
        &deck[PIPS_PER_SUIT * HEARTS + JACK - 1],
        &deck[PIPS_PER_SUIT * HEARTS + NINE - 1],
        &deck[PIPS_PER_SUIT * HEARTS + NINE - 1],
        &deck[PIPS_PER_SUIT * HEARTS + SEVEN - 1],
        &deck[PIPS_PER_SUIT * HEARTS + FOUR - 1]
    };
    //No Flush present returns a zero
    PlayingCard* cards2[] = {
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

static void test_straightPresent(CuTest* ct){
    PlayingCard deck[DECK_LENGTH];
    buildDeck(&deck, false);
    //Detect a simple Straight from among a mixed hand of cards
    PlayingCard* cards1[] = {
        &deck[PIPS_PER_SUIT * SPADES + SEVEN - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + SIX - 1],
        &deck[PIPS_PER_SUIT * HEARTS + ACE - 1],
        &deck[PIPS_PER_SUIT * CLUBS + EIGHT - 1],
        &deck[PIPS_PER_SUIT * HEARTS + FOUR - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + FIVE - 1]
    };
    //Make sure that multiple cards of the same value don't break the consecutivity
    PlayingCard* cards2[] = {
        &deck[PIPS_PER_SUIT * HEARTS + KING - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + QUEEN - 1],
        &deck[PIPS_PER_SUIT * SPADES + QUEEN - 1],
        &deck[PIPS_PER_SUIT * HEARTS + QUEEN - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + JACK - 1],
        &deck[PIPS_PER_SUIT * CLUBS + TEN - 1],
        &deck[PIPS_PER_SUIT * SPADES + NINE - 1]
    };
    //Make sure that multiple duplicates don't cause the incorrect card being returned
    PlayingCard* cards3[] = {
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
    PlayingCard* cards4[] = {
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

static void test_straightNotPresent(CuTest* ct){
    PlayingCard deck[DECK_LENGTH];
    buildDeck(&deck, false);
    //5 cards of the same value does not constitute a Straight
    PlayingCard* cards1[] = {
        &deck[PIPS_PER_SUIT * DIAMONDS + FOUR - 1],
        &deck[PIPS_PER_SUIT * CLUBS + FOUR - 1],
        &deck[PIPS_PER_SUIT * HEARTS + FOUR - 1],
        &deck[PIPS_PER_SUIT * SPADES + FOUR - 1],
        &deck[PIPS_PER_SUIT * SPADES + FOUR - 1],
    };
    int result1 = detectStraight(cards1, 5);
    CuAssert(ct, "", result1 == 0);
}

static void test_TOaKPresent(CuTest* ct){
    PlayingCard deck[DECK_LENGTH];
    buildDeck(&deck, false);
    //Detect a simple TOaK
    PlayingCard* cards1[] = {
        &deck[PIPS_PER_SUIT * CLUBS + FOUR - 1],
        &deck[PIPS_PER_SUIT * HEARTS + FOUR - 1],
        &deck[PIPS_PER_SUIT * SPADES + FOUR - 1]
    };
    //Detect the highest TOaK
    PlayingCard* cards2[] = {
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

static void test_TOaKNotPresent(CuTest* ct){
    PlayingCard deck[DECK_LENGTH];
    buildDeck(&deck, false);
    //For now, just make sure the PC does not explode
    PlayingCard* cards1[] = {
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

static void test_twoPairPresent(CuTest* ct){
    PlayingCard deck[DECK_LENGTH];
    buildDeck(&deck, false);
    //Detect a simple two pair
    PlayingCard* cards1[] = {
        &deck[PIPS_PER_SUIT * CLUBS + THREE - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + THREE - 1],
        &deck[PIPS_PER_SUIT * SPADES + QUEEN - 1],
        &deck[PIPS_PER_SUIT * HEARTS + QUEEN - 1],
        &deck[PIPS_PER_SUIT * SPADES + FOUR - 1],
        &deck[PIPS_PER_SUIT * CLUBS + NINE - 1],
        &deck[PIPS_PER_SUIT * HEARTS + SIX - 1]
    };
    //Another one for good measure
    PlayingCard* cards2[] = {
        &deck[PIPS_PER_SUIT * SPADES + FIVE - 1],
        &deck[PIPS_PER_SUIT * HEARTS + QUEEN - 1],
        &deck[PIPS_PER_SUIT * CLUBS + ACE - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + EIGHT - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + FOUR - 1],
        &deck[PIPS_PER_SUIT * SPADES + EIGHT - 1],
        &deck[PIPS_PER_SUIT * HEARTS + ACE - 1]
    };
    int result1 = detectTwoPair(cards1, 7);
    int result2 = detectTwoPair(cards2, 7);
    CuAssert(ct, "", result1 == (400 * QUEEN) + (20 * THREE) + NINE);
    CuAssert(ct, "", result2 == (400 * ACE) + (20 * EIGHT) + QUEEN);
}

static void test_twoPairNotPresent(CuTest* ct){
    PlayingCard deck[DECK_LENGTH];
    buildDeck(&deck, false);
    //Make sure that one pair doesnt count as two pairs
    PlayingCard* cards1[] = {
        &deck[PIPS_PER_SUIT * CLUBS + THREE - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + THREE - 1],
        &deck[PIPS_PER_SUIT * SPADES + FOUR - 1],
        &deck[PIPS_PER_SUIT * CLUBS + NINE - 1],
        &deck[PIPS_PER_SUIT * HEARTS + SIX - 1]
    };
    //FOaK shouldn't be counter as a two pair (??)
    PlayingCard* cards2[] = {
        &deck[PIPS_PER_SUIT * CLUBS + THREE - 1],
        &deck[PIPS_PER_SUIT * SPADES + FOUR - 1],
        &deck[PIPS_PER_SUIT * CLUBS + NINE - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + THREE - 1],
        &deck[PIPS_PER_SUIT * SPADES + THREE - 1],
        &deck[PIPS_PER_SUIT * HEARTS + THREE - 1],
        &deck[PIPS_PER_SUIT * HEARTS + SIX - 1]
    };
    int result1 = detectTwoPair(cards1, 5);
    int result2 = detectTwoPair(cards2, 7);
    CuAssert(ct, "", result1 == 0);
    CuAssert(ct, "", result2 == 0);
}

static void test_pairPresent(CuTest* ct){
    PlayingCard deck[DECK_LENGTH];
    buildDeck(&deck, false);
    //Simple detection #1
    PlayingCard* cards1[] = {
        &deck[PIPS_PER_SUIT * CLUBS + THREE - 1],
        &deck[PIPS_PER_SUIT * SPADES + FOUR - 1],
        &deck[PIPS_PER_SUIT * CLUBS + NINE - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + THREE - 1],
        &deck[PIPS_PER_SUIT * SPADES + QUEEN - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + EIGHT - 1],
        &deck[PIPS_PER_SUIT * HEARTS + SIX - 1]
    };
    //Simple detection #2
    PlayingCard* cards2[] = {
        &deck[PIPS_PER_SUIT * DIAMONDS + KING - 1],
        &deck[PIPS_PER_SUIT * CLUBS + FOUR - 1],
        &deck[PIPS_PER_SUIT * SPADES + SIX - 1],
        &deck[PIPS_PER_SUIT * HEARTS + JACK - 1],
        &deck[PIPS_PER_SUIT * HEARTS + QUEEN - 1],
        &deck[PIPS_PER_SUIT * CLUBS + EIGHT - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + SIX - 1]
    };
    //Detect higher pair among 2
    PlayingCard* cards3[] = {
        &deck[PIPS_PER_SUIT * CLUBS + FIVE - 1],
        &deck[PIPS_PER_SUIT * SPADES + SIX - 1],
        &deck[PIPS_PER_SUIT * HEARTS + SIX - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + THREE - 1],
        &deck[PIPS_PER_SUIT * CLUBS + FOUR - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + FOUR - 1],
        &deck[PIPS_PER_SUIT * HEARTS + TWO - 1]
    };
    //Detect higher pair among 3
    PlayingCard* cards4[] = {
        &deck[PIPS_PER_SUIT * SPADES + SEVEN - 1],
        &deck[PIPS_PER_SUIT * HEARTS + TWO - 1],
        &deck[PIPS_PER_SUIT * CLUBS + FIVE - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + TWO - 1],
        &deck[PIPS_PER_SUIT * CLUBS + FOUR - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + FIVE - 1],
        &deck[PIPS_PER_SUIT * HEARTS + SEVEN - 1]
    };
    //Not enough kicker cards still gives a score
    PlayingCard* cards5[] = {
        &deck[PIPS_PER_SUIT * DIAMONDS + THREE - 1],
        &deck[PIPS_PER_SUIT * CLUBS + FOUR - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + FOUR - 1]
    };
    int result1 = detectPair(cards1, 7);
    int result2 = detectPair(cards2, 7);
    int result3 = detectPair(cards3, 7);
    int result4 = detectPair(cards4, 7);
    int result5 = detectPair(cards5, 3);
    CuAssert(ct, "", result1 == (THREE * pow(20, 3) + QUEEN * pow(20, 2) + NINE * 20 + EIGHT));
    CuAssert(ct, "", result2 == (SIX * pow(20, 3) + KING * pow(20, 2) + QUEEN * 20 + JACK));
    CuAssert(ct, "", result3 == (SIX * pow(20, 3) + FIVE * pow(20, 2) + FOUR * 20 + THREE));
    CuAssert(ct, "", result4 == (SEVEN * pow(20, 3) + FIVE * pow(20, 2) + FOUR * 20 + TWO));
    CuAssert(ct, "", result5 == (FOUR * pow(20, 3) + THREE * pow(20, 2)));
}

static void test_pairNotPresent(CuTest* ct){
    PlayingCard deck[DECK_LENGTH];
    buildDeck(&deck, false);
    //No pair #1
    PlayingCard* cards1[] = {
        &deck[PIPS_PER_SUIT * SPADES + JACK - 1],
        &deck[PIPS_PER_SUIT * HEARTS + NINE - 1],
        &deck[PIPS_PER_SUIT * CLUBS + FIVE - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + SEVEN - 1],
        &deck[PIPS_PER_SUIT * CLUBS + FOUR - 1]
    };
    //No pair #2
    PlayingCard* cards2[] = {
        &deck[PIPS_PER_SUIT * DIAMONDS + ACE - 1],
        &deck[PIPS_PER_SUIT * CLUBS + EIGHT - 1],
        &deck[PIPS_PER_SUIT * HEARTS + SEVEN - 1],
        &deck[PIPS_PER_SUIT * HEARTS + THREE - 1],
        &deck[PIPS_PER_SUIT * SPADES + FOUR - 1]
    };
    int result1 = detectPair(cards1, 5);
    int result2 = detectPair(cards2, 5);
    CuAssert(ct, "", result1 == 0);
    CuAssert(ct, "", result2 == 0);
}

static void test_highCardCalculations(CuTest* ct){
    PlayingCard deck[DECK_LENGTH];
    buildDeck(&deck, false);
    //Simple test
    PlayingCard* cards1[] = {
        &deck[PIPS_PER_SUIT * DIAMONDS + KING - 1], //12 * 20 ^ 4 = 1 920 000
        &deck[PIPS_PER_SUIT * HEARTS + FIVE - 1],   //4 * 20 ^ 1 = 80
        &deck[PIPS_PER_SUIT * CLUBS + TEN - 1],     //9 * 20 ^ 2 = 3 600
        &deck[PIPS_PER_SUIT * SPADES + JACK - 1],   //10 * 20 ^ 3 = 80 000
        &deck[PIPS_PER_SUIT * HEARTS + TWO - 1]     //1 * 20 ^ 0 = 1
    };  //sum = 2 003 681
    //Less than 5 cards is still calculated correctly
    PlayingCard* cards2[] = {
        &deck[PIPS_PER_SUIT * CLUBS + JACK - 1],    //10 * 20 ^ 2   = 4000
        &deck[PIPS_PER_SUIT * DIAMONDS + NINE - 1], //8 * 20 ^ 1    = 160
        &deck[PIPS_PER_SUIT * SPADES + FIVE - 1]    //4 * 20 ^ 0    = 4
    };
    //More than 5 cards correctly considers only 5 greatest cards
    PlayingCard* cards3[] = {
        &deck[PIPS_PER_SUIT * SPADES + JACK - 1],   //10 * 20 ^ 3  = 80 000
        &deck[PIPS_PER_SUIT * HEARTS + NINE - 1],   //8 * 20 ^ 2   = 3 200
        &deck[PIPS_PER_SUIT * CLUBS + FIVE - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + EIGHT - 1],//7 * 20 ^ 1   = 140
        &deck[PIPS_PER_SUIT * CLUBS + QUEEN - 1],   //11 * 20 ^ 4  = 1 760 000
        &deck[PIPS_PER_SUIT * DIAMONDS + SEVEN - 1],//6 * 20 ^ 0   = 6
        &deck[PIPS_PER_SUIT * CLUBS + FOUR - 1]
    };//sum = 1 843 346
    //Cards with same value are not abandoned during calculation
    PlayingCard* cards4[] = {
        &deck[PIPS_PER_SUIT * SPADES + JACK - 1],   //10 * 20 ^ 3   = 80 000
        &deck[PIPS_PER_SUIT * HEARTS + ACE - 1],    //13 * 20 ^ 4   = 2 080 000
        &deck[PIPS_PER_SUIT * DIAMONDS + JACK - 1], //10 * 20 ^ 2   = 4000
        &deck[PIPS_PER_SUIT * SPADES + JACK - 1],   //10 * 20 ^ 1   = 200
        &deck[PIPS_PER_SUIT * CLUBS + TEN - 1]      // 9 * 20 ^ 0   = 9
    };//sum = 2 164 209
    int result1 = detectHighCard(cards1, 5);
    int result2 = detectHighCard(cards2, 3);
    int result3 = detectHighCard(cards3, 7);
    int result4 = detectHighCard(cards4, 5);
    CuAssert(ct, "", result1 == 2003681);
    CuAssert(ct, "", result2 == 4164);
    CuAssert(ct, "", result3 == 1843346);
    CuAssert(ct, "", result4 == 2164209);
}

static void test_cardSortingInDescendingOrder(CuTest* ct){
    PlayingCard deck[DECK_LENGTH];
    buildDeck(&deck, false);

    PlayingCard* cards[] = {
        &deck[PIPS_PER_SUIT * SPADES + KING - 1],
        &deck[PIPS_PER_SUIT * HEARTS + FIVE - 1],
        &deck[PIPS_PER_SUIT * CLUBS + JACK - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + THREE - 1],
        &deck[PIPS_PER_SUIT * CLUBS + FOUR - 1],

        &deck[PIPS_PER_SUIT * HEARTS + FIVE - 1],
        &deck[PIPS_PER_SUIT * SPADES + TEN - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + SIX - 1],
        &deck[PIPS_PER_SUIT * CLUBS + KING - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + EIGHT - 1],

        &deck[PIPS_PER_SUIT * HEARTS + TEN - 1],
        &deck[PIPS_PER_SUIT * HEARTS + NINE - 1],
        &deck[PIPS_PER_SUIT * SPADES + JACK - 1]
    };
    sortCardsInDescOrder(cards, 13);
    for (int i = 0; i < 12; i++){
        CuAssert(ct, "", cards[i]->pips >= cards[i + 1]->pips);
    }
}

static void test_pipCounting(CuTest* ct){
    PlayingCard deck[DECK_LENGTH];
    buildDeck(&deck, false);

    PlayingCard* cards[] = {
        &deck[PIPS_PER_SUIT * SPADES + KING - 1],
        &deck[PIPS_PER_SUIT * HEARTS + JACK - 1],
        &deck[PIPS_PER_SUIT * CLUBS + JACK - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + THREE - 1],
        &deck[PIPS_PER_SUIT * CLUBS + FOUR - 1],

        &deck[PIPS_PER_SUIT * HEARTS + JACK - 1],
        &deck[PIPS_PER_SUIT * SPADES + TEN - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + EIGHT - 1],
        &deck[PIPS_PER_SUIT * CLUBS + KING - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + EIGHT - 1],

        &deck[PIPS_PER_SUIT * HEARTS + TEN - 1],
        &deck[PIPS_PER_SUIT * HEARTS + NINE - 1],
        &deck[PIPS_PER_SUIT * SPADES + JACK - 1],
        //KING, JACK, THREE, FOUR, TEN, EIGHT, NINE
    };
    int cards_count = 13;
    enum Pip found_pips[cards_count];
    int found_pips_counts[cards_count];

    int found_pips_size = countPipsInCards(found_pips, found_pips_counts, cards, cards_count);

    CuAssert(ct, "", found_pips_size == 7);
    CuAssert(ct, "", found_pips[0] == KING);
    CuAssert(ct, "", found_pips[1] == JACK);
    CuAssert(ct, "", found_pips[2] == THREE);
    CuAssert(ct, "", found_pips[3] == FOUR);
    CuAssert(ct, "", found_pips[4] == TEN);
    CuAssert(ct, "", found_pips[5] == EIGHT);
    CuAssert(ct, "", found_pips[6] == NINE);
    CuAssert(ct, "", found_pips_counts[0] == 2);
    CuAssert(ct, "", found_pips_counts[1] == 4);
    CuAssert(ct, "", found_pips_counts[2] == 1);
    CuAssert(ct, "", found_pips_counts[3] == 1);
    CuAssert(ct, "", found_pips_counts[4] == 2);
    CuAssert(ct, "", found_pips_counts[5] == 2);
    CuAssert(ct, "", found_pips_counts[6] == 1);
}

CuSuite* HandrankingGetSuite(){
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_royalFlushPresent);
    SUITE_ADD_TEST(suite, test_royalFlushNotPresent);
    SUITE_ADD_TEST(suite, test_straightFlushPresent);
    SUITE_ADD_TEST(suite, test_straightFlushNotPresent);
    SUITE_ADD_TEST(suite, test_FOaKPresent);
    SUITE_ADD_TEST(suite, test_FOaKNotPresent);
    SUITE_ADD_TEST(suite, test_fullHousePresent);
    SUITE_ADD_TEST(suite, test_fullHouseNotPresent);
    SUITE_ADD_TEST(suite, test_flushPresent);
    SUITE_ADD_TEST(suite, test_flushNotPresent);
    SUITE_ADD_TEST(suite, test_straightPresent);
    SUITE_ADD_TEST(suite, test_straightNotPresent);
    SUITE_ADD_TEST(suite, test_TOaKPresent);
    SUITE_ADD_TEST(suite, test_TOaKNotPresent);
    SUITE_ADD_TEST(suite, test_twoPairPresent);
    SUITE_ADD_TEST(suite, test_twoPairNotPresent);
    SUITE_ADD_TEST(suite, test_pairPresent);
    SUITE_ADD_TEST(suite, test_pairNotPresent);
    SUITE_ADD_TEST(suite, test_highCardCalculations);
    SUITE_ADD_TEST(suite, test_cardSortingInDescendingOrder);
    SUITE_ADD_TEST(suite, test_pipCounting);
    return suite;
}
