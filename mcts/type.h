//
// Created by Duy on 2/22/2025.
//

#ifndef TYPE_H
#define TYPE_H

#include <unordered_map>
#include <vector>
#include <cstdint>
#include <memory>

constexpr int WIN_CONDITION = 5;
constexpr int BORDER_TOP = 255;
constexpr int BORDER_BOTTOM = 0;

constexpr int dx[] = {1, 0, 1, 1, -1, 0, -1, -1,}, dy[] = {0, 1, 1, -1, 0, -1, -1, 1};
constexpr int COUNT_DIRECTION = 8;

using Position = uint16_t;
using Byte = uint8_t;

// Identify player
enum Player { X, O, None };

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
};

#endif //TYPE_H
