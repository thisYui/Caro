//
// Created by Duy on 2/26/2025.
//

#include "type.h"

std::unordered_map<Position, Player> convertToMap(const int*& arr, const int& size) {
   std::unordered_map<Position, Player> map;
    for (int i = 0; i < size; i++) {
        Position pos = encode(arr[i], arr[i + 1]);
        map[pos] = static_cast<Player>(arr[i + 2]);
        i += 2;
    }

    return map;
}
void Node::createChildren() {
    std::unordered_set<Position> possibleMoves;
    for (const auto& [pos, player] : boardState) {
        const int x = pos >> 8, y = pos & 0xFF;
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx == 0 && dy == 0) continue;
                const int nx = x + dx, ny = y + dy;
                if (nx < 0 || ny < 0 || nx >= 255 || ny >= 255) continue;

                Position newMove = nx << 8 | ny;
                if (!boardState.contains(newMove)) {  // Just in C++20
                    possibleMoves.insert(newMove);
                }
            }
        }
    }

    for (Position move : possibleMoves) {
        auto newBoard = boardState;
        newBoard[move] = currentPlayer;
        children.push_back(std::make_unique<Node>(newBoard, getOpponent(currentPlayer), move, this));
    }
}