/**
* \file     playingcard.c
* \brief    Contains functions, constants and structs related to playing cards.
*/

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
    }
    return "";
}

const char* getSuitName(enum Suit suits){
    switch (suits){
        case HEARTS: return "Hearts";
        case DIAMONDS: return "Diamonds";
        case CLUBS: return "Clubs";
        case SPADES: return "Spades";
    }
    return "";
}
