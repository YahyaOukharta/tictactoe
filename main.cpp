#include "micro.hpp"
#include "minimax.hpp"
#include <vector>

using namespace std;

int main()
{
    Microboard b;
    while (b.status == 3)
    {
        b.print();

        int m = getBestMove(b);

        b.move(m);
    }
    b.print();

    return (0);
}