#ifndef __MACRO_BOARD_HPP__
#define __MACRO_BOARD_HPP__
#include <iostream>
#include <cstring>
#include <cmath>
#include <vector>
#include <unordered_map>
#include <random>
#include <chrono>
using namespace std;
using namespace std::chrono;
using namespace std::chrono::_V2;
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

    void setStatus(GameStatus st){
        status = st;
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
    vector<int> moves; // only generated after construction

    Macroboard(){
        playerToPlay = PLAYER_X;
        next_micro = -1;
        moves = available_moves();//generating moves

    }
    Macroboard(Macroboard const &b){
        memcpy(this,&b,sizeof(b) - sizeof(moves));

        moves = b.moves;//generating moves
    }
    GameStatus status() const {return macro.status;}

    void check_tie() {
        for (int i = 0; i < 9; i++){
            if (micro[i].status == 3) return;
        }
        macro.status = TIE;
    }

    vector<int> available_moves()const {
        vector<int> moves;
        if (macro.status < 3)
            return moves;
        if (next_micro != -1)
            return micro[next_micro].available_moves();
        moves.reserve(81);
        for (int i = 0; i < 9; i++){
            if (micro[i].status < 3) continue;
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
        int boardIdx = next_micro == -1 ? dehash_board(hash) : next_micro;
        Microboard &b = micro[boardIdx];
        int move = next_micro == -1 ? dehash_move(hash) : hash;

        b.move(move, playerToPlay);

        if (b.status < 2)
            macro.move(boardIdx, (PlayerId)b.status);
        next_micro = move;
        if (micro[next_micro].status < 3)
            next_micro = -1;

        playerToPlay = playerToPlay == PLAYER_X ? PLAYER_O : PLAYER_X;

        check_tie();

        moves = available_moves(); //generating moves

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
        cout << " game  status  " << status() << endl;

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


class Node
{
    public:
    int fully_expanded;
    int is_terminal;
    Node *parent;
    unordered_map<int, Node *> children;
    int score = 0;
    int visits = 0;

    Macroboard b;  // add possible moves to object when move is played

    Node(Macroboard const &_b, Node *p = 0)
    {
        b = Macroboard(_b);
        // cerr <<"constructed node, moves "<<b.moves.size()<<endl;

        parent = p;
        is_terminal = b.status() < 3;
        fully_expanded = is_terminal;
    }

    Node(Macroboard const &_b, int move, Node *p = 0)
    {
        b = Macroboard(_b);
        // cerr <<"constructed node, moves "<<b.moves.size()<<endl;
        b.move(move);

        parent = p;
        is_terminal = b.status() < 3;
        fully_expanded = is_terminal;

    }

    void backpropagate(int score)
    {
        Node *current = this;
        while (current){
            current->score+=score;
            current->visits++;
            current = current->parent;
        }
    }

    Node *bestChild(float c, int f = 1)
    {
        float score = -1e6;

        Node *best = 0;
        for (auto it : children)
        {
            Node *child = it.second;
            float uct = ( (float)child->score / (float)child->visits + c * sqrtf(logf((float)visits) / (float)child->visits) ) * f;
            //cerr << "child score = " << uct << endl;
            if (uct > score)
            {
                score = uct;
                best = child;
            }
        }
        return (best);
    }

    int bestMove()
    {
        float score = -1e6;

        int best = -1;
        for (auto it : children)
        {
            Node *child = it.second;
            float uct =  (float)child->score / (float)child->visits;
            //cerr << "child score = " << uct << endl;
            if (uct > score)
            {
                score = uct;
                best = it.first;
            }
        }
        return (best);
    }

    Node *expand(){
        if (!b.moves.size())exit(1); // shouldnt be here fully expanded

        int move = b.moves.back();
        b.moves.pop_back();

        if (b.moves.size() == 0) fully_expanded = true;

        Node *child = new Node(b, move, this);

        children.insert(make_pair(move,child));
        
        return child;
    }

    Node *select(PlayerId me){
        Node *current = this;
        while (current && current->fully_expanded){
            current = current->bestChild(sqrtf(2), current->b.playerToPlay == me ? 1 : -1);
        }
        return current;
    }

    void free(){
        for (auto it : children)
            it.second->free();
        delete this;
    }

};
//duration_cast<microseconds>(stop - start).count()
class MCTS{
    public:
    Node *root;

    MCTS(Macroboard const &b){
        root = new Node(b);

    }

    ~MCTS(){
        if (root)
            root->free();
    }

    int search(PlayerId me){
        int iter_left = 1000;
        Node *n;
        while (iter_left--){
            n = root->select(me);
            if (!n){continue;}
            n = n->expand();
            int score = rollout(n, me);
            n->backpropagate(score);
        }
        return (0);
    }
    int searchUntil(PlayerId me, int us){
        int iter_left = 1000;
        Node *n;

        time_point<system_clock, nanoseconds> start = high_resolution_clock::now();
        while (duration_cast<microseconds>(high_resolution_clock::now() - start).count() < us){
            n = root->select(me);
            if (!n){continue;}
            n = n->expand();
            int score = rollout(n, me);
            n->backpropagate(score);
        }
        return (0);
    }

    int rollout(Node *n, PlayerId me){
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist(1,1e6);

        Macroboard b;
        while (b.status() == 3){
            b.move(b.moves[dist(rng)%b.moves.size()]);
        }
        if (b.status() == 2) return 0;
        if (b.status() == (int)me) return 10;
        return -10;
    }

    int getBestMove(){
        return root->bestMove();
    }
};

#endif