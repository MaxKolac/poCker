#include <string.h>

enum Suit {
    HEARTS,
    DIAMONDS,
    CLUBS,
    SPADES
};

enum Pip {
    ACE,
    TWO,
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
    KING
};

struct PlayingCard {
    enum Suit suit;
    enum Pip pips;
};

const char* getPipName(enum Pip);
const char* getSuitName(enum Suit);

const char* getPipName(enum Pip pips){
    switch (pips){
        case ACE: return "Ace";
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
