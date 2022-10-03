#include <iostream>
#include <cstring>
#include <cmath>
#include <vector>
#include <unordered_map>
#include <random>
#include <algorithm>
#include <chrono>



#pragma GCC optimize("Ofast","unroll-loops", "omit-frame-pointer", "inline")
#pragma GCC option("arch=native", "tune=native", "no-zero-upper")
#pragma GCC target("rdrnd", "popcnt", "avx", "bmi2")

using namespace std;
using namespace std::chrono;
using namespace std::chrono::_V2;
#include <iostream>
#include <cstring>
#include <bitset>
#include <stdio.h>
#include <vector>
using namespace std;

#define FULL_BOARD 511

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

typedef int Moves;

bitset<512> win_states("11111111111111111111111111111111111111111111111111111111111111111111111111111110111100001111000011101110111011101000000010000000111111111111101011111010111100001111101011111010101010101000000011111111111110101111000011110000101010101010101010000000100000001111111111111100101010101000000011111110111111001010101010000000111111111100110010000000100000001100110011001100100000001000000011111111111100001010101010000000111110101111000010101010100000001111111110000000100000001000000010000000100000001000000010000000");
int move_masks[9] = {0b100000000, 0b010000000, 0b001000000, 0b000100000, 0b000010000, 0b00001000, 0b000000100, 0b000000010, 0b000000001};
Moves moves_available[512] = {511, 510, 509, 508, 507, 506, 505, 504, 503, 502, 501, 500, 499, 498, 497, 496, 495, 494, 493, 492, 491, 490, 489, 488, 487, 486, 485, 484, 483, 482, 481, 480, 479, 478, 477, 476, 475, 474, 473, 472, 471, 470, 469, 468, 467, 466, 465, 464, 463, 462, 461, 460, 459, 458, 457, 456, 455, 454, 453, 452, 451, 450, 449, 448, 447, 446, 445, 444, 443, 442, 441, 440, 439, 438, 437, 436, 435, 434, 433, 432, 431, 430, 429, 428, 427, 426, 425, 424, 423, 422, 421, 420, 419, 418, 417, 416, 415, 414, 413, 412, 411, 410, 409, 408, 407, 406, 405, 404, 403, 402, 401, 400, 399, 398, 397, 396, 395, 394, 393, 392, 391, 390, 389, 388, 387, 386, 385, 384, 383, 382, 381, 380, 379, 378, 377, 376, 375, 374, 373, 372, 371, 370, 369, 368, 367, 366, 365, 364, 363, 362, 361, 360, 359, 358, 357, 356, 355, 354, 353, 352, 351, 350, 349, 348, 347, 346, 345, 344, 343, 342, 341, 340, 339, 338, 337, 336, 335, 334, 333, 332, 331, 330, 329, 328, 327, 326, 325, 324, 323, 322, 321, 320, 319, 318, 317, 316, 315, 314, 313, 312, 311, 310, 309, 308, 307, 306, 305, 304, 303, 302, 301, 300, 299, 298, 297, 296, 295, 294, 293, 292, 291, 290, 289, 288, 287, 286, 285, 284, 283, 282, 281, 280, 279, 278, 277, 276, 275, 274, 273, 272, 271, 270, 269, 268, 267, 266, 265, 264, 263, 262, 261, 260, 259, 258, 257, 256, 255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244, 243, 242, 241, 240, 239, 238, 237, 236, 235, 234, 233, 232, 231, 230, 229, 228, 227, 226, 225, 224, 223, 222, 221, 220, 219, 218, 217, 216, 215, 214, 213, 212, 211, 210, 209, 208, 207, 206, 205, 204, 203, 202, 201, 200, 199, 198, 197, 196, 195, 194, 193, 192, 191, 190, 189, 188, 187, 186, 185, 184, 183, 182, 181, 180, 179, 178, 177, 176, 175, 174, 173, 172, 171, 170, 169, 168, 167, 166, 165, 164, 163, 162, 161, 160, 159, 158, 157, 156, 155, 154, 153, 152, 151, 150, 149, 148, 147, 146, 145, 144, 143, 142, 141, 140, 139, 138, 137, 136, 135, 134, 133, 132, 131, 130, 129, 128, 127, 126, 125, 124, 123, 122, 121, 120, 119, 118, 117, 116, 115, 114, 113, 112, 111, 110, 109, 108, 107, 106, 105, 104, 103, 102, 101, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
class Microboard
{
public:
    bitset<9> boards[2];
    GameStatus status;
    vector<int> moves;
    Microboard()
    {
        status = IN_PROGRESS;
        moves = available_moves();
    }

    Microboard(Microboard const &b)
    {
        boards[0] = b.boards[0];
        boards[1] = b.boards[1];
        status = b.status;
        moves = b.moves;
    }
    Microboard(Microboard const &b, PlayerId player, int _move)
    {
        boards[0] = b.boards[0];
        boards[1] = b.boards[1];
        move(player, _move);
    }

    GameStatus getStatus()
    {
        if (win_states[boards[PLAYER_X].to_ulong()])
            return X_WON;
        if (win_states[boards[PLAYER_O].to_ulong()])
            return O_WON;
        if ((boards[PLAYER_X] | boards[PLAYER_O]) == FULL_BOARD)
            return TIE;
        return IN_PROGRESS;
    }

    ulong full_board() const
    {
        return (boards[PLAYER_X] | boards[PLAYER_O]).to_ulong();
    }

    ulong hash() const
    {
        return (boards[0].to_ulong() << 9) | boards[1].to_ulong();
    }

    void move(PlayerId player, int move)
    {
        boards[player] |= move_masks[move];
        status = getStatus();
        moves = available_moves();
    }

    vector<int> available_moves() const
    {
        vector<int> moves;
        if (status != IN_PROGRESS)
            return moves;

        Moves m = moves_available[full_board()];
        moves.reserve(__builtin_popcount(m));

        int i = 0;
        while (m && i < 9)
        {
            if (m & 1)
                moves.push_back(8 - i);
            i++;
            m >>= 1;
        }
        return moves;
    }

    void print() const
    {
        cerr << "hash : " << hash() << endl;
        for (int y = 0; y < 3; y++)
        {
            for (int x = 0; x < 3; x++)
            {
                if (boards[PLAYER_X][8 - y * 3 + x])
                    cerr << "X";
                else if (boards[PLAYER_O][8 - y * 3 + x])
                    cerr << "O";
                else
                    cerr << "-";
            }
            cerr << endl;
        }
        vector<int> moves = available_moves();
        for (int i = 0; i < moves.size(); i++)
        {
            cerr << i << " ";
        }
        cerr << endl;
        cerr << "status :" << status << endl;
    }
};

class Macroboard
{
public:
    Microboard macro;
    Microboard micro[9];


    int next_micro;
    PlayerId playerToPlay;

    vector<int> moves = {112, 111, 110, 109, 108, 107, 106, 105, 104, 99, 98, 97, 96, 95, 94, 93, 92, 91, 86, 85, 84, 83, 82, 81, 80, 79, 78, 73, 72, 71, 70, 69, 68, 67, 66, 65, 60, 59, 58, 57, 56, 55, 54, 53, 52, 47, 46, 45, 44, 43, 42, 41, 40, 39, 34, 33, 32, 31, 30, 29, 28, 27, 26, 21, 20, 19, 18, 17, 16, 15, 14, 13, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    string hash;
    Macroboard()
    {
        next_micro = -1;
        playerToPlay = PLAYER_X;
        hash = "000000000-1";
        // moves = available_moves();
    }

    Macroboard(const Macroboard &b)
    {
        // cerr << "copy "<<endl;
        macro = b.macro;
        for (int i = 0; i < 9; i++)
            micro[i] = b.micro[i];
        next_micro = b.next_micro;
        playerToPlay = b.playerToPlay;
        moves = available_moves();
        hash = b.hash;
    }

    Macroboard(const Macroboard &b, int _move)
    {
        // cerr << "copy with move"<<endl;

        macro = b.macro;
        for (int i = 0; i < 9; i++)
            micro[i] = b.micro[i];
        next_micro = b.next_micro;
        playerToPlay = b.playerToPlay;
        move(_move);
    }

    GameStatus status() const { return macro.status; }

    string hashBoard() const
    {
        string s;
        for (int i = 0; i < 9; i++){
            s += to_string(micro[i].hash());
        }
        s+=to_string(next_micro);
        return s;
    }

    void move(int hash)
    {
        int bo = unhash_board(hash), ce = unhash_cell(hash);
        Microboard &b = micro[bo];
        next_micro = ce;

        b.move(playerToPlay, next_micro);
        if ((int)b.status < 2)
            macro.move((PlayerId)b.status, bo);

        if (micro[next_micro].status != IN_PROGRESS)
            next_micro = -1;

        playerToPlay = (playerToPlay == PLAYER_X ? PLAYER_O : PLAYER_X);
        moves = available_moves();
        if (moves.size() == 0 && macro.status == IN_PROGRESS)
            macro.status = TIE;
        this->hash = hashBoard();
    }

    vector<int> available_moves() const
    {
        vector<int> moves;
        if (macro.status != IN_PROGRESS)
            return moves;

        if (next_micro != -1)
        {
            vector<int> const &m = micro[next_micro].moves;
            moves.reserve(m.size());
            for (auto i = 0; i < m.size(); i++)
                moves.push_back(hash_move(next_micro, m[i]));
        }
        else
        {
            vector<int> const &available_boards = macro.moves;
            moves.reserve(available_boards.size() * 9);
            for (auto i = 0; i < available_boards.size(); i++)
            {
                vector<int> const &m = micro[available_boards[i]].moves;
                for (auto j = 0; j < m.size(); j++)
                    moves.push_back(hash_move(available_boards[i], m[j]));
            }
        }
        return moves;
    }

    int hash_move(int boardIdx, int cellIdx) const
    {
        return boardIdx * 13 + cellIdx;
    }
    int unhash_board(ulong hash) const
    {
        return hash / 13;
    }
    int unhash_cell(ulong hash) const
    {
        return hash % 13;
    }

    void print() const
    {
        cerr <<"hash "<<hash << endl;
        for (int y = 0; y < 9; y++)
        {
            for (int x = 0; x < 9; x++)
            {
                int boardIdx = x / 3 + y / 3 * 3;
                int cellIdx = x % 3 + y % 3 * 3;

                Microboard const &b = micro[boardIdx];
                // b.print();
                if (b.status < 2)
                    cerr << (b.status == X_WON ? 'X' : 'O');
                else if (b.boards[PLAYER_X][8 - cellIdx])
                    cerr << "x";
                else if (b.boards[PLAYER_O][8 - cellIdx])
                    cerr << "o";
                else
                    cerr << "-";
            }
            cerr << endl;
        }
        cerr << "next_micro :" << next_micro << endl;
        cerr << "player to play :" << (playerToPlay == PLAYER_X ? "X" : "O") << endl;

        // vector<int> moves = available_moves();
        for (int i = 0; i < moves.size(); i++)
        {
            cerr << moves[i] << ",";
            // cerr << unhash_board(moves[i]) << "/" << unhash_cell(moves[i]) << " ";
        }
        cerr << endl;
        cerr << "status :" << status() << endl << "-----" << endl<<endl;
        
    }
};

random_device dev;
mt19937 rng(dev());
uniform_int_distribution<mt19937::result_type> dist(1, 1e6);
class Node; 
unordered_map<string, Node *> zobrist;

class Node {
    public:  

    int score = 0;
    
    int visits = 0;
    float log_visits = 0;

    Node *parent = 0;
    unordered_map<int, Node*> children;

    Macroboard b;

    int is_terminal;
    int fully_expanded;

    Node(Macroboard const &_b, int _move = -1,Node *p = 0){
        b = _b;
        if(_move != -1)
            b.move(_move);
        // _b.print();
        // b.print();
        parent = p;
        is_terminal = b.status() != IN_PROGRESS;
        fully_expanded = is_terminal;
    }

    void backpropagate(int score){
        Node *current = this;
        while(current){
            current->score += score;
            current->visits++;
            current->log_visits = log(current->visits);
            current = current->parent;
        }
    }

    Node *expand() {

        if (b.moves.size() == 0) {
            cerr <<"shouldnt be here" << endl;
            return this;
            //exit(1);
        }

        //int rnd = b.moves.size() - 1;//dist(rng) % b.moves.size();
        int m = b.moves.back();//[rnd];

        Macroboard n(b, m);
        Node *c = new Node(n,-1, this);
        zobrist.insert(make_pair(c->b.hash, c));
        //cerr << "adding to zobrist " << c->b.hash << endl;

        b.moves.pop_back();
        //b.moves.erase(b.moves.begin() + rnd); // erasing
        if (b.moves.size() == 0) {
            fully_expanded = true;
        }

        children.insert(make_pair(m, c));

        return (c);
    }

    Node *best_uct(PlayerId me, float ec = 0){
        float best_score = -1e5;
        Node *best = 0;
        int f = (b.playerToPlay == me ? 1 : -1);
        for (auto ch: children){
            Node *c = ch.second;
            float uct = ((float)c->score / c->visits + ec * sqrtf(log_visits / c->visits)) * f;
            if (uct > best_score){
                best_score = uct;
                best = c;
            }
        }
        return best;
    }

    void freeNode(){
        for (auto ch: children)
            ch.second->freeNode();
        delete this;
    }

};

class MCTS {

public:
    Node *root = 0;
    Node *rt = 0;
    PlayerId me;
    MCTS(Macroboard const &b, PlayerId _me){
        update_root(b,me);
        root = rt;
    }

    ~MCTS(){
        if (root)
        root->freeNode();
    }

    void update_root(Macroboard const &b, PlayerId _me){
        auto it = zobrist.find(b.hash);
        if (it != zobrist.end()){
            //cerr <<"WOAH " <<b.hash<<endl;
            rt = it->second;
        }
        else
            rt = new Node(b);

        //cerr << "root updated " << endl;
        //b.print();
        me = _me;
    }


    Node *select() const {
        Node *current = rt;
        while (current->fully_expanded && !current->is_terminal){
            current = current->best_uct(me,2);
        }
        if (current->is_terminal) return current;
        return current->expand();
    }

    void search()
    {
        int iters = 1000;

        while (iters--){
            Node *n = select();
            int score = rollout(n->b);
            n->backpropagate(score);
        }
        //cerr <<"done searching "<< endl;
    }


    void searchUntil(int us)
    {
        time_point<system_clock, nanoseconds> start = high_resolution_clock::now();
        int i = 0;
        while (duration_cast<microseconds>(high_resolution_clock::now() - start).count() < us){
            Node *n = select();
            int score = rollout(n->b);
            n->backpropagate(score);
            i++;
        }
        cerr << i << " iters" << endl;
    }

    int rollout(Macroboard b) const {
        while (b.status() == IN_PROGRESS)
            b.move(b.moves[dist(rng)%b.moves.size()]);
        if (b.status() == 2) return (b.macro.boards[0].count() - b.macro.boards[1].count()) * (me == PLAYER_X ? 1 : -1);
        if (b.status() == (GameStatus)me) return 1;
        return (-1);
    }

    int bestMove()const{
        //cerr << "root best child " << rt->b.hash << endl;
        //rt->b.print();
        float best_score = -1e5;
        int best = 0;
        for (auto ch: rt->children){
            Node *c = ch.second;
            float uct = (float)c->score / c->visits;
            //cerr << c->score <<'/'<< c->visits<<" ";
            if (uct > best_score){
                best_score = uct;
                best = ch.first;
            }
        }
        //cerr << endl;
        return best;
    }

};

int main()
{

    // game loop

    int turn = 0;
    PlayerId me = PLAYER_X;
    Macroboard b;
    MCTS mcts(b,me);

    while (1) {
        int op_y;
        int op_x;
        cin >> op_y >> op_x; cin.ignore();
        if (op_y != -1 && turn == 0)
            me = PLAYER_O;

        //cerr << op_x << " " <<op_y << endl;
        int x,y;
        int bx,by;
        if (op_y != -1)
        {
            x = op_x%3;
            y = op_y%3;
            bx = op_x/3;
            by = op_y/3;
            //b.print();

            //cerr << x << " " << y << "-" << bx << " " << by << "-" << by*3 + bx << " " << x+y*3 << endl;
                b.move(b.hash_move(by*3 +bx, x+y*3));
            //b.print();
        }
        mcts.update_root(b, b.playerToPlay);
        int valid_action_count;
        cin >> valid_action_count; cin.ignore();
        for (int i = 0; i < valid_action_count; i++) {
            int row;
            int col;
            cin >> row >> col; cin.ignore();
        }


        mcts.searchUntil(turn == 0 ? 999000 : 99900);
        int hash = mcts.bestMove();

        //cerr << "move hash " << hash <<endl;
        
            int board = b.unhash_board(hash);
            int move = b.unhash_cell(hash);
            //cerr << board << " " << move <<endl;
            cout << board / 3 * 3 + move / 3 <<" "<< board % 3 * 3 + move % 3 <<" TEST" <<endl;

        b.move(hash);
        //b.print();
        // cerr << "0 0" << endl;
        turn++;
    }
}