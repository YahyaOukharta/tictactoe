#include "Board.hpp"
#include "MCTS.hpp"
#include <random>
#include <chrono>
#pragma GCC optimize("Ofast", "unroll-loops", "omit-frame-pointer", "inline")
#pragma GCC option("arch=native", "tune=native", "no-zero-upper")
#pragma GCC target("rdrnd", "popcnt", "avx", "bmi2")
using namespace std;
using namespace std::chrono;

int main()
{

    
    Macroboard b;

    while (b.status() == IN_PROGRESS)
    {
        //time_point<system_clock, nanoseconds> start = high_resolution_clock::now();
        //b.print();
        MCTS mcts(b, b.playerToPlay);
        mcts.searchUntil(1e5);
        b.move(mcts.bestMove());
        //b.print();
        //time_point<system_clock, nanoseconds> stop = high_resolution_clock::now();
        //cout << duration_cast<microseconds>(stop - start).count() / 1000 << "ms" << endl;
    
    }
    b.print();
return (0);
}