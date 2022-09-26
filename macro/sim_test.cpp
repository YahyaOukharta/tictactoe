#include "macro.hpp"
#include <random>
#include <chrono>

using namespace std::chrono;
using namespace std::chrono::_V2;

int main(void){

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(1,1e6);

    Macroboard b;
    //b.print();
    int turns = 1;

    time_point<system_clock, nanoseconds> start = high_resolution_clock::now();
    while(b.status() == 3)
    {
        vector<int> moves = b.available_moves();
        int randIdx = dist(rng) % moves.size();
        b.move(moves[randIdx]);
        //b.print();
        turns++;
    }
    //b.print();
    time_point<system_clock, nanoseconds> stop = high_resolution_clock::now();

    cout <<"done after "<< turns <<" turns and " <<duration_cast<microseconds>(stop - start).count()<<"us"<< endl;


    return (0);
}