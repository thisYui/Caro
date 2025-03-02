//
// Created by Duy on 2/28/2025.
//

#include "heuristic.h"
#include <random>

Position checkConsecutiveArea(const std::unordered_map<Position, Player>& board,
    const Player& player, const int& condition) {
    std::unordered_set<Position> posPlayer = filterPlayer(board, player);

    Position move = 0;
    for (const auto& pos : posPlayer) {
        for (int i = 0; i < COUNT_DIRECTION; ++i) {
            Position oppositePos = sum(pos, dx[i] * -1, dy[i] * -1);
            Position emtyCell = 0;
            int count = 1;

            for (int j = 1; j < condition; ++j) {
                Position newPos = sum(pos,dx[i] * j, dy[i] * j);

                if (!board.contains(newPos)) {
                    if (emtyCell == 0) {
                        emtyCell = newPos;
                        continue;
                    }
                    break;
                }

                if (board.at(newPos) == player) count++;
                else break;
            }

            if (count == condition - 1 && emtyCell != 0) {
                if (!board.contains(oppositePos)) return emtyCell;  // If the opposite cell is empty, return this cell
                move = emtyCell;
            }
        }
    }

    // Choice (0, 0) is mark, so this cell maybe has some error
    // But don't worry, cells start is (128,128) it't so far from (0,0)
    // But this still 'can' be a bug
    // You can change this to other cell in the board, eny cell you want
    // In now, you can check the cell it is on the board or not
    // If it is on the board, you know you can't win right away
    // If you can win right away, you can return the cell you can win

    return move;
}

void filterMoves(const std::unordered_map<Position, Player>& board,
    const Player& player, std::unordered_set<Position>& possibleMoves) {
    /*
     * Remove all cells if they are not satisfy the following conditions:
     */

    const int AREA = 2;  // Can change this value
    const int STANDARD_AREA = 1;  // Can not change this value
    constexpr int STANDARD_SIZE = 20; // Can change this value
    constexpr int STANDARD_AROUND = 2; // Can change this value

    for (auto& pos : possibleMoves) {
        // In the area be checked
        int countPlayer = 0;
        int countEnemy = 0;
        int countPlayerAround = 0; // In other words is this area 3x3 -> area standard

        for (int i = -AREA; i < AREA; i++) {
            for (int j = -AREA; j < AREA; j++) {
                Position newPos = sum(pos, i, j);

                if (board.contains(newPos)) {
                    if (board.at(newPos) == player) {
                        if (i*i <= STANDARD_AREA * STANDARD_AREA
                            && j*j <= STANDARD_AREA*STANDARD_AREA) {
                            countPlayerAround++;
                        }
                        countPlayer++;
                    }
                    else countEnemy++;
                }
            }
        }

        // 1. In the area 5x5 if you don't have least 2 chess of player and 2 chess of enemy, remove this cell
        // Using when the board iss large, at the beginning of the game the cell too few
        if (board.size() > STANDARD_SIZE && countPlayer < 2 || countEnemy < 2) possibleMoves.erase(pos);
        // 2. Have tended to extend, in this area 5x5 have more half of chess is player's chess
        // Get area - 1, area = (size + 1) ^ 2
        else if (countPlayer < ((2*AREA +1)*(2*AREA +1)) / 2 - 1) possibleMoves.erase(pos);
        // 3. If in around (3x3) have least 2 chess of player, keep this cell
        // Total 8 cless around the cell, 2/8 = 25% -> 25% is a lower limit
        else if (countPlayerAround <= STANDARD_AROUND) {
            possibleMoves.erase(pos);
            continue;
        }

        // 4. If the cell will create a consecutive area, keep this cell
        // Check in 5 cells (standard area) -> ( _ _ _ _ _ )
        // example: _ X (X) X _ : Have 3 cells consecutive and 2 cell is empty behind
        //          _ X (X) _ _ : Have 2 cells consecutive and 3 cell is empty behind
        for (int i = 0; i < COUNT_DIRECTION; i++) {
            // Inner egde
            Position posLeft = sum(pos, dx[i] * 1, dy[i] * 1);
            Position posRight = sum(pos, dx[i] * -1, dy[i] * -1);

            // 2 cells is emty -> delete this cell
            if (!board.contains(posLeft) && !board.contains(posRight)) possibleMoves.erase(pos);
            // 2 cells is enemy's chess -> delete this cell
            else if (board.contains(posLeft) && board.contains(posRight) &&
                board.at(posLeft) != player && board.at(posRight) != player) {
                possibleMoves.erase(pos);
                continue;
            }

            // If board have least 2 cell enemy's chess
            // Outer edge
            posLeft = sum(pos, dx[i] * 2, dy[i] * 2);
            posRight = sum(pos, dx[i] * -2, dy[i] * -2);

            // 2 cells is enemy's chess -> delete this cell
            if (board.contains(posLeft) && board.contains(posRight) &&
            board.at(posLeft) != player && board.at(posRight) != player) possibleMoves.erase(pos);
        }
    }
}

Position heuristic(const std::unordered_map<Position, Player>& board, const Player& currentPlayer,
    const Position& lastMove, std::unordered_set<Position> &possibleMoves){
    /*
    * First, we check if we can win right away
	* The priority list is as follows:
    * Case 1: If you can win right away, return the cell you can win
	* Case 2: If you can lose right away, return the cell you can lose
	* Case 3: If you can win in the next turn, return the cell you can win
	* Case 4: If you can lose in the next turn, return the cell you can lose
	* Case 5: Random a cell in the board
    */
	Position pos = 0;  // Default value
    int consecutiveArea = WIN_CONDITION;

    // 1.  X X X X _ : Have 4 cells consecutive and least one cell is empty behind
    //    X X X _ X :Just need 1 emty cell in the middle of 2 directions to win
    //    X X _ X X : Just need 1 emty cell in the middle of 2 directions to win
    // -> Consentive 5 cells have 4 X cells and 1 empty cell -> If you choose this cell, you will win
    if (pos == 0) pos = checkConsecutiveArea(board, currentPlayer, consecutiveArea);

    // 2: O O O O _ : Have 4 cells consecutive and least one cell is empty behind
    //    O O O _ O : Just need 1 emty cell in the middle of 2 directions to lose
    //    O O _ O O : Just need 1 emty cell in the middle of 2 directions to lose
    // -> Consentive 5 cells have 4 O cells and 1 empty cell -> If you don't choose this cell, you will lose
    if (pos == 0) pos = checkConsecutiveArea(board, getOpponent(currentPlayer), consecutiveArea);

    consecutiveArea--; // Reduce 1 cell to check the next case

    // 3: _ X X X _ : Have 3 cells consecutive and 2 cell is empty behind
	//    _ X X _ X : Have 2 cells consecutive and 3 cell is empty behind
	//    _ X _ X X : Have 1 cells consecutive and 4 cell is empty behind
    //  -> Consentive 4 cells have 3 O cells and 1 empty cell -> You can win in the next turn
    if (pos == 0) pos = checkConsecutiveArea(board, currentPlayer, consecutiveArea);

    // 4: _ O O O _ : If your opponent have 3 cells consecutive and 2 cell is empty behind
    //     _ O O _ O : If your opponent have 2 cells consecutive and 3 cell is empty behind
    //     _ O _ O O : If your opponent have 1 cell consecutive and 4 cell is empty behind
    // -> Consentive 4 cells have 3 O cells and 1 empty cell -> You can lose in the next turn
    if (pos == 0) pos = checkConsecutiveArea(board, getOpponent(currentPlayer), consecutiveArea);

    // 5: Other case: Random a cell in the board
    // You can extend cases, but it will reduce the randomness and make the game seem predetermined
    if (pos == 0) {
		filterMoves(board, currentPlayer, possibleMoves);

        if (possibleMoves.empty()) return 0;  // If the board is full, return 0

        // Choose a random move from the best moves
        // Using static to avoid reseeding the random number generator
        static std::random_device rd;
        static std::mt19937 g(rd());

        // Created a uniform distribution from 0 to possibleMoves.size() - 1
        std::uniform_int_distribution<size_t> dist(0, possibleMoves.size() - 1);
        auto it = possibleMoves.begin();
        std::advance(it, dist(g));

        pos = *it;  // Assign the random move to pos
    }

    return pos;
}