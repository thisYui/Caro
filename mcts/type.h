//
// Created by Duy on 2/22/2025.
//

#ifndef TYPE_H
#define TYPE_H

#include <unordered_map>
#include <vector>
#include <cstdint>
#include <memory>
#include <unordered_set>

using Position = uint16_t;
using Byte = uint8_t;

inline Position encode(const int x, const int y) {
    return (x << 8) | y;
}

// Identify player
enum Player { X, O, None };

inline Player getOpponent(const Player player) {
    return (player == Player::X) ? Player::O : Player::X;
}

struct Box {
    int upLeft, upRight, downLeft, downRight;
};

struct Node {
    std::unordered_map<Position, Player> boardState;  // Bàn cờ tại node này
    Player currentPlayer;  // Người chơi sắp đi
    Position lastMove;  // Vị trí của nước đi tạo ra node này
    int wins = 0;
    int visits = 0;
    std::vector<std::unique_ptr<Node>> children;  // Các nước đi tiếp theo
    Node* parent = nullptr;  // Node cha

    Node(std::unordered_map<Position, Player> state, Player turn, Position move, Node* p = nullptr)
        : boardState(std::move(state)), currentPlayer(turn), lastMove(move), parent(p) {}

    void createChildren();
};

std::unordered_map<Position, Player> convertToMap(const int*& arr, const int& size);

#endif //TYPE_H
