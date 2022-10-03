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
    MCTS mcts(b, b.playerToPlay);
    int i = 0;
    while (b.status() == IN_PROGRESS)
    {
        time_point<steady_clock, nanoseconds> start = high_resolution_clock::now();
        //b.print();
        //b.print();
        mcts.searchUntil(1e5);

        b.move(mcts.bestMove());
        //b.print();

        mcts.update_root(b, b.playerToPlay);
        //b.print();
        time_point<steady_clock, nanoseconds> stop = high_resolution_clock::now();
        cout << duration_cast<microseconds>(stop - start).count() / 1000 << "ms" << endl;
    
    }
    b.print();
return (0);
}