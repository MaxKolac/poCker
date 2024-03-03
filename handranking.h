#include "playingcard.h"
#include "player.h"

#ifndef handranking.h
#define handranking.h

const int RANKS_COUNT = 10;

void scorePlayersHand(struct Player*, struct PlayingCard*[], int);
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

#endif
