#ifndef GAME_H
#define GAME_H

#include <vector>

#include "Board.h"
#include "Player.h"
#include "Bot.h"
#include "ReplayManager.h"
#include "PlayerManager.h"

using namespace std;

class Game
{
private:
    Board board;
    Player player1;
    Player player2;
    vector<Move> moveHistory;

public:
    Game();

    bool startPvP();
    bool startPlayerVsBot(BotLevel selectedLevel);

private:
    void displayCurrentBoard() const;
    bool inputMoveOrCommand(const Player &player, int &row, int &col);
    bool askExitMatch();
    void displayHelp() const;
    void askSaveReplay();
    void waitForEnter() const;
    void clearScreen() const;
};

#endif
