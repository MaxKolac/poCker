#ifndef HANDRANKING_H
#define HANDRANKING_H

#include "player.h"

/**
 *  \file handranking.h
 *  \brief Code responsible for recognition of poker rankings based on player's hand and currently revealed community cards.
 *
 *  All methods return a tie breaker score which helps with resolving tie breaks in case two or more players have the same hand.
 *  Higher scores win ties. Each method calculates their tie break score differently.
 */

int detectRoyalFlush(PlayingCard*[], int);
int detectStraightFlush(PlayingCard*[], int);
int detectFOaK(PlayingCard*[], int);
int detectFullHouse(PlayingCard*[], int);
int detectFlush(PlayingCard*[], int);
int detectStraight(PlayingCard*[], int);
int detectTOaK(PlayingCard*[], int);
int detectTwoPair(PlayingCard*[], int);
int detectPair(PlayingCard*[], int);
int detectHighCard(PlayingCard*[], int);
int countPipsInCards(enum Pip[], int[], PlayingCard*[], int);
void sortCardsInDescOrder(PlayingCard*[], int);

#endif // HANDRANKING_H
