#ifndef BOARD_HPP
#define BOARD_HPP
#include <iostream>
#include <bitset>
using namespace std;

enum PlayerId{
    PLAYER_X,
    PLAYER_O
};

enum GameStatus{
    X_WON,
    O_WON,
    TIE,
    IN_PROGRESS
};

union Board
{
    unsigned int hash;
    bitset<9> boards[2];
    Board(){}
};

static unsigned int win_masks[8] = {0x7, 0x38, 0x1c0, 0x49, 0x92, 0x124, 0x111, 0x54};

class Microboard {
    public:
    Board b;

    GameStatus getStatus(){
    
        for (int i = 0; i < 8; ++i){
            if ((b.boards[PLAYER_X].to_ulong() & win_masks[i]) == win_masks[i])
                return X_WON;
            else if((b.boards[PLAYER_O].to_ulong() & win_masks[i]) == win_masks[i])
                return O_WON;
        }
        if ((b.boards[PLAYER_X] | b.boards[PLAYER_O]) == 0b111111111)
            return TIE;
        return IN_PROGRESS;
    }

};

#endif