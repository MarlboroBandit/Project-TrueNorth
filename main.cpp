#include <iostream>
#include <iomanip>
#include "header.h"

const int SIM_SESSIONS = 10000;
const int HANDS_PER_SESSION = 1000;
const double STARTING_BANKROLL = 1000.0;
const double BET_UNIT = 10.0;

int main() {
    Simulator sim;
    std::cout << "--- Blackjack Risk Simulator (Multi-file C++) ---\n";
    std::cout << "Sessions: " << SIM_SESSIONS << "\n";
    std::cout << "Hands per Session: " << HANDS_PER_SESSION << "\n";
    std::cout << "Bankroll: $" << STARTING_BANKROLL << "\n";
    std::cout << "Bet Unit: $" << BET_UNIT << "\n";
    std::cout << "----------------------------------------------\nRunning simulation...\n";

    int ruins = 0;
    double totalDrawdown = 0;
    double totalFinalBankroll = 0;
    double worstDrawdown = 0;

    for (int i = 0; i < SIM_SESSIONS; ++i) {
        SessionResult res = sim.runSession(STARTING_BANKROLL, BET_UNIT, HANDS_PER_SESSION);
        
        if (res.ruined) ruins++;
        totalDrawdown += res.maxDrawdown;
        totalFinalBankroll += res.finalBankroll;
        if (res.maxDrawdown > worstDrawdown) worstDrawdown = res.maxDrawdown;
        
        if (i % (SIM_SESSIONS / 10) == 0) std::cout << "." << std::flush;
    }
    std::cout << "\nDone.\n";

    double ror = (double)ruins / SIM_SESSIONS * 100.0;
    double avgDrawdown = totalDrawdown / SIM_SESSIONS;
    double avgFinal = totalFinalBankroll / SIM_SESSIONS;
    double netProfit = avgFinal - STARTING_BANKROLL;
    
    // EV Calculation
    double profitPerHand = netProfit / HANDS_PER_SESSION;
    double houseEdgeApprox = -(profitPerHand / BET_UNIT) * 100.0;

    std::cout << "\n--- Risk Report ---\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Risk of Ruin (RoR):  " << ror << "%\n";
    std::cout << "Avg Max Drawdown:    $" << avgDrawdown << "\n";
    std::cout << "Worst Drawdown Seen: $" << worstDrawdown << "\n";
    std::cout << "Average Ending Bank: $" << avgFinal << " (Profit: " << (netProfit > 0 ? "+" : "") << netProfit << ")\n";
    std::cout << "Simulated House Edge: " << houseEdgeApprox << "% (Approx)\n";

    return 0;
}
