#include "mcts.h"
#include "heuristic.h"
#include <cmath>
#include <limits>
#include <random>
#include <algorithm>

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

void expand(Node*& node){
    std::unordered_set<Position> possibleMoves;
    extendMoves(node->boardState, 1, possibleMoves);  // Extend the area to 3x3, 1 = (3 - 1) / 2

    // Created children node from possibleMoves
    for (Position move : possibleMoves) {
        auto newBoard = node->boardState;
        newBoard[move] = node->currentPlayer; // Mark the cell with the current player

        // Create a new child node
        auto child = std::make_unique<Node>(newBoard, getOpponent(node->currentPlayer), move);
        child->parent = node; // Assign the parent node

        // Add node to the children list
        node->children.push_back(std::move(child));
    }
}

static bool isGameOver(const std::unordered_map<Position, Player>& board,
    const Position& lastMove) {
    if (consecutive(board, board.at(lastMove))) return true;
    return false;
}

static Player getWinner(const std::unordered_map<Position, Player>& board, const Position& lastMove) {
    return board.at(lastMove);  // Last player is the winner
}

Player simulate(const Node* const& node) {
    if (!node) return Player::None;

    std::unordered_map<Position, Player> simBoard = node->boardState;
    Player simPlayer = node->currentPlayer;
    Position lastPos = node->lastMove;

    std::unordered_set<Position> possibleMoves;
    extendMoves(simBoard, 4, possibleMoves);  // Extend the area to 9x9, 4 = (9 -1) / 2

    while (!possibleMoves.empty()) {
        // Choose the best move from heuristic
        Position move = heuristic(simBoard, simPlayer, lastPos, possibleMoves);
        if (move == 0) break;  // If the move is invalid, break the loop

        simBoard[move] = simPlayer;
        lastPos = move;

        // Update the possible can be moves
        possibleMoves.erase(move);
        extendMoves(simBoard, 2, possibleMoves);

        if (isGameOver(simBoard, lastPos)) {
            return getWinner(simBoard, lastPos);
        }

        simPlayer = getOpponent(simPlayer);
    }

    return Player::None;
}

void backpropagate(Node*& node, Player winner) {
    while (node != nullptr) {
        node->visits++;  // Increase the number of visits
        if (Player::O == winner) {
            node->wins++;  // If the winner is O, increase the number of wins
        }
        node = node->parent;  // Move to the parent node
    }
}

Node* mcts(Node* root, const int& iterations) {
    Position p = 0;
    // The same heurictis but not random, just check the best move
    p = checkConsecutiveArea(root->boardState, root->currentPlayer, WIN_CONDITION);
    if (p == 0) p = checkConsecutiveArea(root->boardState, getOpponent(root->currentPlayer), WIN_CONDITION);
    if (p == 0) p = checkConsecutiveArea(root->boardState, root->currentPlayer, WIN_CONDITION - 1);
    if (p == 0) p = checkConsecutiveArea(root->boardState, getOpponent(root->currentPlayer), WIN_CONDITION - 1);
    if (p != 0) {
        root->boardState[p] = root->currentPlayer;
        root->lastMove = p;
        return root;
    }

    for (int i = 0; i < iterations; i++) {
        // Choose the best node to expand
        Node* selectedNode = select(root);
        selectedNode = selectedNode ? selectedNode : root;

        // Mở rộng cây
        expand(selectedNode);

        // Simulate the game
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
