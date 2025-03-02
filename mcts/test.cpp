//
// This is the test file for the project.
//

#include "mcts.h"
#include <iostream>
// You can use <gtest/gtest.h>

static void getResult(int &x, int & y, const std::unordered_map<Position, Player>& m) {
    Player currentPlayer = Player::O;
    Position lastMove = (8 << 8) | 8 ;
    Node* root = new Node(m, currentPlayer, lastMove);

    // Run MCTS algorithm
    Node* bestMoveNode = mcts(root);

    x = bestMoveNode->lastMove >> 8;
    y = bestMoveNode->lastMove & 0xFF;

    delete root;  // Free memory
}

void test_1() {
    const std::unordered_map<Position, Player> m = {
        {(8 << 8) | 8, Player::X},
        {(7 << 8) | 8, Player::X},
        {(6 << 8) | 8, Player::X},
        {(4 << 8) | 8, Player::X},
        {(7 << 8) | 9, Player::O},
        {(6 << 8) | 9, Player::O},
        {(5 << 8) | 9, Player::O}
    };

    int x, y;
    getResult(x, y, m);

    if (x == 5 && y == 8) {
        std::cout << "Test 1 passed\n";
    }
    else {
        std::cout << "Test 1 failed\n";
        std::cout << "The result is: (" << x << " / " << y << ") not (5 / 8)\n";
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

    int x, y;
    getResult(x, y, m);

    if (x == 3 && y == 9) {
        std::cout << "Test 2 passed\n";
    }
    else if (x == 8 && y == 9) {
        std::cout << "Test 2 passed\n";
    }
    else {
        std::cout << "Test 2 failed\n";
        std::cout << "The result is: (" << x << " / " << y << ") not (3 / 9) or (8 / 9)\n";
    }
}

void test_3() {
    const std::unordered_map<Position, Player> m = {
        {(8 << 8) | 8, Player::X},
        {(7 << 8) | 8, Player::X},
        {(6 << 8) | 8, Player::X},
        {(5 << 8) | 9, Player::O},
        {(4 << 8) | 9, Player::O},
    };

    int x, y;
    getResult(x, y, m);

    if (x == 9 && y == 8) {
        std::cout << "Test 3 passed\n";
    }
    else if (x == 5 && y == 8) {
        std::cout << "Test 3 passed\n";
    }
    else {
        std::cout << "Test 3 failed\n";
        std::cout << "The result is: (" << x << " / " << y << ") not (9 / 8) or (5 / 8)\n";
    }
}

void test_4() {
    const std::unordered_map<Position, Player> m = {
        {(8 << 8) | 8, Player::X},
        {(7 << 8) | 8, Player::X},
        {(6 << 8) | 8, Player::X},
        {(5 << 8) | 8, Player::X},
        {(4 << 8) | 8, Player::X},
        {(7 << 8) | 9, Player::O},
        {(6 << 8) | 9, Player::O},
        {(5 << 8) | 9, Player::O},
        {(3 << 8) | 9, Player::O},
    };

    int x, y;
    getResult(x, y, m);

    if (x == 4 && y == 9) {
        std::cout << "Test 4 passed\n";
    }
    else if (x == 8 && y == 9) {
        std::cout << "Test 4 passed\n";
    }
    else {
        std::cout << "Test 4 failed\n";
        std::cout << "The result is: (" << x << " / " << y << ") not (4 / 9) or (8 / 9)\n";
    }
}

void test_5() {
    const std::unordered_map<Position, Player> m = {
        {(8 << 8) | 8, Player::X},
        {(7 << 8) | 8, Player::X},
        {(6 << 8) | 8, Player::X},
        {(6 << 8) | 9, Player::O},
        {(5 << 8) | 9, Player::O},
        {(4 << 8) | 9, Player::O},
    };

    int x, y;
    getResult(x, y, m);

    if (x == 7 && y == 9) {
        std::cout << "Test 5 passed\n";
    }
    else if (x == 3 && y == 9) {
        std::cout << "Test 5 passed\n";
    }
    else {
        std::cout << "Test 5 failed\n";
        std::cout << "The result is: (" << x << " / " << y << ") not (7 / 9) or (3 / 9)\n";
    }
}

void test_6() {
    const std::unordered_map<Position, Player> m = {
        {(8 << 8) | 8, Player::X},
        {(7 << 8) | 8, Player::X},
        {(6 << 8) | 8, Player::X},
        {(5 << 8) | 8, Player::X},
        {(7 << 8) | 9, Player::O},
        {(6 << 8) | 9, Player::O},
        {(5 << 8) | 9, Player::O}
    };

    int x, y;
    getResult(x, y, m);

    if (x == 4 && y == 8) {
        std::cout << "Test 6 passed\n";
    }
    else if (x == 9 && y == 8) {
        std::cout << "Test 6 passed\n";
    }
    else {
        std::cout << "Test 6 failed\n";
        std::cout << "The result is: (" << x << " / " << y << ") not (4 / 8) or (9 / 8)\n";
    }
}

int main() {
    test_1();
    test_2();
    test_3();
    test_4();
    test_5();
    test_6();
    return 0;
}