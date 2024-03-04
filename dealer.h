#include <assert.h>

/**
* \file     dealer.h
* \brief    Contains function-equivalents of what a real-life table dealer would do in a real poker game.
*/

void buildDeck(struct PlayingCard[]);
void distributeCards();

/** \brief  Distributes random playing cards to players' hands and selects community cards.
 * \param   deck An array of previously generated playing cards.
 * \param   player_hands Array of pointers which will be populated with addresses to deck's elements.
 * \param   comm_cards Array of pointers which will be populated with addresses to deck's elements.
 *
 *  First, it generates an array of random numbers which will symbolize playing cards' index in the deck.
 *  Then, this array will be checked and modified to ensure that all elements are unique.
 *  Lastly, players' hands and community card arrays are populated with addresses to cards with randomly selected indexes.
 *  Each used index is replaced with -1 to ensure no player and community card were given the same card.
 */
void distributeCards(struct PlayingCard deck[],
                     struct Player* players[],
                     struct PlayingCard* comm_cards[]){
    //Generate an array of random numbers in range of deck's array length.
    int indexes_count = PLAYER_COUNT * CARDS_PER_PLAYER + COMM_CARDS_COUNT;
    int indexes[indexes_count];
    for (int i = 0; i < indexes_count; i++){
        indexes[i] = randRange(0, DECK_LENGTH - 1);
    }
    //Debug
    //for (int i = 0; i < indexes_count; i++){
    //    printf("%d\n", indexes[i]);
    //}

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
    //for (int i = 0; i < indexes_count - 1; i++){
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
            players[i]->current_hand[j] = &(deck[indexes[selectedIndex]]);
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
    //for (int i = 0; i < indexes_count; i++){
    //    assert(indexes[i] == -1);
    //}
}

/**
*   \brief Builds a deck of playing cards containing 4 suits & 13 cards for each suit.
*   \param targetArray The array of PlayingCards to build the deck in.
*/
void buildDeck(struct PlayingCard targetArray[]){
    for (int i = 0; i < SUITS_COUNT; i++){
        for (int j = 0; j < PIPS_PER_SUIT; j++){
            targetArray[PIPS_PER_SUIT * i + j].suit = i;
            targetArray[PIPS_PER_SUIT * i + j].pips = j + 1; //Remember, Pips start from 1, not zero!
            //printf("%d\n", PIPS_PER_SUIT * i + j);
        }
    }
    //Debug
    //for (int i = 0; i < DECK_LENGTH; i++){
    //    printf("%s of %s at %p\n", getPipName(targetArray[i].pips), getSuitName(targetArray[i].suit), &targetArray[i]);
    //}
}
