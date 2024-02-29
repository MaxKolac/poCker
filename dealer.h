void distributeHoleCards(struct PlayingCard deck[],
                         struct PlayingCard* player_hands[][CARDS_PER_PLAYER],
                         struct PlayingCard* comm_cards[]){
    //Give to each player a random array of pointers to unique PlayingCards
    //First, get an array of (player_count * cards_per_player) + 5 unique indexes
    for (int i = 0; i < (PLAYER_COUNT * CARDS_PER_PLAYER) + 5; i++){
        printf("%d\n", rand() % DECK_LENGTH);
    }
    //These will be hole cards and community cards
    //Second, give them at random to each player, leaving 5 unused
    //return;
}
