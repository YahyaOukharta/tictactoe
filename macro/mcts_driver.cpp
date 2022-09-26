#include "macro.hpp"


using namespace std::chrono;
using namespace std::chrono::_V2;

int main(void){
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(1,1e6);

    Macroboard b;

    // do stuff here
    while (b.status() == 3)
    {
        int move;
        if (b.playerToPlay == PLAYER_X)
        {

    time_point<system_clock, nanoseconds> start = high_resolution_clock::now();

            MCTS mcts(b);
            mcts.searchUntil(b.playerToPlay, 45000);
            move = mcts.getBestMove();
    
    time_point<system_clock, nanoseconds> stop = high_resolution_clock::now();
    cout <<"done after " <<duration_cast<microseconds>(stop - start).count()<<"us"<< endl;

        }
        else{
            move = b.moves[dist(rng)% b.moves.size()];
        }
        b.move(move);
        b.print();
    }


    return (0);
}