/*********************************************************
# File Name: a.c
# Author: Vincent Yang
# Mail: mengxian0913@gmail.com
# Created Time: 四  5/ 4 18:19:43 2023
*********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


const char *RANKS[13] = {
    "Ace",
    "Two",
    "Three",
    "Four",
    "Five",
    "Six",
    "Seven",
    "Eight",
    "Nine",
    "Ten",
    "Jack",
    "Queen",
    "King",
};

typedef enum suit {
    CLUBS,
    DIAMONDS,
    HEARTS,
    SPADES,
} Suit;

Suit suit_from_string(char *str) {
    if (strcmp(str, "Spades") == 0) {
        return SPADES;
    } else if (strcmp(str, "Hearts") == 0) {
        return HEARTS;
    } else if (strcmp(str, "Diamonds") == 0) {
        return DIAMONDS;
    } else {
        return CLUBS;
    }
}

typedef struct card {
    int number;
    Suit suit;
} Card;

int cmp_card(const void *l, const void *r) {
    Card *card1 = (Card*) l, *card2 = (Card*) r;

    if (card1->number != card2->number) {
        return card1->number - card2->number;
    } else {
        return card1->suit - card2->suit;
    }
}

int cmp_cards_by_point(Card cards1[5], Card cards2[5]) {
    int max1 = 0, max2 = 0, holder;

    for (int i = 0; i < 5; i++) {
        holder = cards1[i].number;

        if (holder == 1) {
            holder = 14;
        }

        if (max1 < holder) {
            max1 = holder;
        }

        holder = cards2[i].number;

        if (holder == 1) {
            holder = 14;
        }

        if (max2 < holder) {
            max2 = holder;
        }
    }

    return max1 - max2;
}

int cmp_cards_by_suit(Card cards1[5], Card cards2[5]) {
    Suit max1 = cards1[0].suit, max2 = cards2[0].suit;

    for (int i = 1; i < 5; i++) {
        if (max1 < cards1[i].suit) {
            max1 = cards1[i].suit;
        }
        if (max2 < cards2[i].suit) {
            max2 = cards2[i].suit;
        }
    }

    return max1 - max2;
}

Card accept_card() {
    char rank[100], suit[100];
    scanf("%s of %s", rank, suit);
    Card card = { 0 };
    for (int i = 0; i < 13; i++) {
        if (strcmp(RANKS[i], rank) == 0) {
            card.number = i + 1;
        }
    }
    card.suit = suit_from_string(suit);
    return card;
}

typedef enum hands {
    HIGH_CARD,
    ONE_PAIR,
    TWO_PAIRS,
    THREE_OF_A_KIND,
    STRAIGHT,
    FLUSH,
    FULL_HOUSE,
    FOUR_OF_A_KIND,
    STRAIGHT_FLUSH,
} Hands;

Hands resolve_hands(Card cards[5]) {
    straight_flush: {
        Suit first_suit = cards[0].suit;
        for (int i = 1; i < 5; i++) {
            if (cards[i].number - cards[i - 1].number != 1) {
                goto four_of_a_kind;
            }
            if (cards[i].suit != first_suit) {
                goto four_of_a_kind;
            }
        }
        return STRAIGHT_FLUSH;
    }
    four_of_a_kind: {
        int predicted_number = cards[0].number;
        int same_count = 0;

        for (int i = 1; i < 5; i++) {
            if (cards[i].number != predicted_number) {
                predicted_number = cards[i].number;
                same_count = 0;
            } else {
                same_count++;
            }
        }

        if (same_count == 4) {
            return FOUR_OF_A_KIND;
        }
    }
    full_house: {
        bool front_seq = false;

        if (cards[2].number == cards[1].number) {
            front_seq = true;
        }

        if (front_seq) {
            // Front must have 3 same numbers
            int front = cards[0].number, back = cards[3].number;
            for (int i = 1; i < 3; i++) {
                if (cards[i].number != front) {
                    goto flush;
                }
            }
            for (int i = 4; i < 5; i++) {
                if (cards[i].number != back) {
                    goto flush;
                }
            }
        } else {
            // Back must have 3 same numbers
            int front = cards[0].number, back = cards[2].number;
            for (int i = 1; i < 2; i++) {
                if (cards[i].number != front) {
                    goto flush;
                }
            }
            for (int i = 3; i < 5; i++) {
                if (cards[i].number != back) {
                    goto flush;
                }
            }
        }

        return FULL_HOUSE;
    }
    flush: {
        int predicted_suit = cards[0].suit;

        for (int i = 1; i < 5; i++) {
            if (cards[i].suit != predicted_suit) {
                goto straight;
            }
        }

        return FLUSH;
    }
    straight: {
        for (int i = 1; i < 5; i++) {
            if (cards[i].number - cards[i - 1].number != 1) {
                goto three_of_a_kind;
            }
        }

        return THREE_OF_A_KIND;
    }
    three_of_a_kind: {
        bool front_seq = false;

        if (cards[2].number == cards[1].number) {
            front_seq = true;
        }

        if (front_seq) {
            // Front must have 3 same numbers
            int front = cards[0].number;
            for (int i = 1; i < 3; i++) {
                if (cards[i].number != front) {
                    goto two_pairs;
                }
            }
        } else {
            // Back must have 3 same numbers
            int back = cards[2].number;
            for (int i = 3; i < 5; i++) {
                if (cards[i].number != back) {
                    goto two_pairs;
                }
            }
        }

        return THREE_OF_A_KIND;
    }
    two_pairs: {
        // Used to decide where pairs should be.
        // 0 indicates on both sides.
        // 1 indicates preferring back.
        // 2 indicates preferring front.
        int seq_dir = 0;

        if (cards[2].number == cards[1].number) {
            seq_dir -= 1;
        } else if (cards[2].number == cards[3].number) {
            seq_dir += 1;
        }

        if (seq_dir == 0) {
            if (cards[0].number == cards[1].number && cards[3].number == cards[4].number) {
                return TWO_PAIRS;
            }
        } else if (seq_dir == 1) {
            if (cards[0].number == cards[1].number && cards[2].number == cards[3].number) {
                return TWO_PAIRS;
            }
        } else if (seq_dir == -1) {
            if (cards[1].number == cards[2].number && cards[3].number == cards[4].number) {
                return TWO_PAIRS;
            }
        }
    }
    one_pairs: {
        for (int i = 1; i < 5; i++) {
            // Same ranks must be besides each other
            if (cards[i].number == cards[i - 1].number) {
                return ONE_PAIR;
            }
        }
    }
    high_card: {
        return HIGH_CARD;
    }
}

typedef struct hand {
    Card cards[5];
} Hand;

Hand accept_hand() {
    Hand hand = { 0 };
    for (int i = 0; i < 5; i++) {
        hand.cards[i] = accept_card();
    }
    qsort(hand.cards, 5, sizeof(Card), cmp_card);
    return hand;
}

int main(void) {
    int games;

    scanf("%d", &games);

    while (getc(stdin) != '\n');

    for (int i = 0; i < games; i++) {
        Hand hand1 = accept_hand(), hand2 = accept_hand();
        Hands hands1 = resolve_hands(hand1.cards), hands2 = resolve_hands(hand2.cards);

        // for (int j = 0; j < 2; j++) {
        //     printf("Player %d\n", j);
        //     for (int k = 0; k < 5; k++) {
        //         Card card = j == 0 ? hand1.cards[k] : hand2.cards[k];
        //         printf("\tCard: %d %d\n", card.number, card.suit);
        //     }
        //     printf("%d\n", j == 0 ? hands1 : hands2);
        // }

        if (hands1 == hands2) {
            // Cards are already sorted, we can directly
            // compare them.
            int cmp = cmp_cards_by_point(hand1.cards, hand2.cards);

            if (cmp >= 1) {
                printf("Player 1 Win the Game.");
            } else if (cmp <= -1) {
                printf("Player 2 Win the Game.");
            } else {
                cmp = cmp_cards_by_suit(hand1.cards, hand2.cards);

                if (cmp >= 1) {
                    printf("Player 1 Win the Game.");
                } else if (cmp <= -1) {
                    printf("Player 2 Win the Game.");
                }
            }
        } else {
            if (hands1 > hands2) {
                printf("Player 1 Win the Game.");
            } else {
                printf("Player 2 Win the Game.");
            }
        }

        puts("");
        puts("");
    }

    return 0;
}
