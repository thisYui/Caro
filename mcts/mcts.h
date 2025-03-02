//
// Created by Duy on 2/19/2025.
//


#ifndef MCTS_H
#define MCTS_H

#ifdef _WIN32
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

#include "utils.h"

extern "C" {
    DLL_EXPORT Position decision(const int* arr, const int size, const Position lastMove);
}

// Choose the best move for the current player
Node* select(const Node* const& root);

//Expand the tree
void expand(Node*& node);

// Simulate the game
Player simulate(const Node* const& node);

// Backpropagate the result
void backpropagate(Node*& node, Player winner);

// MCTS algorithm
Node* mcts(Node* root, const int& iterations = 1000);


#endif //MCTS_H
