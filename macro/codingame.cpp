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
                    cerr << "X ";
                else if ((oboard & maskOfMove(y*3+x)))
                    cerr << "O ";
                else 
                    cerr << "- ";
            }   
            cerr << endl;
        }

        vector<int> moves = available_moves();
        for (int i = 0;i<moves.size(); ++i){
            cerr << moves[i] << " ";
        }
        cerr << endl;
        cerr << "status = " << status << endl << endl;
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
        if (next_micro != -1)
            return next_micro;
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
                    cerr << (b.status == (int)PLAYER_X ? "X " : "O ");
                else if ((b.xboard & b.maskOfMove(i%3 * 3 + j%3)))
                    cerr << "x ";
                else if ((b.oboard & b.maskOfMove(i%3 * 3 + j%3)))
                    cerr << "o ";
                else 
                    cerr << "- ";
            }
            cerr <<endl;
        }
        cerr << "player To play " << (playerToPlay == PLAYER_X ? "X" : "O") << endl;
        cerr << " game  status  " << status() << endl;

        cerr << "  next micro   " << next_micro << endl;
        if (next_micro == -1)
            for(auto m : available_moves())
                cerr << dehash_board(m) << "/" << dehash_move(m) << " ";
        else
            for(auto m : available_moves())
                cerr << m << " ";
        cerr <<endl;
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
        if (b.status() == (int)me) return 1;
        return -1;
    }

    int getBestMove(){
        return root->bestMove();
    }
};

int main()
{

    // game loop

    int turn = 0;
    PlayerId me = PLAYER_X;
    Macroboard b;
    while (1) {
        int op_y;
        int op_x;
        cin >> op_y >> op_x; cin.ignore();
        if (op_y != -1 && turn == 0)
            me = PLAYER_O;

        cerr << op_x << " " <<op_y << endl;
        int x,y;
        int bx,by;
        if (op_y != -1)
        {
            x = op_x%3;
            y = op_y%3;
            bx = op_x/3;
            by = op_y/3;
            //b.print();

            cerr << x << " " << y << "-" << bx << " " << by << "-" << by*3 + bx << " " << x+y*3 << endl;
            if (b.next_micro == -1)
                b.move(b.hash_move(by*3 +bx, x+y*3));
            else
                b.move(x+y*3);
            //b.print();
        }

        int valid_action_count;
        cin >> valid_action_count; cin.ignore();
        for (int i = 0; i < valid_action_count; i++) {
            int row;
            int col;
            cin >> row >> col; cin.ignore();
        }

        MCTS mcts(b);
        mcts.searchUntil(me, turn == 0 ? 1e6 : 100000);
        int hash = mcts.getBestMove();

        int board = b.dehash_board(hash);
        int move = b.dehash_move(hash);
        cerr << board << " " << move <<endl;
        cout << board / 3 * 3 + move / 3 <<" "<< board % 3 * 3 + move % 3 << endl;

        b.move(hash);
        b.print();
        // cout << "0 0" << endl;
        turn++;
    }
}