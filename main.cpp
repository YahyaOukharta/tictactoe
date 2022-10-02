#include "Board.hpp"
using namespace std;
int main(){

    Microboard b;
    b.b.boards[0] = 0b100000000;
    b.b.boards[1] = 0b100001000;
    cerr << b.b.hash<<endl;
    cout << b.getStatus() << endl;
    return (0);
}