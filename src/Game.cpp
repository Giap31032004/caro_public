#include "../include/Game.h"

#include <cstdlib>
#include <iostream>
#include <limits>

using namespace std;

Game::Game()
    : player1("Player 1", 'X'),
      player2("Player 2", 'O')
{
}

void Game::startPvP()
{
    player1.inputName();
    player2.inputName();

    //
    board.resetBoard();
    moveHistory.clear();

    Player *currentPlayer = &player1;

    //
    while (true)
    {
        displayCurrentBoard();

        int row, col;

        currentPlayer->inputMove(row, col);

        if (!board.placeMove(row, col, currentPlayer->getSymbol()))
        {
            waitForEnter();
            continue;
        }

        moveHistory.push_back(
            {row, col, currentPlayer->getSymbol()});

        //
        if (board.checkWin(row, col, currentPlayer->getSymbol()))
        {
            displayCurrentBoard();

            cout << currentPlayer->getName() << " wins!\n";

            PlayerManager playerManager;

            if (currentPlayer == &player1)
            {
                playerManager.updateWinLoss(
                    player1.getName(),
                    player2.getName());
            }
            else
            {
                playerManager.updateWinLoss(
                    player2.getName(),
                    player1.getName());
            }

            askSaveReplay();
            clearScreen();
            break;
        }

        //
        if (board.checkFull())
        {
            displayCurrentBoard();

            cout << "It's a draw!\n";

            PlayerManager playerManager;

            playerManager.updateDraw(
                player1.getName(),
                player2.getName());

            askSaveReplay();
            clearScreen();
            break;
        }

        //
        if (currentPlayer == &player1)
        {
            currentPlayer = &player2;
        }
        else
        {
            currentPlayer = &player1;
        }
    }
}

void Game::startPlayerVsBot(BotLevel selectedLevel)
{
    player1.inputName();

    //
    Bot bot(
        selectedLevel,
        'O',
        player1.getSymbol());

    //
    board.resetBoard();
    moveHistory.clear();

    bool playerTurn = true;

    //
    while (true)
    {
        displayCurrentBoard();

        int row, col;
        char currentSymbol;

        //
        if (playerTurn)
        {
            player1.inputMove(row, col);
            currentSymbol = player1.getSymbol();

            if (!board.placeMove(row, col, currentSymbol))
            {
                waitForEnter();
                continue;
            }
        }
        else
        {
            //
            bot.makeMove(board, row, col);
            currentSymbol = bot.getSymbol();

            board.placeMove(row, col, currentSymbol);

            displayCurrentBoard();
            cout << "Bot moved at: " << row << " " << col << endl;
        }

        moveHistory.push_back(
            {row, col, currentSymbol});

        //
        if (board.checkWin(row, col, currentSymbol))
        {
            displayCurrentBoard();

            if (playerTurn)
            {
                cout << player1.getName() << " wins!\n";
            }
            else
            {
                cout << "Bot wins!\n";
            }

            askSaveReplay();
            clearScreen();
            break;
        }

        //
        if (board.checkFull())
        {
            displayCurrentBoard();

            cout << "It's a draw!\n";

            askSaveReplay();
            clearScreen();
            break;
        }

        playerTurn = !playerTurn;
    }
}

//
void Game::displayCurrentBoard() const
{
    clearScreen();
    board.displayBoard();
}

//
void Game::askSaveReplay()
{
    char saveChoice;

    cout << "Do you want to save the game history? (y/n): ";
    cin >> saveChoice;

    if (saveChoice == 'y' || saveChoice == 'Y')
    {
        ReplayManager replayManager;
        replayManager.saveReplay(moveHistory);
    }
}

//
void Game::waitForEnter() const
{
    cout << "Press Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

//
void Game::clearScreen() const
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
