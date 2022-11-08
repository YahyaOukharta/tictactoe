#pragma GCC optimize("O3,inline,omit-frame-pointer,unroll-loops","unsafe-math-optimizations","no-trapping-math")
#pragma GCC option("arch=native","tune=native","no-zero-upper")
// #pragma GCC target("sse,sse2,sse3,ssse3,sse4,mmx,avx,avx2,popcnt,rdrnd,abm,bmi2,fma")
#include <iostream>
#include <bitset>
#include <random>
#include <unordered_map>
#include <chrono>
#include <cstring>
using namespace std;
using namespace std::chrono;

bitset<512> win_states("11111111111111111111111111111111111111111111111111111111111111111111111111111110111100001111000011101110111011101000000010000000111111111111101011111010111100001111101011111010101010101000000011111111111110101111000011110000101010101010101010000000100000001111111111111100101010101000000011111110111111001010101010000000111111111100110010000000100000001100110011001100100000001000000011111111111100001010101010000000111110101111000010101010100000001111111110000000100000001000000010000000100000001000000010000000");

int move_masks[9] = {0b100000000, 0b010000000, 0b001000000, 0b000100000, 0b000010000, 0b00001000, 0b000000100, 0b000000010, 0b000000001};
int moves_available[512] = {511, 510, 509, 508, 507, 506, 505, 504, 503, 502, 501, 500, 499, 498, 497, 496, 495, 494, 493, 492, 491, 490, 489, 488, 487, 486, 485, 484, 483, 482, 481, 480, 479, 478, 477, 476, 475, 474, 473, 472, 471, 470, 469, 468, 467, 466, 465, 464, 463, 462, 461, 460, 459, 458, 457, 456, 455, 454, 453, 452, 451, 450, 449, 448, 447, 446, 445, 444, 443, 442, 441, 440, 439, 438, 437, 436, 435, 434, 433, 432, 431, 430, 429, 428, 427, 426, 425, 424, 423, 422, 421, 420, 419, 418, 417, 416, 415, 414, 413, 412, 411, 410, 409, 408, 407, 406, 405, 404, 403, 402, 401, 400, 399, 398, 397, 396, 395, 394, 393, 392, 391, 390, 389, 388, 387, 386, 385, 384, 383, 382, 381, 380, 379, 378, 377, 376, 375, 374, 373, 372, 371, 370, 369, 368, 367, 366, 365, 364, 363, 362, 361, 360, 359, 358, 357, 356, 355, 354, 353, 352, 351, 350, 349, 348, 347, 346, 345, 344, 343, 342, 341, 340, 339, 338, 337, 336, 335, 334, 333, 332, 331, 330, 329, 328, 327, 326, 325, 324, 323, 322, 321, 320, 319, 318, 317, 316, 315, 314, 313, 312, 311, 310, 309, 308, 307, 306, 305, 304, 303, 302, 301, 300, 299, 298, 297, 296, 295, 294, 293, 292, 291, 290, 289, 288, 287, 286, 285, 284, 283, 282, 281, 280, 279, 278, 277, 276, 275, 274, 273, 272, 271, 270, 269, 268, 267, 266, 265, 264, 263, 262, 261, 260, 259, 258, 257, 256, 255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244, 243, 242, 241, 240, 239, 238, 237, 236, 235, 234, 233, 232, 231, 230, 229, 228, 227, 226, 225, 224, 223, 222, 221, 220, 219, 218, 217, 216, 215, 214, 213, 212, 211, 210, 209, 208, 207, 206, 205, 204, 203, 202, 201, 200, 199, 198, 197, 196, 195, 194, 193, 192, 191, 190, 189, 188, 187, 186, 185, 184, 183, 182, 181, 180, 179, 178, 177, 176, 175, 174, 173, 172, 171, 170, 169, 168, 167, 166, 165, 164, 163, 162, 161, 160, 159, 158, 157, 156, 155, 154, 153, 152, 151, 150, 149, 148, 147, 146, 145, 144, 143, 142, 141, 140, 139, 138, 137, 136, 135, 134, 133, 132, 131, 130, 129, 128, 127, 126, 125, 124, 123, 122, 121, 120, 119, 118, 117, 116, 115, 114, 113, 112, 111, 110, 109, 108, 107, 106, 105, 104, 103, 102, 101, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};

#define O 1
#define X 0



enum GameStatus
{
    X_WON,
    O_WON,
    TIE,
    IN_PROGRESS
};
#define U64 unsigned long long
#define uint __uint128_t
#define get_bit128(board, n) (board) & ((uint)1 << n)
#define get_bit(board, n) (board) & (1 << n)
#define set_bit(board, n) (board) |= (1 << n)
#define reset_bit(board, n) (board) &= ~(1 << n)
#define reset_bit128(board, n) (board) &= ~((uint)1 << n)

#define xy_to_board(x, y) y / 3 * 3 + x / 3
#define xy_to_cell(x, y) y % 3 * 3 + x % 3

#define idx_to_board(idx) idx / 9
#define idx_to_cell(idx) idx % 9

#define idx_to_x(idx) idx % 9
#define idx_to_y(idx) idx / 9

void print_uint(uint u)
{

    for (int j = 0; j < 9; j++)
    {
        for (int i = 0; i < 9; i++)
        {
            if (get_bit128(u, (j * 9 + i)))
                cerr << "1";
            else
                cerr << "0";
        }
        cerr << endl;
    }
    cerr << endl;
}
void print_int(int u)
{

    for (int j = 0; j < 3; j++)
    {
        for (int i = 0; i < 3; i++)
        {
            if (get_bit((u), (j * 3 + i)))
                cerr << "1";
            else
                cerr << "0";
        }
        cerr << endl;
    }
    cerr << endl;
}



class Micro
{
public:
    int board[2] = {0, 0};
    int status = IN_PROGRESS;

    void move(int n, int p)
    {
        if (status != IN_PROGRESS)
        {
            cerr << "move attempt on micro with status " << status << endl;
            exit(1);
        }
        if (get_bit((board[X] | board[O]), n))
        {
            cerr << "move attempt on micro, cell is taken, move :" << n << endl;
            exit(1);
        }
        set_bit(board[p], n);
        if (win_states[board[p]])
            status = p;
        else if (!moves_available[board[X] | board[O]])
            status = TIE;
    }

    void undo(int n, int p)
    {
        reset_bit(board[p], n);
        status = IN_PROGRESS;
    }

    void print()
    {
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (get_bit(board[X], (i * 3 + j)))
                    cerr << "x";
                else if (get_bit(board[O], (i * 3 + j)))
                    cerr << "o";
                else
                    cerr << ".";
            }
            cerr << endl;
        }
        cerr << endl;
        cerr << "Status : " << status << endl;
    }
};

void put_uint(__int128 x)
{
    if (x < 0)
    {
        putchar('-');
        x = -x;
    }
    if (x > 9)
        put_uint(x / 10);
    putchar(x % 10 + '0');
}

int ctz_u128(uint u)
{
    U64 hi, lo; /* (or uint64_t) */
    int b = 128;

    if ((lo = u & ~0ULL) != 0)
    {
        b = __builtin_ctzll(lo);
    }
    else if ((hi = u >> 64) != 0)
    {
        b = __builtin_ctzll(hi) + 64;
    }

    return b;
}
int popcount_u128(uint u)
{
    return __builtin_popcountll(u & ~0ULL) + __builtin_popcountll(u >> 64);
}

int pop_move(uint &moves)
{
    int m = ctz_u128(moves);
    reset_bit128(moves, m);
    return (m);
}
std::random_device rd;
mt19937 mt(rd());
uniform_int_distribution<> dist(0, 1e8);
int pop_random_move(uint &moves)
{
    int m = dist(mt) % popcount_u128(moves);
    uint mv = moves;
    /// print_uint(moves);
    // cerr <<"random : " <<m<<endl;
    for (int i = 0; i < m - 1; i++)
    {
        mv &= mv - 1;
    }
    m = ctz_u128(mv);
    reset_bit128(moves, m);
    return m;
}
int get_random_move(uint moves)
{
    int m = dist(mt) % popcount_u128(moves);
    //cerr << m << endl;
    uint mv = moves;
    /// print_uint(moves);
    // cerr <<"random : " <<m<<endl;
    for (int i = 0; i < m - 1; i++)
    {
        mv &= mv - 1;
    }
    m = ctz_u128(mv);
    return m;

}

U64 zobrist[2][81];
void init_zobrist(){
    for (int i = 0; i < 2; i++){
        for (int j = 0; j < 81; j++){
            zobrist[i][j] = dist(mt) * dist(mt);
        }
    }
}

class Macro
{

public:
    Micro micro[9];

    int boards[3] = {0, 0, 0};
    int status = IN_PROGRESS;

    int next_board = -1;
    int turn = 0;

    int hash = 0;

    Macro()
    {
        next_board = -1;
    }
    Macro(Macro const &m)
    {
        memcpy(micro, m.micro, sizeof(Micro) * 9);
        memcpy(boards, m.boards, sizeof(int) * 3);

        status = m.status;
        next_board = m.next_board;
        turn = m.turn;
        hash = m.hash;
    }

    int turn_of()
    {
        return turn % 2 ? O : X;
    }

    void move(int board, int move, int p)
    {
        if (status != IN_PROGRESS)
        {
            cerr << "move : " << board << " " << move << endl;
            print_int(boards[X] | boards[O] | boards[TIE]);
            print();
            print_uint(moves());
            cerr << "move attempt on macro with status " << status << endl;
            exit(1);
        }
        if (micro[board].status != IN_PROGRESS)
        {
            print_int(boards[X] | boards[O] | boards[TIE]);
            print();
            print_uint(moves());
            cerr << "move : " << board << " " << move << endl;
            cerr << "move attempt on macro #" << board << " with status " << micro[board].status << endl;
            exit(1);
        }
        if (next_board != -1 && board != next_board)
        {
            print_int(boards[X] | boards[O] | boards[TIE]);
            print();
            print_uint(moves());
            cerr <<"board "<<board <<endl;
            cerr << "move attempt on macro, invalid board, next_board " << next_board << " , move " << move << endl;
            exit(1);
        }

        micro[board].move(move, p);

        if (micro[board].status != IN_PROGRESS)
        {
            set_bit(boards[micro[board].status], board);

            if (win_states[boards[p]])
                status = p;
            else if (!moves_available[boards[X] | boards[O] | boards[TIE]])
                status = TIE;
        }
        next_board = (get_bit((boards[X] | boards[O] | boards[TIE]), move)) ? -1 : move;
        hash ^= zobrist[p][board*9+move];

        turn++;
    }

    void move(int idx, int p)
    {
        move(idx_to_board(idx), idx_to_cell(idx), p);
    }

    uint moves()
    {
        uint res = 0;
        if (status != IN_PROGRESS)
            return res;
        if (next_board != -1)
        {
            return (uint)(moves_available[micro[next_board].board[X] | micro[next_board].board[O]]) << (next_board * 9);
        }
        for (int i = 8; i >= 0; i--)
        {

            if (!(get_bit((boards[X] | boards[O] | boards[TIE]), i)))
                res |= moves_available[micro[i].board[X] | micro[i].board[O]];
            if (i)
                res <<= 9;
        }
        return res;
    }

    void print()
    {
        cerr << "hash : " << hash << endl;
        for (int j = 0; j < 9; j++)
        {
            for (int i = 0; i < 9; i++)
            {
                // 5 4 = 5
                int board = j / 3 * 3 + i / 3;
                int cell = j % 3 * 3 + i % 3;

                if (get_bit(boards[X], board))
                    cerr << "X";
                else if (get_bit(boards[O], board))
                    cerr << "O";
                else if (get_bit(boards[TIE], board))
                    cerr << "T";
                else if (get_bit(micro[board].board[X], cell))
                    cerr << "x";
                else if (get_bit(micro[board].board[O], cell))
                    cerr << "o";
                else
                    cerr << ".";
            }
            cerr << endl;
        }
        cerr << "status : " << status << endl;
        cerr << "next_board : " << next_board << endl;

        cerr << endl;
    }
};






int main()
{
    init_zobrist();

    Macro b;

    while(b.status == 3)
    { 
        b.print();
        uint moves = b.moves();
        print_uint(moves);

        int move = pop_random_move(moves);

        cerr << move <<endl;

        print_uint(moves);

        b.move(move, b.turn_of());

    }
    b.print();
}
