#include "../include/Bot.h"
#include <cstdlib>
#include <ctime>

// Math utilities
int maxVal(int a, int b) { return (a > b) ? a : b; }
int minVal(int a, int b) { return (a < b) ? a : b; }

Bot::Bot(BotLevel l, char s, char enemy) {
    level = l;
    symbol = s;
    enemySymbol = enemy;
    srand((unsigned int)time(NULL));
}

char Bot::getSymbol() const {
    return symbol;
}

// Route to appropriate move strategy based on difficulty
void Bot::makeMove(Board &board, int &row, int &col) {
    if (level == Easy) {
        makeEasyMove(board, row, col);
    } 
    else if (level == Medium) {
        makeSmartMove(board, row, col, 2); // Depth 2 for Medium
    } 
    else {
        makeSmartMove(board, row, col, 4); // Depth 4 for Hard
    }
}

// Easy Mode: Random valid move
void Bot::makeEasyMove(Board &board, int &row, int &col) {
    do {
        row = rand() % Board::size;
        col = rand() % Board::size;
    } while (board.getCell(row, col) != ' '); 
}

// ==========================================
// MINIMAX ALGORITHM & UTILITIES
// ==========================================

bool Bot::checkWinSilent(Board &board, char sym) const {
    int dx[] = {0, 1, 1, 1};
    int dy[] = {1, 0, 1, -1};
    for (int x = 0; x < Board::size; x++) {
        for (int y = 0; y < Board::size; y++) {
            if (board.getCell(x, y) != sym) continue;
            for (int dir = 0; dir < 4; dir++) {
                int count = 1;
                for (int step = 1; step < 5; step++) {
                    int nx = x + step * dx[dir];
                    int ny = y + step * dy[dir];
                    if (nx >= 0 && nx < Board::size && ny >= 0 && ny < Board::size && board.getCell(nx, ny) == sym) {
                        count++;
                    } else break;
                }
                if (count >= 5) return true;
            }
        }
    }
    return false;
}

bool Bot::hasNeighbor(Board &board, int r, int c) const {
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue;
            int nr = r + i;
            int nc = c + j;
            if (nr >= 0 && nr < Board::size && nc >= 0 && nc < Board::size && board.getCell(nr, nc) != ' ') {
                return true;
            }
        }
    }
    return false;
}

int Bot::getCandidateMoves(Board &board, Point candidates[100]) const {
    int count = 0;
    for (int r = 0; r < Board::size; r++) {
        for (int c = 0; c < Board::size; c++) {
            if (board.getCell(r, c) == ' ' && hasNeighbor(board, r, c)) {
                candidates[count].r = r;
                candidates[count].c = c;
                count++;
            }
        }
    }
    // Fallback to center if board is empty
    if (count == 0) {
        candidates[0].r = Board::size / 2;
        candidates[0].c = Board::size / 2;
        count = 1;
    }
    return count;
}

int Bot::evaluateWindow(int botCount, int playerCount) const {
    if (botCount > 0 && playerCount > 0) return 0; // Blocked line
    if (botCount == 5) return 10000000;
    if (playerCount == 5) return -10000000;
    if (playerCount == 4) return -500000;
    if (botCount == 4) return 100000;
    if (playerCount == 3) return -10000;
    if (botCount == 3) return 2000;
    if (playerCount == 2) return -200;
    if (botCount == 2) return 50;
    return 0;
}

int Bot::evaluateBoard(Board &board) const {
    int score = 0;
    int limit = Board::size - 5;
    
    // 1. Horizontal
    for (int r = 0; r < Board::size; r++) {
        for (int c = 0; c <= limit; c++) {
            int b = 0, p = 0;
            for (int k = 0; k < 5; k++) {
                if (board.getCell(r, c+k) == symbol) b++;
                else if (board.getCell(r, c+k) == enemySymbol) p++;
            }
            score += evaluateWindow(b, p);
        }
    }
    // 2. Vertical
    for (int c = 0; c < Board::size; c++) {
        for (int r = 0; r <= limit; r++) {
            int b = 0, p = 0;
            for (int k = 0; k < 5; k++) {
                if (board.getCell(r+k, c) == symbol) b++;
                else if (board.getCell(r+k, c) == enemySymbol) p++;
            }
            score += evaluateWindow(b, p);
        }
    }
    // 3. Diagonal (\)
    for (int r = 0; r <= limit; r++) {
        for (int c = 0; c <= limit; c++) {
            int b = 0, p = 0;
            for (int k = 0; k < 5; k++) {
                if (board.getCell(r+k, c+k) == symbol) b++;
                else if (board.getCell(r+k, c+k) == enemySymbol) p++;
            }
            score += evaluateWindow(b, p);
        }
    }
    // 4. Anti-Diagonal (/)
    for (int r = 0; r <= limit; r++) {
        for (int c = 4; c < Board::size; c++) {
            int b = 0, p = 0;
            for (int k = 0; k < 5; k++) {
                if (board.getCell(r+k, c-k) == symbol) b++;
                else if (board.getCell(r+k, c-k) == enemySymbol) p++;
            }
            score += evaluateWindow(b, p);
        }
    }
    return score;
}

int Bot::minimax(Board &board, int depth, int maxDepth, int alpha, int beta, bool isMax) {
    if (checkWinSilent(board, symbol)) return 10000000 - depth;
    if (checkWinSilent(board, enemySymbol)) return -10000000 + depth;
    if (depth == maxDepth) return evaluateBoard(board);
    
    Point candidates[100];
    int numCandidates = getCandidateMoves(board, candidates);
    if (numCandidates == 0) return 0;

    if (isMax) {
        int bestScore = -INF;
        for (int i = 0; i < numCandidates; i++) {
            int r = candidates[i].r;
            int c = candidates[i].c;
            
            board.setCellForSimulation(r, c, symbol);
            int score = minimax(board, depth + 1, maxDepth, alpha, beta, false);
            board.setCellForSimulation(r, c, ' ');
            
            bestScore = maxVal(bestScore, score);
            alpha = maxVal(alpha, score);
            if (beta <= alpha) break; // Alpha-beta pruning
        }
        return bestScore;
    } else {
        int bestScore = INF;
        for (int i = 0; i < numCandidates; i++) {
            int r = candidates[i].r;
            int c = candidates[i].c;
            
            board.setCellForSimulation(r, c, enemySymbol);
            int score = minimax(board, depth + 1, maxDepth, alpha, beta, true);
            board.setCellForSimulation(r, c, ' ');            
            
            bestScore = minVal(bestScore, score);
            beta = minVal(beta, score);
            if (beta <= alpha) break; // Alpha-beta pruning
        }
        return bestScore;
    }
}

void Bot::makeSmartMove(Board &board, int &row, int &col, int targetDepth) {
    Point candidates[100];
    int numCandidates = getCandidateMoves(board, candidates);
    
    // Instinct 1: Win immediately
    for (int i = 0; i < numCandidates; i++) {
        board.setCellForSimulation(candidates[i].r, candidates[i].c, symbol);
        if (checkWinSilent(board, symbol)) {
            board.setCellForSimulation(candidates[i].r, candidates[i].c, ' ');
            row = candidates[i].r;
            col = candidates[i].c;
            return;
        }
        board.setCellForSimulation(candidates[i].r, candidates[i].c, ' ');
    }

    // Instinct 2: Block opponent's immediate win
    for (int i = 0; i < numCandidates; i++) {
        board.setCellForSimulation(candidates[i].r, candidates[i].c, enemySymbol);
        if (checkWinSilent(board, enemySymbol)) {
            board.setCellForSimulation(candidates[i].r, candidates[i].c, ' ');
            row = candidates[i].r;
            col = candidates[i].c;
            return;
        }
        board.setCellForSimulation(candidates[i].r, candidates[i].c, ' ');
    }

    int bestVal = -INF;
    row = -1;
    col = -1;

    // Execute deep search via Minimax
    for (int i = 0; i < numCandidates; i++) {
        int r = candidates[i].r;
        int c = candidates[i].c;
        
        board.setCellForSimulation(r, c, symbol); 
        int moveVal = minimax(board, 0, targetDepth, -INF, INF, false);
        board.setCellForSimulation(r, c, ' ');    

        if (moveVal > bestVal) {
            bestVal = moveVal;
            row = r;
            col = c;
        }
    }
    
    // Fallback to Easy mode if no valid move found
    if (row == -1 || col == -1) {
        makeEasyMove(board, row, col);
    }
}
