#include <stdbool.h>
#include <stdio.h>
#include "constants.h"
#include "gamerules.h"
#include "handranking.h"
#include "player.h"
#include "playingcard_enums.h"
#include "playingcard.h"
#include "utils.h"

/**
 *  \brief Distributes random playing cards to players' hands and selects community cards.
 *  \param deck An array of previously generated playing cards.
 *  \param player_hands Array of pointers which will be populated with addresses to deck's elements.
 *  \param comm_cards Array of pointers which will be populated with addresses to deck's elements.
 *
 *  First, it generates an array of random numbers which will symbolize playing cards' index in the deck.
 *  Then, this array will be checked and modified to ensure that all elements are unique.
 *  Lastly, players' hands and community card arrays are populated with addresses to cards with randomly selected indexes.
 *  Each used index is replaced with -1 to ensure no player and community card were given the same card.
 */
void distributeCards(struct PlayingCard deck[], struct Player players[], struct PlayingCard* comm_cards[]){
    //Generate an array of random numbers in range of deck's array length.
    int indexes_count = PLAYER_COUNT * CARDS_PER_PLAYER + COMM_CARDS_COUNT;
    int indexes[indexes_count];
    for (int i = 0; i < indexes_count; i++){
        indexes[i] = randRange(0, DECK_LENGTH - 1);
    }
    //Debug
    //for (int i = 0; i < indexes_count; i++)
    //    printf("%d\n", indexes[i]);

    //Compare all array elements to ensure that they are unique.
    //It starts from the first element and compares it to all elements after it.
    //If two indexes are similar, randomize the J index's value and check if it's a duplicate of any indexes to the left of it.
    //This index will be compared to the rest of indexes to the right later on when I reaches its position in the array.
    for (int i = 0; i < indexes_count - 1; i++){
        for (int j = i + 1; j < indexes_count; j++){
            if (indexes[i] == indexes[j]){
                bool isDuplicate;
                do{
                    isDuplicate = false;
                    indexes[j] = randRange(0, DECK_LENGTH - 1);
                    for (int k = 0; k < j; k++){
                        isDuplicate = indexes[j] == indexes[k] || isDuplicate;
                    }
                } while (isDuplicate);
            }
        }
    }
    //Debug - check for uniqueness
    //for (int i = 0; i < indexes_count - 1; i++)
    //{
    //    for (int j = i + 1; j < indexes_count; j++){
    //        printf("[i = %d] = %d; [j = %d] = %d;\n", i, indexes[i], j, indexes[j]);
    //        assert(indexes[i] != indexes[j]);
    //    }
    //}

    //Now assign addresses of playing cards under those indexes.
    //Indexes are chosen randomly too. Once an index is used, its value is set to -1.
    //If the chosen index is -1, keep searching for the first non-negative element and use that instead.
    for (int i = 0; i < PLAYER_COUNT; i++){
        for (int j = 0; j < CARDS_PER_PLAYER; j++){
            int selectedIndex = randRange(0, indexes_count - 1);
            while (indexes[selectedIndex] == -1){
                selectedIndex = (selectedIndex + 1) % indexes_count;
            }
            players[i].current_hand[j] = &deck[indexes[selectedIndex]];
            indexes[selectedIndex] = -1;
        }
    }
    //After all this, there should be COMM_CARDS_COUNT amount of indexes left.
    //Those will be community cards.
    for (int i = 0; i < COMM_CARDS_COUNT; i++){
        int selectedIndex = randRange(0, indexes_count - 1);
        while (indexes[selectedIndex] == -1){
            selectedIndex = (selectedIndex + 1) % indexes_count;
        }
        comm_cards[i] = &(deck[indexes[selectedIndex]]);
        indexes[selectedIndex] = -1;
    }
    //Debug
    //for (int i = 0; i < indexes_count; i++)
    //    assert(indexes[i] == -1);
}

/**
 *  \brief Builds a deck of playing cards containing 4 suits & 13 cards for each suit.
 *  \param targetArray The array of PlayingCards to build the deck in.
 *  \param print_addrs If true, console will print out memory address of each card. For debug purposes.
 */
void buildDeck(struct PlayingCard targetArray[], bool print_addrs){
    for (int i = 0; i < SUITS_COUNT; i++){
        for (int j = 0; j < PIPS_PER_SUIT; j++){
            targetArray[PIPS_PER_SUIT * i + j].suit = i;
            targetArray[PIPS_PER_SUIT * i + j].pips = j + 1; //Remember, Pips start from 1, not zero!
            //printf("%d\n", PIPS_PER_SUIT * i + j);
        }
    }
    //Debug
    if (print_addrs){
        for (int i = 0; i < DECK_LENGTH; i++){
            char buffer[30];
            getCardName(&targetArray[i], buffer, 30);
            printf("%s at %p\n", buffer, &targetArray[i]);
            //printf("%s of %s at %p\n", getPipName(targetArray[i].pips), getSuitName(targetArray[i].suit), &targetArray[i]);
        }
    }
}

/**
 *  \brief Analyzes Player's hand using functions inside handranking.c and saves scores in Player.scores array.
 *  \param _player The player whose hand will be scored.
 *  \param comm_cards An array of pointers to current community cards.
 *  \param rev_cards_count How many community cards have been revealed by dealer.
 *  For more info on how scores work, refer to handranking.h documentation.
 */
void scorePlayersHand(struct Player _player, struct PlayingCard* comm_cards[], int rev_cards_count){
    //Build an array containing all cards to analyze
    struct PlayingCard* all_cards[CARDS_PER_PLAYER + rev_cards_count];
    for (int i = 0; i < CARDS_PER_PLAYER; i++){
        all_cards[i] = _player.current_hand[i];
    }
    for (int i = 0; i < rev_cards_count; i++){
        all_cards[i + CARDS_PER_PLAYER] = comm_cards[i];
    }

    //This might be overengineered, but honestly ATM I can't think of a better place to try out function pointers
    int (*handranks[10]) (struct PlayingCard*[], int) = {
        detectRoyalFlush,
        detectStraightFlush,
        detectFOaK,
        detectFullHouse,
        detectFlush,
        detectStraight,
        detectTOaK,
        detectTwoPair,
        detectPair,
        detectHighCard
    };

    //Calculate scores for each individual rank
    for (int i = 0; i < 10; i++){
        _player.scores[i] = (*handranks[i]) (all_cards, CARDS_PER_PLAYER + rev_cards_count);
    }
}

/**
 *  \brief Compares the scores of all players on the same tier (scores array index). Populates the winners array with player indexes who function deemed as winners.
 *  \param players The Players array to compare score-wise.
 *  \param players_count The length of players array.
 *  \param winners An array which the function will populate with player indexes who are to be awarded the pot or part of it.
 *  \returns The size of the resulting winners array, or how many players are winners.
 */
int decideWinners(struct Player players[], int players_count, int *winners){
    int possible_winners[players_count];
    int possible_winners_count = 0;
    int score_tier = -1;

    //Get all players who have a non-zero score on the same tier
    for (int i = 0; i < 10; i++){
        for (int j = 0; j < players_count; j++){
            if (players[j].scores[i] > 0){
                possible_winners[possible_winners_count] = j;
                possible_winners_count++;
                score_tier = i;
            }
        }
        if (possible_winners_count > 0){
            break;
        }
    }

    //Only one player had a non-zero score? They win without a shadow of a doubt
    if (possible_winners_count == 1){
        winners[0] = possible_winners[0];
        return 1;
    }

    //More than 1 player had a non-zero score? Oh boy.
    int winners_count = 0;
    int highest_score = 0;
    for (int i = 0; i < possible_winners_count; i++){
        //Player with a higher score overrides the previous contender for a winner
        if (players[possible_winners[i]].scores[score_tier] > highest_score){
            highest_score = players[possible_winners[i]].scores[score_tier];
            winners_count = 1;
            winners[0] = possible_winners[i];
        }
        //If they have both the exact same score, we have a tie
        else if (players[possible_winners[i]].scores[score_tier] == highest_score){
            winners[winners_count] = possible_winners[i];
            winners_count++;
        }
    }
    return winners_count;
}

/**
 *  \brief Checks that the player is allowed to do their action.
 *  \return True, if the player is allowed. False, otherwise.
 *  This function checks the following:
 *  - can Player afford their raise
 *  - can Player afford their call, and if not, have they betted all their funds
 *  - is Player trying to raise a bet to an amount smaller than the current bet
 *  - if the game has fixed limits, does Player's raise doesn't exceede the high limit
 */
bool checkPlayerDecisionValidity(const struct Player* _player,
                                 const struct GameRuleSet* rules,
                                 int player_decision,
                                 unsigned int current_bet)
{
    //TODO: VERIFY THIS AND ADD PROPER CHECKS FOR FIXED LIMIT TYPE GAME
    //For raises:
    if (player_decision > 0){
        //They can afford it
        return _player->funds >= player_decision &&
               //Player's raise actually raises the bet, and does not reduce or match it
               player_decision > current_bet; //&&
               //TODO: differing rules from different sources on what limit-fixed game is
               //If the game has fixed limits, make sure their bet does not exceede it
               //(!limits_fixed || (limits_fixed && player_decision <= b_blind_amount * 2))
    }
    //For calls/checks:
    else if (player_decision == 0){
        //If player wishes to call/check when not able to afford it, they must bet all their last funds
        //return _player.funds < current_bet ?

    }
    //For folds, no checks need to be performed
    return true;
}
