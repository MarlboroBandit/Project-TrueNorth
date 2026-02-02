#include "header.h"
#include <algorithm>
#include <ctime>

int Card::value() const {
    if (rank >= JACK && rank <= KING) return 10;
    if (rank == ACE) return 11;
    return static_cast<int>(rank);
}

Hand::Hand(double b) : bet(b), isDoubled(false), isSurrendered(false) {}

void Hand::addCard(Card c) {
    cards.push_back(c);
}

int Hand::getValue() const {
    int sum = 0;
    int aces = 0;
    for (const auto& c : cards) {
        sum += c.value();
        if (c.rank == ACE) aces++;
    }
    while (sum > 21 && aces > 0) {
        sum -= 10;
        aces--;
    }
    return sum;
}

bool Hand::isSoft() const {
    int sum = 0;
    int aces = 0;
    for (const auto& c : cards) {
        sum += c.value();
        if (c.rank == ACE) aces++;
    }
    return (sum <= 21 && aces > 0 && (getValue() == sum));
}

bool Hand::isPair() const {
    return cards.size() == 2 && cards[0].value() == cards[1].value();
}

Shoe::Shoe() : currentPosition(0) {
    rng.seed(std::random_device{}());
    refill();
}

void Shoe::refill() {
    cards.clear();
    for (int d = 0; d < NUM_DECKS; ++d) {
        for (int s = 0; s < 4; ++s) {
            for (int r = 2; r <= 14; ++r) {
                cards.push_back({static_cast<Rank>(r), static_cast<Suit>(s)});
            }
        }
    }
    shuffle();
}

void Shoe::shuffle() {
    std::shuffle(cards.begin(), cards.end(), rng);
    currentPosition = 0;
}

Card Shoe::draw() {
    if (currentPosition >= cards.size() * (DECK_PENETRATION_PERCENT / 100.0)) {
        shuffle();
    }
    return cards[currentPosition++];
}

Action BasicStrategy::getAction(const Hand& player, Card dealerUp) {
    int pVal = player.getValue();
    int dVal = dealerUp.value();

    if (player.isPair() && player.cards.size() == 2) {
        int cardVal = player.cards[0].value();
        if (cardVal == 11) return SPLIT; // Always split Aces
        if (cardVal == 8) return SPLIT;  // Always split 8s an idea by Edward Thorp
    }

    if (player.isSoft()) {
        if (pVal >= 20) return STAND;
        if (pVal == 19) return (dVal == 6) ? DOUBLE : STAND;
        if (pVal == 18) {
            if (dVal >= 2 && dVal <= 6) return DOUBLE;
            if (dVal >= 9) return HIT;
            return STAND;
        }
        if (pVal == 17) return (dVal >= 3 && dVal <= 6) ? DOUBLE : HIT;
        if (pVal >= 15 && pVal <= 16) return (dVal >= 4 && dVal <= 6) ? DOUBLE : HIT;
        if (pVal >= 13 && pVal <= 14) return (dVal >= 5 && dVal <= 6) ? DOUBLE : HIT;
        return HIT; 
    }

    if (pVal >= 17) return STAND;
    if (pVal >= 13 && pVal <= 16) {
        return (dVal >= 2 && dVal <= 6) ? STAND : HIT;
    }
    if (pVal == 12) {
        return (dVal >= 4 && dVal <= 6) ? STAND : HIT;
    }
    if (pVal == 11) return DOUBLE;
    if (pVal == 10) return (dVal < 10) ? DOUBLE : HIT;
    if (pVal == 9) return (dVal >= 3 && dVal <= 6) ? DOUBLE : HIT;
    
    return HIT;
}

SessionResult Simulator::runSession(double bankroll, double betUnit, int maxHands) {
    double peakBankroll = bankroll;
    double maxDrawdown = 0.0;
    int hands = 0;

    while (bankroll >= betUnit && hands < maxHands) {
        hands++;
        
        Hand dealer(0);
        Hand player(betUnit);
        
        player.addCard(shoe.draw());
        dealer.addCard(shoe.draw());
        player.addCard(shoe.draw());
        dealer.addCard(shoe.draw());

        bool playerBJ = (player.getValue() == 21);
        bool dealerBJ = (dealer.getValue() == 21);

        if (playerBJ || dealerBJ) {
            if (playerBJ && dealerBJ) { /* Push */ }
            else if (playerBJ) bankroll += betUnit * BLACKJACK_PAYOUT;
            else bankroll -= betUnit;
            
            if (bankroll > peakBankroll) peakBankroll = bankroll;
            double drawdown = peakBankroll - bankroll;
            if (drawdown > maxDrawdown) maxDrawdown = drawdown;
            continue;
        }

        bool bust = false;
        while (true) {
            Action act = BasicStrategy::getAction(player, dealer.cards[0]);
            
            if (act == SPLIT) act = HIT;

            if (act == DOUBLE) {
                if (bankroll >= betUnit) {
                    player.bet *= 2;
                    player.addCard(shoe.draw());
                    if (player.getValue() > 21) bust = true;
                    break;
                } else {
                    act = HIT;
                }
            }
            
            if (act == HIT) {
                player.addCard(shoe.draw());
                if (player.getValue() > 21) {
                    bust = true;
                    break;
                }
            } else if (act == STAND) {
                break;
            }
        }

        if (!bust) {
            while (dealer.getValue() < 17 || (DEALER_HITS_SOFT_17 && dealer.isSoft() && dealer.getValue() == 17)) {
                dealer.addCard(shoe.draw());
            }
        }

        if (bust) {
            bankroll -= player.bet;
        } else {
            int dTotal = dealer.getValue();
            int pTotal = player.getValue();

            if (dTotal > 21 || pTotal > dTotal) {
                bankroll += player.bet;
            } else if (dTotal > pTotal) {
                bankroll -= player.bet;
            }
        }

        if (bankroll > peakBankroll) peakBankroll = bankroll;
        double drawdown = peakBankroll - bankroll;
        if (drawdown > maxDrawdown) maxDrawdown = drawdown;
    }

    return { bankroll, maxDrawdown, bankroll < betUnit, hands };
}
