#ifndef MCTS_HPP
# define MCTS_HPP
# include "Board.hpp"
# include <cmath>
# include <unordered_map>
# include <random>
# include <chrono>
using namespace std;
using namespace std::chrono;
random_device dev;
mt19937 rng(dev());
uniform_int_distribution<mt19937::result_type> dist(1, 1e6);

class Node; 
unordered_map<Zobrist, Node *, hash_fn> zobrist;

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

        int rnd = dist(rng) % b.moves.size();
        int m = b.moves[rnd];

        Node *c = new Node(b,m, this);
        //zobrist.insert(make_pair(c->b.hash(), c));

        b.moves.erase(b.moves.begin() + rnd); // erasing
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
        // auto it = zobrist.find(b.hash());
        // if (it != zobrist.end()){
        //     cout <<"WOAH" <<endl;
        //     rt = it->second;
        // }
        // else
            rt = new Node(b);
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
        //cout <<"done searching "<< endl;
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
        cout << i << " iters" << endl;
    }

    int rollout(Macroboard b) const {
        while (b.status() == IN_PROGRESS)
            b.move(b.moves[dist(rng)%b.moves.size()]);
        if (b.status() == 2) return (b.macro.boards[0].count() - b.macro.boards[1].count()) * (me == PLAYER_X ? 1 : -1);
        if (b.status() == (GameStatus)me) return 10;
        return (-10);
    }

    int bestMove()const{
        float best_score = -1e5;
        int best = 0;
        for (auto ch: root->children){
            Node *c = ch.second;
            float uct = (float)c->score / c->visits;
            //cout << c->score <<'/'<< c->visits<<" ";
            if (uct > best_score){
                best_score = uct;
                best = ch.first;
            }
        }
        //cout << endl;
        return best;
    }

};

#endif