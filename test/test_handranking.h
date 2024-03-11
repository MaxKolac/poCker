#include <assert.h>

/**
* \brief Unit-tests for handranking.h file.
* CppUTest refuses to just distribute a pre-compiled version and their repository won't open on my VSCode.
* Still, cumbersome unit-tests are better than none. *shrug*
*/

void test_handrankingPerform(struct PlayingCard deck[]){
    test_royalFlushPresent(deck);
    test_royalFlushNotPresent(deck);
    test_straightFlushPresent(deck);
    test_straightFlushNotPresent(deck);
    test_FOaKPresent(deck);
    test_FOaKNotPresent(deck);
    test_fullHousePresent(deck);
    test_fullHouseNotPresent(deck);
    test_flushPresent(deck);
    test_flushNotPresent(deck);
}

void test_royalFlushPresent(struct PlayingCard deck[]){
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
    assert(result == 1);
}

void test_royalFlushNotPresent(struct PlayingCard deck[]){
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
    assert(result == 0);
}

void test_straightFlushPresent(struct PlayingCard deck[]){
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
    assert(result1 == TEN);
    assert(result2 == SIX);
    assert(result3 == QUEEN);
}

void test_straightFlushNotPresent(struct PlayingCard deck[]){
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
    assert(result1 == 0);
    assert(result2 == 0);
    assert(result3 == 0);
}

void test_FOaKPresent(struct PlayingCard deck[]){
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
    assert(result1 == 309);
    assert(result2 == 708);
    assert(result3 == 113);
}

void test_FOaKNotPresent(struct PlayingCard deck[]){
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
    assert(result1 == 0);
    assert(result2 == 0);
    assert(result3 == 0);
}

void test_fullHousePresent(struct PlayingCard* deck[]){
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
    assert(result1 == 313);
    assert(result2 == 904);
    assert(result3 == 1211);
}

void test_fullHouseNotPresent(struct PlayingCard* deck[]){
    //Check that a simple TOaK is not treated as Full House
    struct PlayingCard* cards1[] = {
        &deck[PIPS_PER_SUIT * HEARTS + KING - 1],
        &deck[PIPS_PER_SUIT * DIAMONDS + KING - 1],
        &deck[PIPS_PER_SUIT * SPADES + KING - 1]
    };
    int result1 = detectFullHouse(cards1, 3);
    assert(result1 == 0);
}

void test_flushPresent(struct PlayingCard* deck[]){
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
    assert(result1 == 2003323);
    assert(result2 == 2180609);
    assert(result3 == 1674925);
}

void test_flushNotPresent(struct PlayingCard* deck[]){
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
    assert(result1 == 0);
    assert(result2 == 0);
}
