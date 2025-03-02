//
// Created by Duy on 3/2/2025.
//

#ifndef UTILS_H
#define UTILS_H

#include "type.h"
#include <unordered_set>

inline Position encode(const int x, const int y) {
    return (x << 8) | y;
}

inline Player getOpponent(const Player player) {
    return (player == Player::X) ? Player::O : Player::X;
}
inline void decode(Position position, int& x, int& y) {
    x = position >> 8;
    y = position & 0xFF;
}

inline Position sum(const Position& p, const int& x, const int& y) {
    int x1, y1;
    decode(p, x1, y1);
    return encode(x1 + x, y1 + y);
}

std::unordered_map<Position, Player> convertToMap(const int*& arr, const int& size);

std::unordered_set<Position> filterPlayer(const std::unordered_map<Position, Player>& board, const Player& player);

void extendMoves(const std::unordered_map<Position, Player>& board,
    const int& areaExtend, std::unordered_set<Position>& possibleMoves);

bool checkDirection(const std::unordered_map<Position, Player>& board,
    const Player& player, const Position& pos, const int& condition = WIN_CONDITION);

Position consecutive(const std::unordered_map<Position, Player>& board,
    const Player& player, const int& condition = WIN_CONDITION);

#endif //UTILS_H
