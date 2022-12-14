#ifndef __MINIMAX_HPP__
#define __MINIMAX_HPP__
#include "micro.hpp"

int minimax(Microboard const & board, int max_depth, int isMaximizing, int me){


    if (max_depth == 0 || board.status < 3)
    {
        if (board.status < 2)
            return board.status == me ? 10 : -10;
        else return (0);
    }
    
    if (isMaximizing)
    {
        vector<int> moves = board.available_moves();
        int maxScore = -50;
        for (int i = 0; i < moves.size(); ++i){
            maxScore = max(maxScore, minimax(Microboard(board,moves[i]), max_depth - 1, !isMaximizing, me));
        }
        // return Best Move here
        return maxScore;
    }
    else
    {
        vector<int> moves = board.available_moves();
        int minScore = 50;
        for (int i = 0; i < moves.size(); ++i){
            minScore = min(minScore, minimax(Microboard(board,moves[i]), max_depth - 1, !isMaximizing, me));
        }
        return minScore;
    }
}

int alphaBeta(Microboard const & board, int max_depth, int isMaximizing, int me, int alpha, int beta){


    if (max_depth == 0 || board.status < 3)
    {
        if (board.status < 2)
            return board.status == me ? 10 : -10;
        else return (0);
    }
    
    if (isMaximizing)
    {
        vector<int> moves = board.available_moves();
        int maxScore = -50;
        for (int i = 0; i < moves.size(); ++i){
            maxScore = max(maxScore, alphaBeta(Microboard(board,moves[i]), max_depth - 1, !isMaximizing, me, alpha, beta));
            alpha = max(alpha, maxScore);
            if (beta <= alpha) break;
        }
        // return Best Move here
        return maxScore;
    }
    else
    {
        vector<int> moves = board.available_moves();
        int minScore = 50;
        for (int i = 0; i < moves.size(); ++i){
            minScore = min(minScore, alphaBeta(Microboard(board,moves[i]), max_depth - 1, !isMaximizing, me, alpha, beta));
            beta = min(beta, minScore);
            if (beta <= alpha) break;
        }
        return minScore;
    }
}

int getBestMoveAB(Microboard const & board){
    vector<int> moves = board.available_moves();
    int bestScore = -50;
    int bestMove = -1;

    for (int i = 0; i < moves.size(); ++i){
        int v = alphaBeta(Microboard(board,moves[i]),9, false, board.playerToPlay, -100, 100);
        if (v > bestScore){
            bestScore = v;
            bestMove = moves[i];
        }
    }
    return bestMove;
}


int getBestMove(Microboard const & board){
    vector<int> moves = board.available_moves();
    int bestScore = -50;
    int bestMove = -1;

    for (int i = 0; i < moves.size(); ++i){
        int v = minimax(Microboard(board,moves[i]),9, false, board.playerToPlay);
        if (v > bestScore){
            bestScore = v;
            bestMove = moves[i];
        }
    }
    return bestMove;
}

#endif