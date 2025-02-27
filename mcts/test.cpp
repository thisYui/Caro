//
// This is the test file for the project.
//

#include "mcts.h"
#include <iostream>
// You can use <gtest/gtest.h>

void test_1() {
    const std::unordered_map<Position, Player> m = {
        {(8 << 8) | 8, Player::X},
        {(7 << 8) | 8, Player::X},
        {(6 << 8) | 8, Player::X},
        {(5 << 8) | 8, Player::X},
        {(7 << 8) | 9, Player::O},
        {(6 << 8) | 9, Player::O},
        {(5 << 8) | 9, Player::O}
    };

    Player currentPlayer = Player::O;
    Position lastMove = (8 << 8) | 8 ;
    Node* root = new Node(m, currentPlayer, lastMove);
    root->createChildren();

    // Run MCTS algorithm
    Node* bestMoveNode = mcts(root);

    const int x = bestMoveNode->lastMove >> 8,
            y = bestMoveNode->lastMove & 0xFF;

    delete root;  // Free memory

    if (x == 9 && y == 8) {
        std::cout << "Test 1 passed\n";
    }
    else if (x == 4 && y == 8) {
        std::cout << "Test 1 passed\n";
    }
    else {
        std::cout << "Test 1 failed\n";
    }
}

void test_2() {
    const std::unordered_map<Position, Player> m = {
        {(8 << 8) | 10, Player::X},
        {(8 << 8) | 8, Player::X},
        {(7 << 8) | 8, Player::X},
        {(6 << 8) | 8, Player::X},
        {(5 << 8) | 8, Player::X},
        {(7 << 8) | 9, Player::O},
        {(6 << 8) | 9, Player::O},
        {(5 << 8) | 9, Player::O},
        {(4 << 8) | 9, Player::O},
    };

    Player currentPlayer = Player::O;
    Position lastMove = (8 << 8) | 8 ;
    Node* root = new Node(m, currentPlayer, lastMove);
    root->createChildren();

    // Run MCTS algorithm
    Node* bestMoveNode = mcts(root);

    const int x = bestMoveNode->lastMove >> 8,
            y = bestMoveNode->lastMove & 0xFF;

    delete root;  // Free memory

    if (x == 3 && y == 9) {
        std::cout << "Test 2 passed\n";
    }
    else if (x == 8 && y == 9) {
        std::cout << "Test 2 passed\n";
    }
    else {
        std::cout << "Test 2 failed\n";
    }
}

int main() {
    test_1();
    test_2();
    return 0;
}