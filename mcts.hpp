#ifndef __MCTS_HPP__
#include "micro.hpp"
# include <map>
class Node {
    public:
    Microboard b;
    map<size_t,Node *> children;
    Node *parent;

    int visits;
    int score;
    int is_fully_expanded;

    Node(){
        visits = 0;
        score = 0;
        parent = 0;
        is_fully_expanded = 0;
    }
    Node(Microboard const & _b, Node *parent = 0){
        visits = 0;
        score = 0;
        parent = parent;
        b = _b;
        is_fully_expanded = is_terminal();
    }
    Node(Microboard const & _b, int move, Node *parent = 0){ 
        visits = 0;
        score = 0;
        parent = parent;
        b = _b;
        b.move(move);
        is_fully_expanded = is_terminal();
    }
    Node(Node *parent , int move){ // probably this one 
        visits = 0;
        score = 0;
        parent = parent;
        b = parent->b;
        b.move(move);
        is_fully_expanded = is_terminal();
    }
    int is_terminal() const {
        return b.status < 3;
    }
    void addScore(int s){
        score += s;
    }
};

class MCTS {

    public:
    Node root;
    PlayerId me;
    Node *select_node(Node *node){

        while (!node->is_terminal()){
            if (node->is_fully_expanded)
                node = get_best_move(node, 2 , me);
            else
                return expand_node(node);
        }
        return node;
    }

    Node *expand_node(Node *node){
        vector<int> moves = node->b.available_moves();
        for (int i = 0; i < moves.size(); ++i){
            // only expand to children not found yet
            size_t h = node->b.hash_board_move(moves[i]); 
            if (node->children.find(h) == node->children.end())
            {
                Node *new_node = new Node(node, moves[i]);
                node->children.insert(make_pair(h, new_node));
                
                if (i == moves.size() - 1){
                    node->is_fully_expanded = true;
                }
                return new_node;
            }
        }
        //shouldnt get here
        return 0;
    }

    Node * get_best_move(Node *node, int exploration_constant, PlayerId me){
        float best_score = -1000000;
        Node *best_move;

        for (map<size_t,Node *>::iterator it = node->children.begin(); it != node->children.end(); ++it){
            Node *child = it->second;
            int player = child->b.playerToPlay == me ? 1 : -1; // do this outside ?

            float move_score = player *  // Upper Confidence bound 1 applied to trees, aka UCT
                child->score / child->visits 
            + exploration_constant * sqrtf(logf(child->visits / child->visits));

            if (move_score > best_score){
                best_score = move_score;
                // best_move = child->b.lastMove;
                best_move = child;
            }
            else if (move_score == best_score){
                // append  to return random move
            }
        }
        return best_move;
    }
};

#endif