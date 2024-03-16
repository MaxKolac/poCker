#ifndef PLAYINGCARD_ENUMS_H
#define PLAYINGCARD_ENUMS_H

/**
 *  \file playingcard_enums.h
 *  \brief Stores enums and PlayingCard struct.
 *  \warning For unknown reasons to me, those enums can NOT be in the playingcard.h.
 *           Doing so results in an "enum Pip has incomplete type" compile error on getPipName() function.
 *           No idea why the source file does not see its own header, but putting those enums in a separate file worked.
 */

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
struct PlayingCard {
    /** \brief The card's symbol. */
    enum Suit suit;
    /** \brief The amount of symbols on a card. */
    enum Pip pips;
};

#endif // PLAYINGCARD_ENUMS_H
