#ifndef PLAYINGCARD_H
#define PLAYINGCARD_H

#include <string.h>

/**
*   \file playingcard.h
*   \brief Contains functions, constants and structs related to PlayingCards.
*/

/** \brief Amount of different value cards under the same card symbol. */
#define PIPS_PER_SUIT 13
/** \brief Amount of different symbols in a single deck. */
#define SUITS_COUNT 4
/** \brief Amount of cards in a single deck. */
#define DECK_LENGTH (4 * 13)
/** \brief Longest name of a single PlayingCard. Meant for char arrays that might hold them. */
#define CARDNAME_MAX_LENGTH (5 + 4 + 8 + 1)
/** \brief The amount of hole cards per Player. */
#define CARDS_PER_PLAYER 2
/** \brief The amount of community cards the dealer will place. */
#define COMM_CARDS_COUNT 5

/** \brief Represents a symbol in a card deck. */
enum Suit {
    HEARTS,
    DIAMONDS,
    CLUBS,
    SPADES
};

/**
 *  \brief Represents the amount of symbols on a card and more importantly, its value amongst other cards.
 *  \warning Keep in mind that for the sake of hand rank score calculation, Pip enumerator starts with 1, and not zero!
 */
enum Pip {
    TWO = 1,
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

/** \brief Represents a single playing card. */
typedef struct {
    /** \brief The card's symbol. */
    enum Suit suit;
    /** \brief The amount of symbols on a card. */
    enum Pip pips;
} PlayingCard;

const char* getPipName(enum Pip);
const char* getSuitName(enum Suit);
void getCardName(PlayingCard*, char*, int);

#endif  //PLAYINGCARD_H
