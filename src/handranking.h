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

int detectRoyalFlush(struct PlayingCard*[], int);
int detectStraightFlush(struct PlayingCard*[], int);
int detectFOaK(struct PlayingCard*[], int);
int detectFullHouse(struct PlayingCard*[], int);
int detectFlush(struct PlayingCard*[], int);
int detectStraight(struct PlayingCard*[], int);
int detectTOaK(struct PlayingCard*[], int);
int detectTwoPair(struct PlayingCard*[], int);
int detectPair(struct PlayingCard*[], int);
int detectHighCard(struct PlayingCard*[], int);
int countPipsInCards(enum Pip[], int[], struct PlayingCard*[], int);

#endif // HANDRANKING_H
