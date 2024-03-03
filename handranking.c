#include "constants.h"
#include "playingcard.h"
#include "player.h"

/**
* \file     handranking.c
* \brief    Code responsible for recognition of poker rankings based on player's hand and currently revealed community cards.
*
* All methods return a tie breaker score which helps with resolving tie breaks in case two or more players have the same hand.
* Higher scores win ties. Each method calculates their tie break score differently.
*/

/** \brief  Detects if the given card set would result in a Royal Flush.
 * \param   cards Array of pointers to playing cards.
 * \param   cards_count Size of the cards array.
 * \return  1 if a Royal Flush is present, otherwise 0.
 *
 * Royal Flush is the best hand possible. It consists of A, K, Q, J and 10, all of the same suit.
 *
 * While ties are very unlikely, they are unbreakable. Two or more players having a Royal Flush results in an instant tie.
 *
 * Since Royal Flush ties can't be broken, they don't have any scores calculated.
 * The returned value is a simple integer behaving like a boolean indicating whether or not a Royal Flush was found.
 */
int detectRoyalFlush(struct PlayingCard* cards[], int cards_count){
    //If program finds a card that could be a part of a Royal Flush, it remembers finding it in a 4 by 5 bool table.
    //Kinda like ticking off boxes on a bingo card, except the only way to get bingo is fill any suit's row.
    //If one suit column is filled with only trues, it means we have a Royal Flush.
    //Royal Flush requires 5 cards so if we already had more than (5 + 2) - 5 = 2 cards smaller than TEN
    //then it's pointless to keep looking for a Royal Flush.
    int conflictingCards = 0;
    bool card_bingo_table[SUITS_COUNT][5];
    for (int i = 0; i < CARDS_PER_PLAYER + COMM_CARDS_COUNT; i++){
        if (conflictingCards > CARDS_PER_PLAYER + COMM_CARDS_COUNT - 5){
            return 0;
        }
        if (cards[i]->pips >= TEN){
            card_bingo_table[cards[i]->suit][cards[i]->pips - TEN] = true;
        }
        else{
            conflictingCards++;
        }
    }

    //Check if we got a "bingo" anywhere - 5 required cards for a Royal Flush of the same suit.
    for (int i = 0; i < SUITS_COUNT; i++){
        int filledBoxes = 0;
        for (int j = 0; j < 5; j++){
            if (card_bingo_table[i][j]){
                filledBoxes++;
            }
        }
        if (filledBoxes == 5){
            return 1;
        }
    }
    return 0;
}

/** \brief  Detects if the given card set would result in a Straight Flush.
 * \param   cards Array of pointers to playing cards.
 * \param   cards_count Size of the cards array.
 * \return  Tie breaker score to help determine a winner in case of a tie. Higher score wins. If a Straight Flush was not present, 0 is returned.
 *
 * Straight Flush is the 2nd best hand possible. It consists of 5 cards of consecutive value and of the same suit.
 * For example: 10 of Clubs, 9 of Clubs, 8 of Clubs, 7 of Clubs, 6 of Clubs.
 *
 * While ties are unlikely, in the event of one happening, highest card wins.
 * If two or more players have Straight Flushes with same highest cards, a tie becomes unbreakable.
 *
 * The score of a Straight Flush is equal to the highest card's value.
 */
int detectStraightFlush(struct PlayingCard* cards[], int cards_count){
    return 0;
}

/** \brief  Detects if the given card set would result in a Four Of a Kind.
 * \param   cards Array of pointers to playing cards.
 * \param   cards_count Size of the cards array.
 * \return  Tie breaker score to help determine a winner in case of a tie. Higher score wins. If a Four Of a Kind was not present, 0 is returned.
 *
 * Four Of a Kind is a strong hand consisting of 4 cards of equal value.
 * For example: 8888Q, 9999A, 4444J
 *
 * In the event of a tie between players, the highest value of those four cards that make up the rank wins.
 * If two or more players have Four of a Kind consisting of same cards, highest 5th kicker card wins.
 * If the kicker card could not resolve a tie, it becomes unbreakable.
 *
 * The score of a Four Of a Kind is calculated as follows:
 * (value of card that makes the FOaK) * 100 + (value of 5th card)
 */
int detectFOaK(struct PlayingCard* cards[], int cards_count){
    return 0;
}

/** \brief  Detects if the given card set would result in a Full House.
 * \param   cards Array of pointers to playing cards.
 * \param   cards_count Size of the cards array.
 * \return  Tie breaker score to help determine a winner in case of a tie. Higher score wins. If a Full House was not present, 0 is returned.
 *
 * Full House is a hand consisting of 3 cards of one value and 2 cards of another
 * For example: JJ888, 33AAA, QQ555, 44KKK
 *
 * In the event of a tie between players, the highest value of the card making up the "trio" wins.
 * If two or more players have a Full House with the same "trio", the highest value of the remaining pair wins.
 * If the card pair could not resolve a tie, it becomes unbreakable.
 *
 * The score of a Full House is calculated as follows:
 * ("trio" card value) * 100 + (pair card value)
 */
int detectFullHouse(struct PlayingCard* cards[], int cards_count){
    return 0;
}

/** \brief  Detects if the given card set would result in a Flush.
 * \param   cards Array of pointers to playing cards.
 * \param   cards_count Size of the cards array.
 * \return  Tie breaker score to help determine a winner in case of a tie. Higher score wins. If a Flush was not present, 0 is returned.
 *
 * Flush is a hand consisting of 5 cards of the same suit.
 * For example: Ace of Spades, 4 of Spades, Queen of Spades, 7 of Spades, 10 of Spades
 *
 * In the event of a tie between players, the highest card in the Flush wins.
 * Rule of the highest card continues through the rest of the cards in descending order, in case a tie is still unbroken.
 * If two or more players have cards of exact same value, tie becomes unbreakable.
 *
 * The score of a Flush is calculated as follows:
 * Sum of (cardValue * 20 ^ n) for cards in descending order, where n = 4 -> 0
 */
int detectFlush(struct PlayingCard* cards[], int cards_count){
    return 0;
}

/** \brief  Detects if the given card set would result in a Straight.
 * \param   cards Array of pointers to playing cards.
 * \param   cards_count Size of the cards array.
 * \return  Tie breaker score to help determine a winner in case of a tie. Higher score wins. If a Straight was not present, 0 is returned.
 *
 * Straight is a hand which has 5 cards of consecutive value, but of different suits.
 * For example: T9876, KQJT9, 65432, 87654
 *
 * In the event of a tie between players, the highest card in the Straight wins.
 * If two or more players have cards of exact same value, tie becomes unbreakable.
 *
 * The score of a Straight is simply the value of the highest card.
 */
int detectStraight(struct PlayingCard* cards[], int cards_count){
    return 0;
}

/** \brief  Detects if the given card set would result in a Three Of a Kind.
 * \param   cards Array of pointers to playing cards.
 * \param   cards_count Size of the cards array.
 * \return  Tie breaker score to help determine a winner in case of a tie. Higher score wins. If a Three Of a Kind was not present, 0 is returned.
 *
 * Three Of a Kind is a hand which consists of 3 cards of equal value.
 * For example: TTT23, 666Q2, 999AQ
 *
 * In the event of a tie between players, the higher card making up the TOaK wins.
 * In a game with only one full deck of playing cards, it's impossible for 2 players to have a Three Of a Kind consisting of cards with the same value.
 *
 * The score of a Three Of a Kind is simply the value of the card the TOaK is made of.
 */
int detectTOaK(struct PlayingCard* cards[], int cards_count){
    return 0;
}

/** \brief  Detects if the given card set would result in a Two Pair.
 * \param   cards Array of pointers to playing cards.
 * \param   cards_count Size of the cards array.
 * \return  Tie breaker score to help determine a winner in case of a tie. Higher score wins. If a Two Pair was not present, 0 is returned.
 *
 * Two Pair is a hand which consists of 2 sets of 2 cards of equal value.
 * For example: 9922A, 55TTK, QQ778
 *
 * In the event of a tie between players, the higher card pair wins.
 * If the higher pairs are the same, the higher second pair wins.
 * If both second pairs are the same, the highest value of the 5th kicker card wins.
 * If even the kicker card is of the same value, the tie becomes unbreakable.
 *
 * The score of a Two Pair is calculated as follows:
 * (value of higher pair card * 20 ^ 2) + (value of lower pair card * 20) + (value of kicker card)
 */
int detectTwoPair(struct PlayingCard* cards[], int cards_count){
    return 0;
}

/** \brief  Detects if the given card set would result in a Pair.
 * \param   cards Array of pointers to playing cards.
 * \param   cards_count Size of the cards array.
 * \return  Tie breaker score to help determine a winner in case of a tie. Higher score wins. If a Pair was not present, 0 is returned.
 *
 * Pair is a hand which consists of 2 cards of equal value.
 * For example: KK827, QQ24A, JJ76Q
 *
 * In the event of a tie between players, the higher card pair wins.
 * If two or more players have a Pair of the same value, the highest card from the remaining 3 cards wins.
 * If the highest cards are the same, the second highest determines the winner. Same for the lowest card.
 * If two or more players have a set of cards with the exact same values, tie becomes unbreakable.
 *
 * The score of a Pair is calculated as follows:
 * (pair card value * 20 ^ 3) + (highest non-pair card * 20 ^ 2) + (2nd highest non-pair card * 20) + (lowest non-pair card)
 */
int detectPair(struct PlayingCard* cards[], int cards_count){
    return 0;
}

/** \brief  Calculates players' High Card score.
 * \param   cards Array of pointers to playing cards.
 * \param   cards_count Size of the cards array.
 * \return  Tie breaker score to help determine a winner in case of a tie. Higher score wins. Since High Card will always be present, 0 will never be returned.
 *
 * High Card is the weakest hand possible where a combination of player's and community's cards do not interact to create a stronger hand.
 *
 * The score of a High Card is calculated as follows:
 * Sum of (card value * 20 ^ n) for all 5 cards in descending order, where n = 4 -> 0
 */
int detectHighCard(struct PlayingCard* cards[], int cards_count){
    return 0;
}

void scorePlayersHand(struct Player* _player, struct PlayingCard* comm_cards[], int rev_cards_count){
    //Build an array containing all cards to analyze
    struct PlayingCard* all_cards[CARDS_PER_PLAYER + rev_cards_count];
    for (int i = 0; i < CARDS_PER_PLAYER; i++){
        all_cards[i] = _player->current_hand[i];
    }
    for (int i = 0; i < rev_cards_count; i++){
        all_cards[i + CARDS_PER_PLAYER] = comm_cards[i];
    }

    //Calculate scores for each individual rank
    _player->scores[0] = detectRoyalFlush(all_cards, CARDS_PER_PLAYER + rev_cards_count);
    _player->scores[1] = detectStraightFlush(all_cards, CARDS_PER_PLAYER + rev_cards_count);
    _player->scores[2] = detectFOaK(all_cards, CARDS_PER_PLAYER + rev_cards_count);
    _player->scores[3] = detectFullHouse(all_cards, CARDS_PER_PLAYER + rev_cards_count);
    _player->scores[4] = detectFlush(all_cards, CARDS_PER_PLAYER + rev_cards_count);
    _player->scores[5] = detectStraight(all_cards, CARDS_PER_PLAYER + rev_cards_count);
    _player->scores[6] = detectTOaK(all_cards, CARDS_PER_PLAYER + rev_cards_count);
    _player->scores[7] = detectTwoPair(all_cards, CARDS_PER_PLAYER + rev_cards_count);
    _player->scores[8] = detectPair(all_cards, CARDS_PER_PLAYER + rev_cards_count);
    _player->scores[9] = detectHighCard(all_cards, CARDS_PER_PLAYER + rev_cards_count);
}