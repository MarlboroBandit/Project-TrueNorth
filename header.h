#ifndef BLACKJACK_H
#define BLACKJACK_H

#include <vector>
#include <random>

constexpr int DECK_PENETRATION_PERCENT = 75;
constexpr double BLACKJACK_PAYOUT = 1.5;
constexpr bool DEALER_HITS_SOFT_17 = true;

enum Rank { TWO = 2, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, ACE };
enum Suit { CLUBS, DIAMONDS, HEARTS, SPADES };
enum Action { HIT, STAND, DOUBLE, SPLIT };

struct Card {
    Rank rank;
    Suit suit;
    int value() const; 
};

struct Hand {
    std::vector<Card> cards;
    double bet;
    bool isDoubled;
    bool isSurrendered;

    Hand(double b);
    void addCard(Card c);
    int getValue() const;
    bool isSoft() const;
    bool isPair() const;
};

struct SessionResult {
    double finalBankroll;
    double maxDrawdown;
    bool ruined;
    int handsPlayed;
};

class Shoe {
private:
    std::vector<Card> cards;
    int currentPosition;
    std::mt19937 rng;

public:
    Shoe();
    void refill();
    void shuffle();
    Card draw();
};

class BasicStrategy {
public:
    static Action getAction(const Hand& player, Card dealerUp);
};

class Simulator {
    Shoe shoe;
public:
    SessionResult runSession(double bankroll, double betUnit, int maxHands);
};

#endif
