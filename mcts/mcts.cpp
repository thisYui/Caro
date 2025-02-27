#include "mcts.h"
#include <cmath>
#include <limits>
#include <random>
#include <algorithm>


static void decode(Position position, int& x, int& y) {
    x = position >> 8;
    y = position & 0xFF;
}

// Choose the best move for the current player
static double computeUCT(const int wins, const int visits,
    const int totalSimulations, const double exploration = std::sqrt(2)) {
    /*
     * win: number of wins when choosing the node
     * visits: number of visits of the node
     * total: total number of simulations
     * exploration: exploration parameter (default = sqrt(2))
     */

    // If node doesn't have any visit, return infinity (UTC = infinity)
    if (visits == 0) return std::numeric_limits<double>::infinity();

    // Calculated by formula: wins / visits + exploration * sqrt(log(totalSimulations) / visits)
    return static_cast<double>(wins) / visits +
        exploration * std::sqrt(std::log(std::max(1, totalSimulations)) / visits);
}

Node* select(const Node* const& root) {
    if (!root || root->children.empty()) return nullptr;  // Check node nullptr

    double bestUCT = -std::numeric_limits<double>::infinity();  // Get negative infinity is the bestUTC
    Node* bestMove = nullptr;  // Best move
    int minDistance = std::numeric_limits<int>::max();  // Save the minimum distance

    const int totalSimulations = std::max(1, root->visits);  // Avoid log(0)

    for (const auto& child : root->children) {
        if (!child) continue;

        double uctValue = computeUCT(child->wins, child->visits, totalSimulations);

        const int childX = child->lastMove >> 8,
                childY = child->lastMove & 0xFF,
                rootX = root->lastMove >> 8,
                rootY = root->lastMove & 0xFF;

        const int distance = std::abs(childX - rootX) + std::abs(childY - rootY);

        if (uctValue > bestUCT || (uctValue == bestUCT && distance < minDistance)) {
            bestUCT = uctValue;
            bestMove = child.get();
            minDistance = distance;  // Update the minimum distance
        }
    }

    // Returns a reference to the children
    return bestMove;
}

void expand(Node*& node, const int MAX_X, const int MAX_Y){
    if (!node || !node->children.empty()) return;

    std::unordered_set<Position> possibleMoves;

    // Identify cells around the current board
    for (const auto& [pos, player] : node->boardState) {
       const int x = pos >> 8, y = pos & 0xFF; // Decode Position

        // Check 8 cells around the current cell
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx == 0 && dy == 0) continue; // Skip the current cell
                const int nx = x + dx, ny = y + dy;
                if (nx < 0 || ny < 0 || nx >= MAX_X || ny >= MAX_Y) continue; // Check out of range

                Position newMove = encode(nx, ny);
                // Check if the cell is not in the boardState, add it to the possibleMoves
                if (!node->boardState.contains(newMove)) { // Just in C++20
                    // If using C++ under 20, use:
                    // if (node->boardState.find(newMove) == node->boardState.end())
                    possibleMoves.insert(newMove);
                }
            }
        }
    }

    // Created children node from possibleMoves
    for (Position move : possibleMoves) {
        auto newBoard = node->boardState;
        newBoard[move] = node->currentPlayer; // Mark the cell with the current player

        // Create a new child node
        auto child = std::make_unique<Node>(newBoard, getOpponent(node->currentPlayer), move);
        child->parent = node; // Gán cha của node con

        // Add node to the children list
        node->children.push_back(std::move(child));
    }
}

static bool isGameOver(const std::unordered_map<Position, Player>& board,
    const Position& lastMove, const int& WIN_CONDITION = 5) {
    if (lastMove == 0) return false; // Doesn't have any move, game is not over

    const int x = lastMove >> 8, y = lastMove & 0xFF; // Decode Position
    const Player current = board.at(lastMove); // Get the current player at the last move

    // Directions to check, 8 directions
    constexpr int dx[] = {1, 0, 1, 1}, dy[] = {0, 1, 1, -1};

    for (int d = 0; d < 4; d++) {
        int count = 1; // Count the number of consecutive pieces

        // Traverse in 2 directions
        for (int dir = -1; dir <= 1; dir += 2) {
            int nx = x, ny = y;
            while (true) {
                nx += dx[d] * dir;
                ny += dy[d] * dir;
                Position nextPos = encode(nx, ny);

                if (board.contains(nextPos) && board.at(nextPos) == current) {  // Just in C++20 or higher
                    count++;
                }
                else {
                    break;
                }
            }
        }

        // If the number of consecutive pieces >= WIN_CONDITION, the current player wins
        if (count >= WIN_CONDITION) {
            return true;
        }
    }

    // Check if the board is full
    if (board.size() >= 255 * 255) {
        return false;
    }

    return false; // Game is not over
}

static Player getWinner(const std::unordered_map<Position, Player>& board, const Position& lastMove) {
    return board.at(lastMove);  // Last player is the winner
}

Player simulate(const Node* const& node) {
    if (!node) return Player::None;

    std::unordered_map<Position, Player> simBoard = node->boardState;
    Player simPlayer = node->currentPlayer;
    Position lastPos = 0;

    std::unordered_set<Position> possibleMoves;

    // Check the cells near the available cell
    for (const auto& [pos, _] : simBoard) {
        int x = pos >> 8, y = pos & 0xFF;

        for (int dx = -2; dx <= 2; dx++) {
            for (int dy = -2; dy <= 2; dy++) {
                const int nx = x + dx, ny = y + dy;
                if (nx < 0 || ny < 0 || nx >= 255 || ny >= 255) continue; // Check bounds

                Position move = encode(nx, ny);
                if (!simBoard.contains(move)) {
                    possibleMoves.insert(move);
                }
            }
        }
    }

    while (!possibleMoves.empty()) {
        // Choose a random move from the best moves
        // Using static to avoid reseeding the random number generator
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::vector<Position> moves(possibleMoves.begin(), possibleMoves.end());

        // Created a uniform distribution from 0 to moves.size() - 1
        std::uniform_int_distribution<size_t> dist(0, moves.size() - 1);
        Position randomMove = moves[dist(gen)];

        simBoard[randomMove] = simPlayer;
        lastPos = randomMove;

        // Update the possible can be moves
        possibleMoves.erase(randomMove);
        const int x = randomMove >> 8, y = randomMove & 0xFF;
        for (int dx = -2; dx <= 2; dx++) {
            for (int dy = -2; dy <= 2; dy++) {
                const int nx = x + dx, ny = y + dy;
                if (nx < 0 || ny < 0 || nx >= 255 || ny >= 255) continue;  // Check bounds

                Position newMove = encode(nx, ny);
                if (!simBoard.contains(newMove)) {
                    possibleMoves.insert(newMove);
                }
            }
        }

        if (isGameOver(simBoard, lastPos)) {
            return getWinner(simBoard, lastPos);
        }

        simPlayer = getOpponent(simPlayer);
    }

    return Player::None;
}

void backpropagate(Node*& node, Player winner) {
    while (node != nullptr) {
        node->visits++;  // Tăng số lần thăm trạng thái này
        if (node->currentPlayer == winner) {
            node->wins++;  // Nếu người chơi hiện tại thắng, tăng số lần thắng
        }
        node = node->parent;  // Đi ngược lên cha của nó
    }
}

Node* mcts(Node* root, const int& iterations) {
    for (int i = 0; i < iterations; i++) {
        // Chọn nút tốt nhất để mở rộng
        Node* selectedNode = select(root);

        // Mở rộng cây
        expand(selectedNode);

        // Mô phỏng trò chơi
        Node* simulationNode = selectedNode;
        if (!selectedNode->children.empty() && selectedNode->children.front()) {
            simulationNode = selectedNode->children.front().get();
        }

        Player winner = simulate(simulationNode);
        backpropagate(simulationNode, winner);
    }
    // Choose the best move
    const auto bestChild = std::max_element(
    root->children.begin(), root->children.end(),
    [](const std::unique_ptr<Node>& a, const std::unique_ptr<Node>& b) {
        return a->visits < b->visits;
        }
    );

    return (bestChild != root->children.end()) ? bestChild->get() : nullptr;
}