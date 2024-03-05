#ifndef playingcard.h
#define playingcard.h

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

const char* getPipName(enum Pip pips){
    switch (pips){
        case TWO: return "Two";
        case THREE: return "Three";
        case FOUR: return "Four";
        case FIVE: return "Five";
        case SIX: return "Six";
        case SEVEN: return "Seven";
        case EIGHT: return "Eight";
        case NINE: return "Nine";
        case TEN: return "Ten";
        case JACK: return "Jack";
        case QUEEN: return "Queen";
        case KING: return "King";
        case ACE: return "Ace";
        default: return "";
    }
}

const char* getSuitName(enum Suit suits){
    switch (suits){
        case HEARTS: return "Hearts";
        case DIAMONDS: return "Diamonds";
        case CLUBS: return "Clubs";
        case SPADES: return "Spades";
        default: return "";
    }
}

void getCardName(struct PlayingCard* card, char* buffer, int buffer_size){
    snprintf(buffer, buffer_size, "%s of %s", getPipName(card->pips), getSuitName(card->suit));
}

#endif
