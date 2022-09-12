#ifndef __MICRO_BOARD_HPP__
#define __MICRO_BOARD_HPP__
#include <iostream>
#include <cstring>
#include <vector>

using namespace std;

enum PlayerId
{
    PLAYER_X,
    PLAYER_O
};

enum GameStatus
{
    X_WON,
    O_WON,
    TIE,
    IN_PROGRESS
};

static int win_masks[8] = {0x7, 0x38, 0x1c0, 0x49, 0x92, 0x124, 0x111, 0x54};

class Microboard
{
public:
    int xboard;
    int oboard;
    GameStatus status;
    PlayerId playerToPlay;

    Microboard() // fresh board
    {
        xboard = 0b000000000;
        oboard = 0b000000000;
        status = IN_PROGRESS;
        playerToPlay = PLAYER_X;
    }

    Microboard(Microboard const &b) // cloning
    {
        xboard = b.xboard;
        oboard = b.oboard;
        status = b.status;
        playerToPlay = b.playerToPlay;
    }
    Microboard(Microboard const &b, int m) // cloning and applying bove
    {
        xboard = b.xboard;
        oboard = b.oboard;
        status = b.status;
        playerToPlay = b.playerToPlay;
        move(m);
    }
    ~Microboard() // cloning and applying bove
    {
    }

    int maskOfMove(int m) const
    {
        return 1 << (8 - m);
    }

    vector<int> available_moves() const
    {
        vector<int> moves;
        int full_board = xboard | oboard;

        for (int i = 0; i < 9; ++i)
        {
            if ((full_board & maskOfMove(i)) == 0)
                moves.push_back(i);
        }
        return moves;
    }

    void move(int m)
    {
        if (playerToPlay == PLAYER_X)
        {
            xboard += maskOfMove(m);
            playerToPlay = PLAYER_O;
        }
        else
        {
            oboard += maskOfMove(m);
            playerToPlay = PLAYER_X;
        }
        status = getStatus();
    }

    GameStatus getStatus() const {
        for (int i = 0; i < 8; ++i){
            if ((xboard & win_masks[i]) == win_masks[i])
                return X_WON;
            else if((oboard & win_masks[i]) == win_masks[i])
                return O_WON;
        }
        if (available_moves().size() == 0) return TIE;
        return IN_PROGRESS;
    }

    void print() const {
        for (int y = 0; y < 3; ++y){
            for (int x = 0; x < 3; ++x){
                if ((xboard & maskOfMove(y*3+x)))
                    cout << "X ";
                else if ((oboard & maskOfMove(y*3+x)))
                    cout << "O ";
                else 
                    cout << "- ";
            }   
            cout << endl;
        }
        cout << "playerToPlay = " << (playerToPlay == PLAYER_X ? "X" : "O") << endl;

        vector<int> moves = available_moves();
        for (int i = 0;i<moves.size(); ++i){
            cout << moves[i] << " ";
        }
        cout << endl;
        cout << "status = " << status << endl << endl;
    }
};

#endif