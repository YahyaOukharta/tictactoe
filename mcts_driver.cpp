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

    Node *node;

    while ((node = mcts.select_node(&mcts.root))){
        node->b.print();
    }

    return (0);
}