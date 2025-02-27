//
// Created by Duy on 2/22/2025.
//

#include "mcts.h"

extern "C" {
     DLL_EXPORT Position decision(const int* arr, const int size, const Position lastMove) {
          const std::unordered_map<Position, Player> map = convertToMap(arr, size);;
          const Player currentPlayer = Player::O; // Default the program is O

          // Create the root of the MCTS tree
          Node* root = new Node(map, currentPlayer, lastMove);
          root->createChildren();

          // Run MCTS algorithm
          Node* bestMoveNode = mcts(root);
          const int x = bestMoveNode->lastMove >> 8;
          const int y = bestMoveNode->lastMove & 0xFF;

          // Free memory
          delete root;

          return encode(x, y);
     }
}