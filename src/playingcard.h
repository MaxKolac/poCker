#ifndef PLAYINGCARD_H
#define PLAYINGCARD_H

#include <string.h>

/**
* \file     playingcard.h
* \brief    Contains functions, constants and structs related to playing cards.
*/

#define PIPS_PER_SUIT 13
#define SUITS_COUNT 4
#define DECK_LENGTH (4 * 13)
#define CARDNAME_MAX_LENGTH (5 + 4 + 8 + 1)
#define CARDS_PER_PLAYER 2
#define COMM_CARDS_COUNT 5

extern const int PIPS_PER_SUIT_OBJ;
extern const int SUITS_COUNT_OBJ;
extern const int DECK_LENGTH_OBJ;
extern const int CARDS_PER_PLAYER_OBJ;
extern const int COMM_CARDS_COUNT_OBJ;

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
