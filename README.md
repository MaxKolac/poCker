# poCker
A text-based customizable game of Texas Hold 'em Poker running within a console with rules based on [Rules of Hold 'em Poker for Computers (and people too!)](https://www.cs.rochester.edu/~ferguson/poker/rules.html) by George Ferguson and Chris Brown.
A backup of the linked page is available in the root folder of this repo [here](https://github.com/MaxKolac/poCker/blob/main/rules-guide.html).

Project I started in order to learn the C language through a practical crash course.

## Settings

Some things can be adjusted after starting the program such as:
 - The amount of the big blind (and subsequently, the small blind)
 - Whether or not the game will have fixed limits or no limits on betting
 - Initial amount of funds per player
 - Number of AI and human players

Some things are also hard-coded such as:
 - The game doesn't consider some suits as stronger than other - all suits are treated equally. It's a common poker myth.
 - The rule of "cards speak" at showdown.
 - There are only big blind and small blind without any antes.
 - Fixed maximum amount of raises per betting round.
 - Ace is considered stronger than King.
 - Only one full deck of cards is used (52 cards) with no jokers.

## Rules

A single game of Texas Hold 'em Poker consists of 4 betting rounds - pre-flop, flop, turn and river.
Each game has a so called "big blind amount" - this is the amount that a "big blind player" is forced to chip into the pot to create a bet and ensure that the pot contains any funds. There's also the "small blind player" and "small blind amount" which is the big blind divided by two and rounded down.

A game can be fixed-limit or no-limit. Fixed limit means that each player can raise the current bet only by a fixed amount.
The bet can only be raised by the small blind amount in the first two rounds and only by the big blind amount in the two latter rounds.
No limit means that players are free to raise the bet by any amount, including the ability to bet all of their funds.
In each betting round, the bet can be raised no more than 3 times.

At the beginning of the game, each player is dealt 2 cards face-down. These are referred to as 'hole cards'.
One player is given the dealer's button. This button signifies that the next player clockwise is designated as small blind player, and the next after him, big blind player.
The first betting round, pre-flop, begins with small blind player making a forced bet of small blind amount and the big blind player making a forced bet of big blind amount.
The player after the big blind can now choose one of the following actions:
 - CALL/CHECK - Throw enough of his funds into the pot to match the current bet amount and stay in the game. Check is a variant of calling when no bet was placed.
 - RAISE - Throw more funds into the pot than the current bet, setting a new greater bet which everyone else need to either match or fold.
 - FOLD - Give up his cards and forfeit any winning in the current game. A folded player will not need to match any raises or bets and can't act until after the showdown.
 - TAPOUT - Special case of a check, where the current bet is greater than player's available funds. They can tapout to stay in the game and not need to match any future bets, however in the case of winning, tapped out player can only win as much as the pot held at the time of them tapping out.

This continues for each subsequent player until everyone match the bet, fold their cards or tapout.
The forced bet of the small blind doesn't count as an action. Once it's their turn to act, they need to match the current bet.
If noone raised the bet in the meantime, big blind player does not need to match his own forced bet.

Once the pre-flop round is concluded, the dealer places 3 cards face-up. These are called 'community cards'.
Flop and each next round starts with the small blind player being able to act first.
After the pre-flop, there are no enforced bets. If no player raises the bet from 0, no funds will be thrown into the pot.
Each betting round concludes if the same condition is met as in the first round - everyone matched the bet (if anyone raised) or tapped out.
If everyone but one player have folded, that player automatically wins the pot without showing their cards.
With each round, the dealer also deals one additional community card, up to maximum of 5.

Once the four betting rounds conclude, players show their cards. This is called the "showdown".
The highest ranking hand of 5 cards, consisting of any combination of hole and community cards, wins the whole pot.
This game is hard-coded to the rule of "cards speak", where players don't need to announce what they believe is the strongest combination.
The opposite of that rule is "players speak", where there is a possibility of a player missing a hand ranking in their cards stronger than the one they announce.
This is the official hand rankings, from the strongest to the weakest:

| Name | Description |
| --- | --- |
| Royal Flush | The best hand possible. It consists of A, K, Q, J and 10, all of the same suit. |
| Straight Flush |  Straight Flush is the 2nd best hand possible. It consists of 5 cards of consecutive value and of the same suit. |
| Four of a Kind |  Four Of a Kind is a strong hand consisting of 4 cards of equal value. |
| Full House | Full House is a hand consisting of 3 cards of one value and 2 cards of another. |
| Flush | Flush is a hand consisting of 5 cards of the same suit. |
| Straight | Straight is a hand which has 5 cards of consecutive value, but of different suits. |
| Three of a Kind | Three Of a Kind is a hand which consists of 3 cards of equal value. |
| Two Pair | Two Pair is a hand which consists of 2 sets of 2 cards of equal value. |
| Pair | Pair is a hand which consists of 2 cards of equal value. |
| High Card | High Card is the weakest hand possible where a combination of player's and community cards do not interact to create a stronger hand. |

Source: [Upswing Poker](https://upswingpoker.com/poker-rules/)

If there is more than 1 player with hands of the same rank, there are tie-breaker rules specific to each rank.

| Name | Tie-breaker rules |
| --- | --- |
| Royal Flush | While ties are very unlikely, they are unbreakable. Two or more players having a Royal Flush results in an instant tie. |
| Straight Flush | While ties are unlikely, in the event of one happening, highest card wins. If two or more players have Straight Flushes with same highest cards, a tie becomes unbreakable. |
| Four of a Kind | In the event of a tie between players, the highest value of those four cards that make up the rank wins.  If two or more players have Four of a Kind consisting of same cards, highest 5th kicker card wins.  If the kicker card could not resolve a tie, it becomes unbreakable. |
| Full House | In the event of a tie between players, the highest value of the card making up the "trio" wins.  If two or more players have a Full House with the same "trio", the highest value of the remaining pair wins.  If the card pair could not resolve a tie, it becomes unbreakable. |
| Flush | In the event of a tie between players, the highest card in the Flush wins.  Rule of the highest card continues through the rest of the cards in descending order, in case a tie is still unbroken.  If two or more players have cards of exact same value, tie becomes unbreakable. |
| Straight | In the event of a tie between players, the highest card in the Straight wins. If two or more players have cards of exact same value, tie becomes unbreakable. |
| Three of a Kind | In the event of a tie between players, the higher card making up the TOaK wins. In a game with only one full deck of playing cards, it's impossible for 2 players to have a Three Of a Kind consisting of cards with the same value. |
| Two Pair | In the event of a tie between players, the higher card pair wins. If the higher pairs are the same, the higher second pair wins. If both second pairs are the same, the highest value of the 5th kicker card wins. If even the kicker card is of the same value, the tie becomes unbreakable.  |
| Pair | In the event of a tie between players, the higher card pair wins. If two or more players have a Pair of the same value, the highest card from the remaining 3 cards wins. If the highest cards are the same, the second highest determines the winner. Same for the lowest card. If two or more players have a set of cards with the exact same values, tie becomes unbreakable. |
| High Card | In the event of a tie between players, the highest card wins.  Rule of the highest card continues through the rest of the cards in descending order, in case a tie is still unbroken.  If two or more players have cards of exact same value, tie becomes unbreakable. |

If the tie couldn't be broken, the pot is split evenly between the tied players. Any remaining funds from an uneven split of the pot are given to the small blind player.

Once one full game has been played, the dealer button travels by 1 player clockwise and a new game begins.
This loop continues until all but one player is left with funds. In real-life, players can quit or join the table between games.


