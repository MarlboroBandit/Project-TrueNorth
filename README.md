
<br />
<div align="center">
    <h3 align="center">Project-TrueNorth</h3>

    <p align="center">
        A Blackjack Risk simulator CLI made with C++.
        <br />
        <a href="https://github.com/MarlboroBandit/Project-TrueNorth/tree/main/Documentation"><strong>Explore the docs</strong></a>
        <br />
        <br />
        <a href="https://github.com/MarlboroBandit/Project-TrueNorth/issues">Issues Section</a>
        .
        </p>
    </div>

## About The Project

A high-performance, stochastic simulation engine written in pure C++23 that models the financial volatility of Blackjack sessions using Monte Carlo methods. The system implements a "Basic Strategy" decision engine and a state-dependent 6-deck shoe with configurable penetration. It simulates tens of thousands of independent "lifetimes" to aggregate statistical data on variances, specifically calculating Risk of Ruin (RoR), Maximum Drawdown, and Expected Value (EV) without external dependencies.

> [!NOTE]
> This project is still in work-in-progress. Dont expect it to be perfect. Thanks.

**Built With:**
* ![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=flat&logo=c%2B%2B&logoColor=white)
* ![CMake](https://img.shields.io/badge/CMake-064F8C?style=flat&logo=cmake&logoColor=white)

### Installation

1. Clone the repo
    ```sh
    git clone https://github.com/MarlboroBandit/Project-TrueNorth.git
    ```

2. Create a build dir
    ```sh
    mkdir build && cd build
    ```
3. Compile the project
    ```sh
    cmake .. && make
    ```

## Usage

The project is pretty much hardcoded, but you can freely modify the values.

Modify it here: ***you can find it in the main.cpp file***
```cpp
const int SIM_SESSIONS = 10000;
const int HANDS_PER_SESSION = 1000;
const double STARTING_BANKROLL = 1000.0;
const double BET_UNIT = 10.0;
```
