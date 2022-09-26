#include "micro.hpp"
#include "minimax.hpp"
#include "mcts.hpp"
#include <vector>
#include <chrono>

using namespace std;

using namespace std::chrono;
using namespace std::chrono::_V2;


int main()
{
    MCTS mcts;

    Microboard b;
    int i = 0;

    while (1)
    {
        time_point<system_clock, nanoseconds> start = high_resolution_clock::now();

        PlayerId player = i%2 ? PLAYER_O : PLAYER_X;
        int move = mcts.search(b, 1000);

        time_point<system_clock, nanoseconds> stop = high_resolution_clock::now();
        milliseconds duration = duration_cast<milliseconds>(stop - start);

        cout << "Time taken by function: "
            << duration.count() << " ms" << endl;

        b.move(move);
        b.print();


        i++;
        if (b.status < 3){

            if (b.status != 2){
                cout << "something weird is going on "<<endl;
                exit(1);
            }
            b = Microboard();
            i = 0;

        }
    }



    return (0);
}