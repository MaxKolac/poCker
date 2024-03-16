#ifndef PLAYINGCARD_H
#define PLAYINGCARD_H

#include <string.h>

/**
* \file     playingcard.h
* \brief    Contains functions, constants and structs related to playing cards.
*/

const int PIPS_PER_SUIT = 13;
const int SUITS_COUNT = 4;
const int DECK_LENGTH = SUITS_COUNT * PIPS_PER_SUIT;

/** \brief Represents a symbol in a card deck. */
enum Suit {
    HEARTS,
    DIAMONDS,
    CLUBS,
    SPADES
};

/**
* \brief Represents the amount of symbols on a card and more importantly, its value amongst other cards.
* \warning Keep in mind that for the sake of hand rank score calculation, Pip enumerator starts with 1, and not zero!
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
struct PlayingCard {
    /** \brief The card's symbol. */
    enum Suit suit;
    /** \brief The amount of symbols on a card. */
    enum Pip pips;
};

const char* getPipName(enum Pip);
const char* getSuitName(enum Suit);
void getCardName(struct PlayingCard*, char*, int);

#endif  //PLAYINGCARD_H
