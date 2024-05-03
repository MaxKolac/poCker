#include "playingcard.h"

#include <stdio.h>
#include <string.h>

const int PIPS_PER_SUIT_OBJ = PIPS_PER_SUIT;
const int SUITS_COUNT_OBJ = SUITS_COUNT;
const int DECK_LENGTH_OBJ = SUITS_COUNT * PIPS_PER_SUIT;

/**
 *  \brief Returns a string literal containing the name of the given Pip enumerator.
 *  \param pips Pip enumerator to get the name of.
 *  Returns the corresponding name of the passed Pip. Returns "ERR" if no match was found.
 */
 const char* getPipName(enum Pip _pips){
    switch (_pips){
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
    }
    return "ERR";
}

/**
 *  \brief Returns a string literal containing the name of the given Suit enumerator.
 *  \param suits Suit enumerator to get the name of.
 *  Returns the corresponding name of the passed Suit. Returns "ERR" if no match was found.
 */
const char* getSuitName(enum Suit _suits){
    switch (_suits){
        case HEARTS: return "Hearts";
        case DIAMONDS: return "Diamonds";
        case CLUBS: return "Clubs";
        case SPADES: return "Spades";
    }
    return "ERR";
}

/**
 *  \brief Populates a char array with the card's name.
 *  \param card Pointer to a card to read.
 *  \param buffer The char array to fill.
 *  \param buffer_size The length of the buffer array.
 *  The buffer will be filled with the card's name in a format of "Pips of Suit". For example "Ace of Spades".
 */
void getCardName(PlayingCard* card, char* buffer, int buffer_size){
    snprintf(buffer, buffer_size, "%s of %s", getPipName(card->pips), getSuitName(card->suit));
}
