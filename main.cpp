#include "Board.hpp"
#include <random>
#include <chrono>
#pragma GCC optimize("Ofast", "unroll-loops", "omit-frame-pointer", "inline")
#pragma GCC option("arch=native", "tune=native", "no-zero-upper")
#pragma GCC target("rdrnd", "popcnt", "avx", "bmi2")
using namespace std;
using namespace std::chrono;

random_device dev;
mt19937 rng(dev());
uniform_int_distribution<mt19937::result_type> dist(1, 1e6);
int main()
{

    time_point<system_clock, nanoseconds> start = high_resolution_clock::now();
    for (int j = 0; j < 2e5; j++)
    {
        Macroboard b;
        int i = 0;
        while (b.status() == IN_PROGRESS)
        {
           //b.print();
            //b = Macroboard(b ,b.moves[dist(rng)%b.moves.size()]);
            b.move(b.moves[dist(rng) % b.moves.size()]);
            i++;
        }
    }

    // b.print();

    time_point<system_clock, nanoseconds> stop = high_resolution_clock::now();
    cout << duration_cast<microseconds>(stop - start).count()/1000 << "ms" << endl;
    return (0);
}