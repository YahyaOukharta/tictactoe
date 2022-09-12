#include "micro.hpp"
#include "minimax.hpp"
#include "mcts.hpp"
#include <vector>
#include <chrono>

using namespace std;

using namespace std::chrono;
using namespace std::chrono;

#define ALPHA_BETA_PRUNING 1

int main()
{
    Microboard b;
    while (b.status == 3)
    {
        b.print();

        steady_clock::time_point start = high_resolution_clock::now();

        int bestMove;
        if (ALPHA_BETA_PRUNING)
            bestMove = getBestMoveAB(b); // get bestMove
        else
            bestMove = getBestMove(b);

        steady_clock::time_point stop = high_resolution_clock::now();
        microseconds duration = duration_cast<microseconds>(stop - start);

        cout << "Time taken by function: "
            << duration.count() << " microseconds" << endl;
    
        b.move(bestMove);
    }
    b.print();

    return (0);
}