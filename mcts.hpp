# ifndef __MCTS_HPP__
# include "micro.hpp"
# include <cmath>
# include <random>
# include <map>



class Node {
    public:
    Microboard b;
    map<size_t,Node *> children;
    Node *parent;

    float visits;
    float score;
    int is_fully_expanded;

    Node(){
        visits = 0;
        score = 0;
        parent = 0;
        is_fully_expanded = 0;
    }

    Node(Microboard const & _b){
        visits = 0;
        score = 0;
        parent = 0;
        b = _b;
        is_fully_expanded = is_terminal();
    }

    // Node(Microboard const & _b, int move, Node *parent = 0){ 
    //     visits = 0;
    //     score = 0;
    //     parent = parent;
    //     b = _b;
    //     b.move(move);
    //     is_fully_expanded = is_terminal();
    // }

    Node(Node *p , int move){ // probably this one 
        visits = 0;
        score = 0;
        parent = p;
        b = p->b;
        b.move(move);
        is_fully_expanded = is_terminal();
    }
    int is_terminal() const {
        return b.status < 3;
    }
    void addScore(int s){
        //cout << "updating score " <<s <<" " <<score <<" " <<visits <<" "<<endl;
        score += s;
        visits ++;
    }
};

class MCTS {

    public:
    PlayerId me;


    int search(Microboard const &b, int iter){
        Node *node;
        Node *root = new Node(b);

        PlayerId me = b.playerToPlay;

        for (int i = 0; i < iter; i++){
            // cout << "iter "<< 1 + i << endl;
            node = select_node(root);
            int score = rollout(node->b, me);
            // cout << "score =" << score <<" propagating .." <<endl;
            backpropagate(node,score, me);
        }

        cout << "root visited count "<< root->visits << endl;
        cout << "children count "<< root->children.size() << endl;
       for (map<size_t,Node *>::iterator it = root->children.begin(); it != root->children.end(); ++it){
            Node *child = it->second;
            cout << "child "<<it->second->b.lastMove << ": "<<it->second->score << "/"<<it->second->visits << endl;
       }
        return get_best_move(root, 0, me)->b.lastMove;
    }

    Node *select_node(Node *node)
    {
        while (!node->is_terminal()){
            if (node->is_fully_expanded)
            {
                // cout << "getting best move in select" << endl;
                node = get_best_move(node,2 , me);
            }
            else
            {
                // cout << "expanding in select" << endl;
                return expand_node(node);
            }
        }
        return node;
    }

    Node *expand_node(Node *node)
    {
        vector<int> moves = node->b.available_moves();
        for (int i = 0; i < moves.size(); ++i){
            // only expand to children not found yet
            size_t h = node->b.hash_board_move(moves[i]); 

            if (node->children.find(h) == node->children.end())
            {
                Node *new_node = new Node(node, moves[i]);
                node->children.insert(make_pair(h, new_node));
                // cout << "new child " << endl;
                if (i == moves.size() - 1){
                    node->is_fully_expanded = true;
                    // cout << "fully expanded " << endl;
                }
                return new_node;
            }
            else{
                // cout<<"moving on.."<<endl;
            }
        }
        //shouldnt get here
        exit(1);
    }

    int rollout(Microboard b, int me) // return a score ? // later backpropagate inside rollout
    {
        while (b.status == 3)
        {
            vector<int> moves = b.available_moves();
            std::random_device dev;
            std::mt19937 rng(dev());
            std::uniform_int_distribution<std::mt19937::result_type> dist(0,moves.size()-1);

            b.move(moves[dist(rng)]);
        }
        int player = b.playerToPlay;
        if (me == b.status)
            return (1);
        else if ((GameStatus)((me + 1) % 2) == b.status)
            return (-1);
        return (0);
    }   

    void backpropagate(Node *node, int score, int me){
        int i = 0;

        while (node)
        {
            node->addScore(score);
            node = node->parent;
            i++;
        }
        // cout << "updated status in " << i << " nodes" << endl;
    }

    Node * get_best_move(Node *node, float exploration_constant, PlayerId me)
    {
        float best_score = -1000000;
        Node *best_move = 0;

        int player = node->b.playerToPlay == me ? 1 : -1; // do this outside ?
        for (map<size_t,Node *>::iterator it = node->children.begin(); it != node->children.end(); ++it){
            Node *child = it->second;

            float move_score = player *  // Upper Confidence bound 1 applied to trees, aka UCT
                child->score / child->visits 
            + exploration_constant 
            * sqrtf(logf(node->visits) / child->visits);
            
            if (!exploration_constant)
            cout <<"score child  " << move_score << endl;


            if (move_score >= best_score){
                best_score = move_score;
                // cout <<"found best move" << endl;
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