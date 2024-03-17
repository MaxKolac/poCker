#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include "playingcard.h"
#include "playingcard_enums.h"
#include "utils.h"

/**
 *  \brief Detects if the given card set would result in a Royal Flush.
 *  \param cards Array of pointers to playing cards.
 *  \param cards_count Size of the cards array.
 *  \return 1 if a Royal Flush is present, otherwise 0.
 *
 *  Royal Flush is the best hand possible. It consists of A, K, Q, J and 10, all of the same suit.
 *
 *  While ties are very unlikely, they are unbreakable. Two or more players having a Royal Flush results in an instant tie.
 *
 *  Since Royal Flush ties can't be broken, they don't have any scores calculated.
 *  The returned value is a simple integer behaving like a boolean indicating whether or not a Royal Flush was found.
 */
int detectRoyalFlush(struct PlayingCard* cards[], int cards_count){
    //If program finds a card that could be a part of a Royal Flush, it remembers finding it in a 4 by 5 bool table.
    //Kinda like ticking off boxes on a bingo card, except the only way to get bingo is fill any suit's row.
    //If one suit column is filled with only trues, it means we have a Royal Flush.

    //Table needs to be first filled with 'false's.
    //Testing has shown that this table almost always includes some random integers from the memory, likely because the array is only declared.
    //Condition checking each bingo square for a 'true' returns true for integers greater than 1, it seems.
    //Without pre-filling the array with falses, it is very likely for filledBoxes to be incremented at wrong times.
    //This algorithm could probably be made shorter if instead of checking for 'true's,
    //the condition was 'equals to 1', but I'm too scared right now. Don't fix what ain't broke.
    bool card_bingo_table[SUITS_COUNT][5];
    for (int i = 0; i < SUITS_COUNT; i++){
        for (int j = 0; j < 5; j++){
            card_bingo_table[i][j] = false;
        }
    }

    //Royal Flush requires 5 cards so if we already had more than cards_count - 5 cards smaller than Pips.TEN
    //then it's pointless to keep looking for a Royal Flush.
    int conflictingCards = 0;
    for (int i = 0; i < cards_count; i++){
        if (conflictingCards > cards_count - 5){
            return 0;
        }
        //printf("%s of %s\n", getPipName(cards[i]->pips), getSuitName(cards[i]->suit));
        if (cards[i]->pips >= TEN){
            card_bingo_table[cards[i]->suit][(cards[i]->pips) - TEN] = true;
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

/**
 *  \brief Detects if the given card set would result in a Straight Flush.
 *  \param cards Array of pointers to playing cards.
 *  \param cards_count Size of the cards array.
 *  \return Tie breaker score to help determine a winner in case of a tie. Higher score wins. If a Straight Flush was not present, 0 is returned.
 *
 *  Straight Flush is the 2nd best hand possible. It consists of 5 cards of consecutive value and of the same suit.
 *  For example: 10 of Clubs, 9 of Clubs, 8 of Clubs, 7 of Clubs, 6 of Clubs.
 *
 *  While ties are unlikely, in the event of one happening, highest card wins.
 *  If two or more players have Straight Flushes with same highest cards, a tie becomes unbreakable.
 *
 *  The score of a Straight Flush is equal to the highest card's value.
 */
int detectStraightFlush(struct PlayingCard* cards[], int cards_count){
    //Cards are sorted by suits, and then by their value.
    //Creating 4 lists of them, all in descending order.
    //Then, whichever list happens to have 5 or more cards
    //And they are all consecutive in relation to their neighboring cards
    //Result in a Straight Flush
    struct PlayingCard* suit_arrays[SUITS_COUNT][cards_count];
    int suit_arrays_size[SUITS_COUNT];
    for (int i = 0; i < SUITS_COUNT; i++){
        suit_arrays_size[i] = 0;
    }

    //Put each card into their respective suit_array in such manner that each list keeps their cards in a descending order.
    for (int i = 0; i < cards_count; i++){
        //If there is/are already card(s) in this array, putting another card will require finding a spot for it
        //So that the whole array retains its elements in a descending order
        //Debug
        //char debug_buff[30]; getCardName(cards[i], debug_buff, 30); printf("%s\n", debug_buff);
        if (suit_arrays_size[cards[i]->suit] > 0){
            for (int j = 0; j < suit_arrays_size[cards[i]->suit]; j++){
                //If our current card has greater value, than the current 'j' card
                //To preserve the descending order, 'i' will have to be inserted into this spot and push all cards after it by 1.
                //debug_buff[30];
                //getCardName(suit_arrays[cards[i]->suit][j], debug_buff, 30);
                //printf("%s smaller than %s ?\n", debug_buff, getPipName(cards[i]->pips));
                if (suit_arrays[cards[i]->suit][j]->pips < cards[i]->pips){
                    //Move elements forward by 1 index, including the one we just compared our card to
                    //Place the card in the resulting empty space
                    for (int k = suit_arrays_size[cards[i]->suit]; k > j; k--){
                        suit_arrays[cards[i]->suit][k] = suit_arrays[cards[i]->suit][k - 1];
                    }
                    suit_arrays[cards[i]->suit][j] = cards[i];
                    break;
                }
                //If all cards in the array were greater than current card and we reached j == array's size - 1 (end of iterating over suit_array)
                //We'll append the card at the end
                else if (j + 1 == suit_arrays_size[cards[i]->suit]){
                    suit_arrays[cards[i]->suit][j + 1] = cards[i];
                    //suit_arrays_size[cards[i]->suit]++;
                    //We need to break the 'for' loop, because incrementing suit_array_size will make the 'for' loop
                    //perform an additional iteration. During that iteration, both conditions in 'if' and 'else if' would be true.
                    //That would cause adding the same card over and over.
                    break;
                }
            }
        }
        //But if this suit_array is empty, we can freely put the card in it without any consequences
        else {
            suit_arrays[cards[i]->suit][0] = cards[i];
        }
        //Regardless of which condition was true, size of one of suit_arrays definitely increased by 1.
        suit_arrays_size[cards[i]->suit]++;
    }

    //Now, the suit_arrays should have all the passed cards sorted by suit, then value.
    //We simply iterate over suit_arrays that gathered 5 or more cards (because otherwise StraightFlush is impossible)
    //And check if each consecutive card is 1 pip less than the previous.
    for (int i = 0; i < SUITS_COUNT; i++){
        if (suit_arrays_size[i] < 5){
            continue;
        }
        int consecutive_cards = 0;
        for (int j = 1; j < suit_arrays_size[i]; j++){
            if (suit_arrays[i][j - 1]->pips == (suit_arrays[i][j]->pips) + 1){
                consecutive_cards++;
            }
            else {
                consecutive_cards = 0;
            }
            //We are checking a total of 4 relations between cards and if they meet a condition.
            //This is why it's 4 consecutive cards, not 5
            if (consecutive_cards == 4){
                return suit_arrays[i][j - 4]->pips;
            }
        }
    }
    //No array gathered 5 or more cards OR no array has 5 consecutive cards, Straight Flush is impossible.
    return 0;
}

/**
 *  \brief Detects if the given card set would result in a Four Of a Kind.
 *  \param cards Array of pointers to playing cards.
 *  \param cards_count Size of the cards array.
 *  \return Tie breaker score to help determine a winner in case of a tie. Higher score wins. If a Four Of a Kind was not present, 0 is returned.
 *
 *  Four Of a Kind is a strong hand consisting of 4 cards of equal value.
 *  For example: 8888Q, 9999A, 4444J
 *
 *  In the event of a tie between players, the highest value of those four cards that make up the rank wins.
 *  If two or more players have Four of a Kind consisting of same cards, highest 5th kicker card wins.
 *  If the kicker card could not resolve a tie, it becomes unbreakable.
 *
 *  The score of a Four Of a Kind is calculated as follows:
 *  (value of card that makes the FOaK) * 100 + (value of 5th card)
 */
int detectFOaK(struct PlayingCard* cards[], int cards_count){
    //Treat those two arrays like dictionaries, where each unique pip is a key and their count is the value.
    enum Pip found_pips[cards_count];
    int found_pips_counts[cards_count];
    int found_pips_size = countPipsInCards(found_pips, found_pips_counts, cards, cards_count);

    //Find the highest FOaK and save its pip
    int highest_foak = -1;
    for (int i = 0; i < found_pips_size; i++){
        if (found_pips_counts[i] >= 4){
            highest_foak = highest_foak == -1 ? found_pips[i] : mathMax(2, highest_foak, found_pips[i]);
        }
    }
    //No FOaKs found, return zero
    if (highest_foak == -1){
        return 0;
    }
    //After finding highest FOaK, find the highest value card that is not a part of that FOaK
    int highest_kicker = -1;
    for (int i = 0; i < found_pips_size; i++){
        if (found_pips[i] != highest_foak){
            highest_kicker = mathMax(2, highest_kicker, found_pips[i]);
        }
    }
    //Calculate and return total score
    return highest_foak * 100 + highest_kicker;
}

/**
 *  \brief Detects if the given card set would result in a Full House.
 *  \param cards Array of pointers to playing cards.
 *  \param cards_count Size of the cards array.
 *  \return Tie breaker score to help determine a winner in case of a tie. Higher score wins. If a Full House was not present, 0 is returned.
 *
 *  Full House is a hand consisting of 3 cards of one value and 2 cards of another
 *  For example: JJ888, 33AAA, QQ555, 44KKK
 *
 *  In the event of a tie between players, the highest value of the card making up the "trio" wins.
 *  If two or more players have a Full House with the same "trio", the highest value of the remaining pair wins.
 *  If the card pair could not resolve a tie, it becomes unbreakable.
 *
 *  The score of a Full House is calculated as follows:
 *  ("trio" card value) * 100 + (pair card value)
 */
int detectFullHouse(struct PlayingCard* cards[], int cards_count){
    //Treat those two arrays like dictionaries, where each unique pip is a key and their count is the value.
    enum Pip found_pips[cards_count];
    int found_pips_counts[cards_count];
    int found_pips_size = countPipsInCards(found_pips, found_pips_counts, cards, cards_count);

    //Find the highest FOaK and save its pip
    int highest_toak = -1;
    for (int i = 0; i < found_pips_size; i++){
        if (found_pips_counts[i] >= 3){
            highest_toak = highest_toak == -1 ? found_pips[i] : mathMax(2, highest_toak, found_pips[i]);
        }
    }
    //No TOaKs found, Full House is impossible
    if (highest_toak == -1){
        return 0;
    }
    //After finding highest TOaK, find the highest value pair that is not a part of that TOaK
    int highest_pair = -1;
    for (int i = 0; i < found_pips_size; i++){
        if (found_pips[i] != highest_toak && found_pips_counts[i] >= 2){
            highest_pair = highest_pair == -1 ? found_pips[i] : mathMax(2, highest_pair, found_pips[i]);
        }
    }
    //No pair to accompany the TOaK, Full House is impossible.
    //Otherwise, return a proper tie breaker score
    return highest_pair == -1 ? 0 : highest_toak * 100 + highest_pair;
}

/**
 *  \brief Detects if the given card set would result in a Flush.
 *  \param cards Array of pointers to playing cards.
 *  \param cards_count Size of the cards array.
 *  \return Tie breaker score to help determine a winner in case of a tie. Higher score wins. If a Flush was not present, 0 is returned.
 *
 *  Flush is a hand consisting of 5 cards of the same suit.
 *  For example: Ace of Spades, 4 of Spades, Queen of Spades, 7 of Spades, 10 of Spades
 *
 *  In the event of a tie between players, the highest card in the Flush wins.
 *  Rule of the highest card continues through the rest of the cards in descending order, in case a tie is still unbroken.
 *  If two or more players have cards of exact same value, tie becomes unbreakable.
 *
 *  The score of a Flush is calculated as follows:
 *  Sum of (cardValue * 20 ^ n) for cards in descending order, where n = 4 -> 0
 */
int detectFlush(struct PlayingCard* cards[], int cards_count){
    struct PlayingCard* suit_arrays[SUITS_COUNT][cards_count];
    int suit_arrays_size[SUITS_COUNT];
    for (int i = 0; i < SUITS_COUNT; i++){
        suit_arrays_size[i] = 0;
    }

    //Algorithm for grouping cards by suit and sorting in descending order from detectStraightFlush
    for (int i = 0; i < cards_count; i++){
        //Debug
        //char debug_buff[30]; getCardName(cards[i], debug_buff, 30); printf("%s\n", debug_buff);
        if (suit_arrays_size[cards[i]->suit] > 0){
            for (int j = 0; j < suit_arrays_size[cards[i]->suit]; j++){
                //debug_buff[30];
                //getCardName(suit_arrays[cards[i]->suit][j], debug_buff, 30);
                //printf("%s smaller than %s ?\n", debug_buff, getPipName(cards[i]->pips));
                if (suit_arrays[cards[i]->suit][j]->pips < cards[i]->pips){
                    for (int k = suit_arrays_size[cards[i]->suit]; k > j; k--){
                        suit_arrays[cards[i]->suit][k] = suit_arrays[cards[i]->suit][k - 1];
                    }
                    suit_arrays[cards[i]->suit][j] = cards[i];
                    break;
                }
                else if (j + 1 == suit_arrays_size[cards[i]->suit]){
                    suit_arrays[cards[i]->suit][j + 1] = cards[i];
                    break;
                }
            }
        }
        else {
            suit_arrays[cards[i]->suit][0] = cards[i];
        }
        suit_arrays_size[cards[i]->suit]++;
    }

    //Also copied from detectStraightFlush
    //Check suit_arrays that have 5 or more cards
    //In case of two Flushes, keep the highest score calculated
    int highest_score = 0;
    for (int i = 0; i < SUITS_COUNT; i++){
        if (suit_arrays_size[i] < 5){
            continue;
        }
        int consecutive_cards = 0;
        for (int j = 1; j < suit_arrays_size[i]; j++){
            //Difference between this code and code from detectStraightFlush
            //Check that each subsequent card has smaller value than the previous one
            if (suit_arrays[i][j - 1]->pips > suit_arrays[i][j]->pips){
                consecutive_cards++;
            }
            else {
                consecutive_cards = 0;
            }
            //We are checking a total of 4 relations between cards and if they meet a condition.
            //This is why it's 4 consecutive cards, not 5
            if (consecutive_cards == 4){
                int current_score = 0;
                for (int k = 4; k >= 0; k--){
                    current_score += suit_arrays[i][j - k]->pips * pow(20, k);
                }
                highest_score = mathMax(2, highest_score, current_score);
            }
        }
    }
    return highest_score;
}

/**
 *  \brief Detects if the given card set would result in a Straight.
 *  \param cards Array of pointers to playing cards.
 *  \param cards_count Size of the cards array.
 *  \return Tie breaker score to help determine a winner in case of a tie. Higher score wins. If a Straight was not present, 0 is returned.
 *
 *  Straight is a hand which has 5 cards of consecutive value, but of different suits.
 *  For example: T9876, KQJT9, 65432, 87654
 *
 *  In the event of a tie between players, the highest card in the Straight wins.
 *  If two or more players have cards of exact same value, tie becomes unbreakable.
 *
 *  The score of a Straight is simply the value of the highest card.
 */
int detectStraight(struct PlayingCard* cards[], int cards_count){
    //To not meddle with the array passed as parameter, create a temporary copy
    struct PlayingCard* sorted_cards[cards_count];
    for (int i = 0; i < cards_count; i++){
        sorted_cards[i] = cards[i];
    }
    //Sort this array in a descending order
    //Find out if there exists a subarray consisting of 5 cards where each subsequent card is smaller by 1 from the previous one.
    //Multiple cards of the same value do not break the consecutivity.

    //This variable counts cards, not relations between them. One card is considered consecutive to itself.
    int consecutive_cards = 1;
    //Duplicate cards is the amount of duplicates in the currently detected consecutive streak. One card is considered a duplicate of itself.
    int duplicate_cards = 0;
    for (int i = 1; i < cards_count; i++){
        if (sorted_cards[i - 1]->pips == sorted_cards[i]->pips + 1){
            consecutive_cards++;
        }
        else if (sorted_cards[i - 1]->pips == sorted_cards[i]->pips){
            duplicate_cards++;
        }
        else {
            consecutive_cards = 1;
            duplicate_cards = 0;
        }

        if (consecutive_cards == 5){
            return sorted_cards[i - 4 - duplicate_cards]->pips;
        }
    }

    //No subarray of 5 consecutive cards found, return zero.
    return 0;
}

/**
 *  \brief Detects if the given card set would result in a Three Of a Kind.
 *  \param cards Array of pointers to playing cards.
 *  \param cards_count Size of the cards array.
 *  \return Tie breaker score to help determine a winner in case of a tie. Higher score wins. If a Three Of a Kind was not present, 0 is returned.
 *  \warning As of right now, this function will NOT work properly if you introduce the ability to play with more than 1 full deck!
 *
 *  Three Of a Kind is a hand which consists of 3 cards of equal value.
 *  For example: TTT23, 666Q2, 999AQ
 *
 *  In the event of a tie between players, the higher card making up the TOaK wins.
 *  In a game with only one full deck of playing cards, it's impossible for 2 players to have a Three Of a Kind consisting of cards with the same value.
 *
 *  The score of a Three Of a Kind is simply the value of the card the TOaK is made of.
 */
int detectTOaK(struct PlayingCard* cards[], int cards_count){
    //Treat those two arrays like dictionaries, where each unique pip is a key and their count is the value.
    enum Pip found_pips[cards_count];
    int found_pips_counts[cards_count];
    int found_pips_size = countPipsInCards(found_pips, found_pips_counts, cards, cards_count);

    //Find the highest TOaK and save its pip
    int highest_toak = -1;
    for (int i = 0; i < found_pips_size; i++){
        if (found_pips_counts[i] >= 3){
            highest_toak = highest_toak == -1 ? found_pips[i] : mathMax(2, highest_toak, found_pips[i]);
        }
    }
    //No TOaKs found, return zero, otherwise, return the highest card
    return highest_toak == -1 ? 0 : highest_toak;
}

/**
 *  \brief Detects if the given card set would result in a Two Pair.
 *  \param cards Array of pointers to playing cards.
 *  \param cards_count Size of the cards array.
 *  \return Tie breaker score to help determine a winner in case of a tie. Higher score wins. If a Two Pair was not present, 0 is returned.
 *
 *  Two Pair is a hand which consists of 2 sets of 2 cards of equal value.
 *  For example: 9922A, 55TTK, QQ778
 *
 *  In the event of a tie between players, the higher card pair wins.
 *  If the higher pairs are the same, the higher second pair wins.
 *  If both second pairs are the same, the highest value of the 5th kicker card wins.
 *  If even the kicker card is of the same value, the tie becomes unbreakable.
 *
 *  The score of a Two Pair is calculated as follows:
 *  (value of higher pair card * 20 ^ 2) + (value of lower pair card * 20) + (value of kicker card)
 */
int detectTwoPair(struct PlayingCard* cards[], int cards_count){
    //Treat those two arrays like dictionaries, where each unique pip is a key and their count is the value.
    enum Pip found_pips[cards_count];
    int found_pips_counts[cards_count];
    int found_pips_size = countPipsInCards(found_pips, found_pips_counts, cards, cards_count);

    //Find the higher pair and save its pip
    int higher_pair = -1;
    for (int i = 0; i < found_pips_size; i++){
        if (found_pips_counts[i] >= 2){
            higher_pair = higher_pair == -1 ? found_pips[i] : mathMax(2, higher_pair, found_pips[i]);
        }
    }
    //No pairs found, return zero
    if (higher_pair == -1){
        return 0;
    }

    //After finding higher pair, find the lower pair, which is the highest pip except the one already found
    int lower_pair = -1;
    for (int i = 0; i < found_pips_size; i++){
        if (found_pips[i] != higher_pair && found_pips_counts[i] >= 2){
            lower_pair = mathMax(2, lower_pair, found_pips[i]);
        }
    }

    //No second pair, return 0
    if (lower_pair == -1){
        return 0;
    }

    //Now find the greatest kicker card
    int kicker_card = -1;
    for (int i = 0; i < found_pips_size; i++){
        if (found_pips[i] != higher_pair && found_pips[i] != lower_pair){
            kicker_card = mathMax(2, kicker_card, found_pips[i]);
        }
    }

    //Calculate and return total score
    return higher_pair * pow(20, 2) + lower_pair * 20 + kicker_card;
}

/**
 *  \brief Detects if the given card set would result in a Pair.
 *  \param cards Array of pointers to playing cards.
 *  \param cards_count Size of the cards array.
 *  \return Tie breaker score to help determine a winner in case of a tie. Higher score wins. If a Pair was not present, 0 is returned.
 *
 *  Pair is a hand which consists of 2 cards of equal value.
 *  For example: KK827, QQ24A, JJ76Q
 *
 *  In the event of a tie between players, the higher card pair wins.
 *  If two or more players have a Pair of the same value, the highest card from the remaining 3 cards wins.
 *  If the highest cards are the same, the second highest determines the winner. Same for the lowest card.
 *  If two or more players have a set of cards with the exact same values, tie becomes unbreakable.
 *
 *  The score of a Pair is calculated as follows:
 *  (pair card value * 20 ^ 3) + (highest non-pair card * 20 ^ 2) + (2nd highest non-pair card * 20) + (3rd highest non-pair card)
 */
int detectPair(struct PlayingCard* cards[], int cards_count){
    //Treat those two arrays like dictionaries, where each unique pip is a key and their count is the value.
    enum Pip found_pips[cards_count];
    int found_pips_counts[cards_count];
    int found_pips_size = countPipsInCards(found_pips, found_pips_counts, cards, cards_count);

    //Find the highest pair and save its pip
    int pair_card = -1;
    for (int i = 0; i < found_pips_size; i++){
        if (found_pips_counts[i] >= 2){
            pair_card = pair_card == -1 ? found_pips[i] : mathMax(2, pair_card, found_pips[i]);
        }
    }
    //No pairs found, return zero
    if (pair_card == -1){
        return 0;
    }

    //After finding highest pair, remove the pair_card from found_pips
    for (int i = 0; i < found_pips_size; i++){
        if (found_pips[i] != pair_card)
            continue;
        for (int j = i; j < found_pips_size - 1; j++){
            found_pips[j] = found_pips[j + 1];
            found_pips_counts[j] = found_pips_counts[j + 1];
        }
        break;
    }
    found_pips_size--;

    //Pick 3 highest unique cards
    int high_kicker = 0, mid_kicker = 0, low_kicker = 0;
    for (int i = 0; i < 3 * found_pips_size; i++){
        int index = i % found_pips_size;
        if (i < found_pips_size){
            high_kicker = high_kicker == 0 ? found_pips[index] : mathMax(2, high_kicker, found_pips[index]);
        }
        else if (i < found_pips_size * 2 && found_pips[index] != high_kicker){
            mid_kicker = mid_kicker == 0 ? found_pips[index] : mathMax(2, mid_kicker, found_pips[index]);
        }
        else if (i < found_pips_size * 3 && found_pips[index] != high_kicker && found_pips[index] != mid_kicker){
            low_kicker = low_kicker == 0 ? found_pips[index] : mathMax(2, low_kicker, found_pips[index]);
        }
    }

    //Calculate final score
    return pair_card * pow(20, 3) + high_kicker * pow(20, 2) + mid_kicker * 20 + low_kicker;
}

/**
 *  \brief Calculates players' High Card score.
 *  \param cards Array of pointers to playing cards.
 *  \param cards_count Size of the cards array.
 *  \return Tie breaker score to help determine a winner in case of a tie. Higher score wins. Since High Card will always be present, 0 will never be returned.
 *
 *  High Card is the weakest hand possible where a combination of player's and community's cards do not interact to create a stronger hand.
 *
 *  The score of a High Card is calculated as follows:
 *  Sum of (card value * 20 ^ n) for all 5 cards in descending order, where n = 4 -> 0
 */
int detectHighCard(struct PlayingCard* cards[], int cards_count){
    return 0;
}

/**
 *  \brief Takes a deck of cards and counts the amounts of each pip in the deck.
 *  \returns The size of the found_pips array which was filled by the function. found_pips and found_pips_counts share the same length.
 *  found_pips and found_pips_counts should be initialized with cards_count lengths before calling this function.
 *  To ensure that the function does not reach outside of those arrays, they should preferably be initialized with the length of cards_count.
 */
int countPipsInCards(enum Pip found_pips[], int found_pips_counts[], struct PlayingCard* cards[], int cards_count){
    //Treat those two arrays like dictionaries, where each unique pip is a key and their count is the value.
    //Initial setup
    int found_pips_size = 0;
    for (int i = 0; i < cards_count; i++){
        found_pips[i] = 0;
        found_pips_counts[i] = 0;
    }
    //Count on how many occurrences of which Pips there are in the cards
    for (int i = 0; i < cards_count; i++){
        for (int j = 0; j <= found_pips_size; j++){
            if (found_pips[j] == cards[i]->pips){
                found_pips_counts[j]++;
                break;
            }
            if (j == found_pips_size){
                found_pips[found_pips_size] = cards[i]->pips;
                found_pips_counts[found_pips_size]++;
                found_pips_size++;
                break;
            }
        }
    }
    //Debug
    //for (int i = 0; i < found_pips_size; i++){
    //    printf("Found %d %s\n", found_pips_counts[i], getPipName(found_pips[i]));
    //
    return found_pips_size;
}
