#include <assert.h>

/**
* \brief Unit-tests for handranking.h file.
* CppUTest refuses to just distribute a pre-compiled version and their repository won't open on my VSCode.
* Still, cumbersome unit-tests are better than none. *shrug*
*/

void test_handrankingPerform(struct PlayingCard deck[]){
    test_royalFlushPresent(deck);
    test_royalFlushNotPresent(deck);
}

void test_royalFlushPresent(struct PlayingCard deck[]){
    struct PlayingCard* cards[] = {
        [0] = &deck[PIPS_PER_SUIT * DIAMONDS + SEVEN - 1], //Seven of Diamonds
        [1] = &deck[PIPS_PER_SUIT * CLUBS + KING - 1], //King of Clubs
        [2] = &deck[PIPS_PER_SUIT * CLUBS + ACE - 1], //Ace of Clubs
        [3] = &deck[PIPS_PER_SUIT * HEARTS + TEN - 1], //Ten of Hearts
        [4] = &deck[PIPS_PER_SUIT * CLUBS + TEN - 1], //Ten of Clubs
        [5] = &deck[PIPS_PER_SUIT * CLUBS + QUEEN - 1], //Queen of Clubs
        [6] = &deck[PIPS_PER_SUIT * CLUBS + JACK - 1], //Jack of Clubs
    };
    int result = detectRoyalFlush(cards, 7);
    assert(result == 1);
}

void test_royalFlushNotPresent(struct PlayingCard deck[]){
    struct PlayingCard* cards[] = {
        [0] = &deck[PIPS_PER_SUIT * DIAMONDS + SEVEN - 1], //Seven of Diamonds
        [1] = &deck[PIPS_PER_SUIT * CLUBS + KING - 1], //King of Clubs
        [2] = &deck[PIPS_PER_SUIT * CLUBS + ACE - 1], //Ace of Clubs
        [3] = &deck[PIPS_PER_SUIT * HEARTS + TEN - 1], //Ten of Hearts
        [4] = &deck[PIPS_PER_SUIT * CLUBS + TEN - 1], //Ten of Clubs
        [5] = &deck[PIPS_PER_SUIT * SPADES + QUEEN - 1], //Queen of Spades
        [6] = &deck[PIPS_PER_SUIT * CLUBS + JACK - 1], //Jack of Clubs
    };
    int result = detectRoyalFlush(cards, 7);
    assert(result == 0);
}

void test_straightFlushPresent(struct PlayingCard deck[]){
    struct PlayingCard* cards[] = {
        [0] = &deck[PIPS_PER_SUIT * + - 1],
        [1] = &deck[PIPS_PER_SUIT * + - 1],
        [2] = &deck[PIPS_PER_SUIT * + - 1],
        [3] = &deck[PIPS_PER_SUIT * + - 1],
        [4] = &deck[PIPS_PER_SUIT * + - 1],
        [5] = &deck[PIPS_PER_SUIT * + - 1],
        [6] = &deck[PIPS_PER_SUIT * + - 1],
    };
    int result = detectStraightFlush(cards, 6);
}
