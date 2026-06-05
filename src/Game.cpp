#include "../include/Game.h"

#include <cctype>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

using namespace std;

Game::Game()
    : player1("Player 1", 'X'),
      player2("Player 2", 'O')
{
}

bool Game::startPvP()
{
    player1.inputName();
    player2.inputName();

    board.resetBoard();
    moveHistory.clear();

    Player *currentPlayer = &player1;

    while (true)
    {
        displayCurrentBoard();

        int row, col;
        if (!inputMoveOrCommand(*currentPlayer, row, col))
        {
            clearScreen();
            return false;
        }

        if (!board.placeMove(row, col, currentPlayer->getSymbol()))
        {
            waitForEnter();
            continue;
        }

        moveHistory.push_back({row, col, currentPlayer->getSymbol()});

        if (board.checkWin(row, col, currentPlayer->getSymbol()))
        {
            displayCurrentBoard();

            cout << currentPlayer->getName() << " wins!\n";

            PlayerManager playerManager;

            if (currentPlayer == &player1)
            {
                playerManager.updateWinLoss(player1.getName(), player2.getName());
            }
            else
            {
                playerManager.updateWinLoss(player2.getName(), player1.getName());
            }

            askSaveReplay();
            clearScreen();
            return true;
        }

        if (board.checkFull())
        {
            displayCurrentBoard();

            cout << "It's a draw!\n";

            PlayerManager playerManager;
            playerManager.updateDraw(player1.getName(), player2.getName());

            askSaveReplay();
            clearScreen();
            return true;
        }

        if (currentPlayer == &player1)
        {
            currentPlayer = &player2;
        }
        else
        {
            currentPlayer = &player1;
        }
    }

    return true;
}

bool Game::startPlayerVsBot(BotLevel selectedLevel)
{
    player1.inputName();
    player2.setName("Bot");
    player2.setSymbol('O');

    Bot bot(selectedLevel, 'O', player1.getSymbol());

    board.resetBoard();
    moveHistory.clear();

    bool playerTurn = true;

    while (true)
    {
        displayCurrentBoard();

        int row, col;
        char currentSymbol;

        if (playerTurn)
        {
            if (!inputMoveOrCommand(player1, row, col))
            {
                clearScreen();
                return false;
            }

            currentSymbol = player1.getSymbol();

            if (!board.placeMove(row, col, currentSymbol))
            {
                waitForEnter();
                continue;
            }
        }
        else
        {
            bot.makeMove(board, row, col);
            currentSymbol = bot.getSymbol();

            board.placeMove(row, col, currentSymbol);

            displayCurrentBoard();
            cout << "Bot moved at: " << row << " " << col << endl;
        }

        moveHistory.push_back({row, col, currentSymbol});

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
            return true;
        }

        if (board.checkFull())
        {
            displayCurrentBoard();

            cout << "It's a draw!\n";

            askSaveReplay();
            clearScreen();
            return true;
        }

        playerTurn = !playerTurn;
    }

    return true;
}

void Game::displayCurrentBoard() const
{
    clearScreen();
    cout << "\nPlayers: " << player1.getName() << " [" << player1.getSymbol() << "]"
         << "  |  " << player2.getName() << " [" << player2.getSymbol() << "]\n";
    cout << "Commands: enter row col, help, or exit.\n\n";
    board.displayBoard();
}

bool Game::inputMoveOrCommand(const Player &player, int &row, int &col)
{
    while (true)
    {
        cout << "\n" << player.getName() << " [" << player.getSymbol()
             << "], enter move: ";

        string line;
        getline(cin >> ws, line);

        string command = line;
        for (char &ch : command)
        {
            ch = static_cast<char>(tolower(static_cast<unsigned char>(ch)));
        }

        if (command == "help" || command == "h" || command == "?")
        {
            displayHelp();
            if (askExitMatch())
            {
                return false;
            }
            continue;
        }

        if (command == "exit" || command == "quit" || command == "q")
        {
            if (askExitMatch())
            {
                return false;
            }
            continue;
        }

        stringstream ss(line);
        if ((ss >> row >> col) && row >= 0 && row < Board::size &&
            col >= 0 && col < Board::size)
        {
            return true;
        }

        cout << "Invalid input. Enter coordinates from 0 to 9, or type help.\n";
    }
}

bool Game::askExitMatch()
{
    char choice;

    cout << "Do you want to exit the current match? (y/n): ";
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (choice == 'y' || choice == 'Y')
    {
        askSaveReplay();
        return true;
    }

    return false;
}

void Game::displayHelp() const
{
    cout << "\n===== HELP =====\n";
    cout << "Move  : type row and column, for example: 3 4\n";
    cout << "Marker: the latest move is displayed as [X] or [O]\n";
    cout << "Players: each player's symbol is shown above the board\n";
    cout << "Exit  : type exit, quit, q, help, h, or ? to choose leaving the match\n";
}

void Game::askSaveReplay()
{
    char saveChoice;

    cout << "Do you want to save the game history? (y/n): ";
    cin >> saveChoice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (saveChoice == 'y' || saveChoice == 'Y')
    {
        ReplayManager replayManager;
        replayManager.saveReplay(moveHistory);
    }
}

void Game::waitForEnter() const
{
    cout << "Press Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void Game::clearScreen() const
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
