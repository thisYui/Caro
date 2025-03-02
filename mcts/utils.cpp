//
// Created by Duy on 3/2/2025.
//

#include "utils.h"

std::unordered_map<Position, Player> convertToMap(const int*& arr, const int& size) {
    std::unordered_map<Position, Player> map;
    for (int i = 0; i < size; i++) {
        Position pos = encode(arr[i], arr[i + 1]);
        map[pos] = static_cast<Player>(arr[i + 2]);
        i += 2;
    }

    return map;
}

std::unordered_set<Position> filterPlayer(const std::unordered_map<Position, Player>& board, const Player& player) {
    std::unordered_set<Position> result;
    for (const auto& [pos, p] : board) {
        if (p == player) {
            result.insert(pos);
        }
    }

    return result;
}

void extendMoves(const std::unordered_map<Position, Player>& board,
                                             const int& areaExtend, std::unordered_set<Position>& possibleMoves) {
    for (const auto& [pos, _] : board) {
        int px = pos >> 8, py = pos & 0xFF;

        for (int x = -areaExtend; x <= areaExtend; x++) {
            for (int y = -areaExtend; y <= areaExtend; y++) {
                const int nx = px + x, ny = py + y;
                if (nx < BORDER_BOTTOM || ny < BORDER_BOTTOM
                    || nx >= BORDER_TOP || ny >= BORDER_TOP) continue; // Check bounds

                Position move = encode(nx, ny);
                if (!board.contains(move)) possibleMoves.insert(move);
            }
        }
    }
}


bool checkDirection(const std::unordered_map<Position, Player>& board,
    const Player& player, const Position& pos, const int& condition) {

    for (int i = 0; i < COUNT_DIRECTION; i++) {
        int count = 1;

        for (int j = 1; j < condition; j++) {
            Position newPos = sum(pos, dx[i] * j, dy[i] * j);

            if (!board.contains(newPos)) break;

            if (board.at(newPos) == player) count++;
            else break;
        }

        if (count == condition) return true;
    }

    return false;
}

Position consecutive(const std::unordered_map<Position, Player>& board,
    const Player& player, const int& condition) {
    std::unordered_set<Position> posPlayer = filterPlayer(board, player);

    for (const auto& pos : posPlayer) {
        if (checkDirection(board, player, pos, condition)) return pos;
    }

    return 0;
}