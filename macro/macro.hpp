#ifndef __MACRO_BOARD_HPP__
#define __MACRO_BOARD_HPP__
#include <iostream>
#include <cstring>
#include <vector>
#include <unordered_map>
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
    int lastMove;
    Microboard() // fresh board
    {
        lastMove = -1;
        xboard = 0b000000000;
        oboard = 0b000000000;
        status = IN_PROGRESS;
    }

    Microboard(Microboard const &b) // cloning
    {
        lastMove = b.lastMove;
        xboard = b.xboard;
        oboard = b.oboard;
        status = b.status;
    }

    ~Microboard()
    {
    }

    int maskOfMove(int m) const
    {
        return 1 << (8 - m);
    }

    vector<int> available_moves() const
    {
        vector<int> moves;
        if (status < 3) return moves;
        int full_board = xboard | oboard;

        for (int i = 0; i < 9; ++i)
        {
            if ((full_board & maskOfMove(i)) == 0)
                moves.push_back(i);
        }
        return moves;
    }

    void move(int m, PlayerId playerToPlay)
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
        lastMove = m;
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

        vector<int> moves = available_moves();
        for (int i = 0;i<moves.size(); ++i){
            cout << moves[i] << " ";
        }
        cout << endl;
        cout << "status = " << status << endl << endl;
    }
};

class Macroboard
{
    public:
    Microboard  macro;
    Microboard  micro[9];
    
    PlayerId    playerToPlay;
    int         next_micro;

    Macroboard(){
        playerToPlay = PLAYER_X;
        next_micro = -1;
    }
    Macroboard(Macroboard const &b){
        memcpy(this,&b,sizeof(b));
    }

    vector<int> available_moves()const {
        if (next_micro != -1){
            return micro[next_micro].available_moves();
        }
        vector<int> moves;
        moves.reserve(81);
        for (int i = 0; i < 9; i++){
            vector<int>micro_moves = micro[i].available_moves();
            for(auto m : micro_moves)
                moves.push_back(hash_move(i, m));
        }
        return moves;
    }

    int hash_move(int micro, int move)const {
        return 13 * micro + move;
    }
    int dehash_move(int hash) const {
        return hash % 13;
    }
    int dehash_board(int hash) const {
        return hash / 13;
    }

    void move(int hash)
    {
        Microboard &b = micro[next_micro == -1 ? dehash_board(hash) : next_micro];
        int move = next_micro == -1 ? dehash_move(hash) : hash;

        b.move(move, playerToPlay);
        if (b.status < 2)
            macro.move(move, (PlayerId)b.status);
        next_micro = move;
        if (micro[next_micro].status < 3)
            next_micro = -1;
    }

    void print() const {
        for (int i = 0; i < 9; i++) {//rows
            for (int j = 0; j < 9; j++) { // cols.
                int idx = (j / 3)+i/3*3;
                Microboard const &b = micro[idx];
                if (b.status < 2)
                    cout << (b.status == (int)PLAYER_X ? "X " : "O ");
                else if ((b.xboard & b.maskOfMove(i%3 * 3 + j%3)))
                    cout << "x ";
                else if ((b.oboard & b.maskOfMove(i%3 * 3 + j%3)))
                    cout << "o ";
                else 
                    cout << "- ";
            }
            cout <<endl;
        }
        cout << "player To play " << (playerToPlay == PLAYER_X ? "X" : "O") << endl;
        cout << " game  status  " << macro.status << endl;

        cout << "  next micro   " << next_micro << endl;
        if (next_micro == -1)
            for(auto m : available_moves())
                cout << dehash_board(m) << "/" << dehash_move(m) << " ";
        else
            for(auto m : available_moves())
                cout << m << " ";
        cout <<endl;
    }

};

#endif