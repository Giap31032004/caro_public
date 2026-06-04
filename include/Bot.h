#ifndef BOT_H
#define BOT_H

#include "Board.h"

enum BotLevel { Easy, Medium, Hard };

// Internal struct for Minimax coordinates
struct Point {
    int r;
    int c;
};

class Bot {
private:
    BotLevel level;
    char symbol;
    char enemySymbol;

    const int INF = 100000000;

    // --- MINIMAX UTILITY FUNCTIONS (Silent execution) ---
    
    // Check for win on the simulated board
    bool checkWinSilent(Board &board, char sym) const;
    
    // Check if an empty cell has adjacent pieces
    bool hasNeighbor(Board &board, int r, int c) const;
    
    // Get a list of potential moves to evaluate
    int getCandidateMoves(Board &board, Point candidates[100]) const;
    
    // Evaluate score for a 5-cell window
    int evaluateWindow(int botCount, int playerCount) const;
    
    // Evaluate the overall board state
    int evaluateBoard(Board &board) const;
    
    // Core Minimax algorithm with Alpha-Beta Pruning
    int minimax(Board &board, int depth, int maxDepth, int alpha, int beta, bool isMax);

    // --- MOVE STRATEGIES ---
    
    // Pick a random available move
    void makeEasyMove(Board &board, int &row, int &col);
    
    // Pick optimal move using Minimax
    void makeSmartMove(Board &board, int &row, int &col, int targetDepth);

public:
    Bot(BotLevel l, char s, char enemy);
    char getSymbol() const;
    
    // Main interface to execute a move
    void makeMove(Board &board, int &row, int &col);
};

#endif // BOT_H
