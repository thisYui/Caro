//
// Created by Duy on 2/28/2025.
//

#ifndef HEURISTIC_H
#define HEURISTIC_H

#include "utils.h"

Position checkConsecutiveArea(const std::unordered_map<Position, Player>& board,
    const Player& player, const int& condition);

void filterMoves(const std::unordered_map<Position, Player>& board,
    const Player& player, std::unordered_set<Position>& possibleMoves);

Position heuristic(const std::unordered_map<Position, Player>& board, const Player& currentPlayer,
    const Position& lastMove, std::unordered_set<Position> &possibleMoves);

#endif //HEURISTIC_H
