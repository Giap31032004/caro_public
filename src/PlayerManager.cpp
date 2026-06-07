#include "../include/PlayerManager.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;

namespace
{
    const int MAX_PLAYER_NAME_LENGTH = 20;

    string trim(const string &value)
    {
        size_t first = value.find_first_not_of(" \t\r\n");

        if (first == string::npos)
        {
            return "";
        }

        size_t last = value.find_last_not_of(" \t\r\n");
        return value.substr(first, last - first + 1);
    }

    string toLowerCase(string value)
    {
        for (char &ch : value)
        {
            // tolower doi chu hoa ve chu thuong de so sanh ten khong phan biet hoa/thuong
            ch = static_cast<char>(
                tolower(static_cast<unsigned char>(ch)));
        }

        return value;
    }

    string normalizeName(const string &name)
    {
        string result = trim(name);

        if (result.length() > MAX_PLAYER_NAME_LENGTH)
        {
            result = result.substr(0, MAX_PLAYER_NAME_LENGTH);
        }

        return result;
    }

    bool isValidName(const string &name)
    {
        if (name.empty())
        {
            return false;
        }

        for (char ch : name)
        {
            unsigned char value = static_cast<unsigned char>(ch);

            // isalnum kiem tra ky tu co phai chu cai hoac chu so hay khong.
            if (!isalnum(value) && ch != '_' && ch != '-')
            {
                return false;
            }
        }

        return true;
    }

    int totalMatches(const Player &player)
    {
        return player.getWin() + player.getLoss() + player.getDraw();
    }

    double winRate(const Player &player)
    {
        int total = totalMatches(player);

        if (total == 0)
        {
            return 0.0;
        }

        return static_cast<double>(player.getWin()) / total;
    }

    void printPlayerHeader()
    {
        cout << left
             << setw(18) << "Name"
             << setw(8) << "Wins"
             << setw(8) << "Losses"
             << setw(8) << "Draws"
             << setw(8) << "Total"
             << "WinRate" << endl;
        cout << string(65, '-') << endl;
    }

    void printPlayerRow(const Player &player)
    {
        double rate = winRate(player);

        cout << left
             << setw(18) << player.getName()
             << setw(8) << player.getWin()
             << setw(8) << player.getLoss()
             << setw(8) << player.getDraw()
             << setw(8) << totalMatches(player)
             << fixed << setprecision(2)
             << rate * 100 << "%" << endl;
    }
}

PlayerManager::PlayerManager()
{
    fileName = "data/players.txt";
}

void PlayerManager::loadPlayers()
{
    players.clear();

    ifstream file(fileName);

    if (!file)
    {
        return;
    }

    string line;
    int lineNumber = 0;

    while (getline(file, line))
    {
        lineNumber++;

        if (trim(line).empty())
        {
            continue;
        }

        string name;
        string extra;
        int wins;
        int losses;
        int draws;
        istringstream lineStream(line);

        // Doc tung dong de bo qua du lieu loi ma khong lam dung chuong trinh
        if (!(lineStream >> name >> wins >> losses >> draws) ||
            (lineStream >> extra) ||
            wins < 0 || losses < 0 || draws < 0)
        {
            cout << "Warning: invalid player data at line "
                 << lineNumber << ". Skipped.\n";
            continue;
        }

        name = normalizeName(name);

        if (!isValidName(name))
        {
            cout << "Warning: invalid player name at line "
                 << lineNumber << ". Skipped.\n";
            continue;
        }

        string key = toLowerCase(name);
        bool found = false;

        // kiem tra player da ton tai chua 
        for (Player &player : players)
        {
            if (toLowerCase(player.getName()) == key)
            {
                player.setWin(player.getWin() + wins);
                player.setLoss(player.getLoss() + losses);
                player.setDraw(player.getDraw() + draws);
                found = true;
                break;
            }
        }

        if (!found)
        {
            players.push_back(Player(name, ' ', wins, losses, draws));
        }
    }
}

void PlayerManager::savePlayers()
{
    ofstream file(fileName);

    if (!file)
    {
        cout << "Cannot save player data.\n";
        return;
    }

    for (const Player &player : players)
    {
        file << player.getName() << " "
             << player.getWin() << " "
             << player.getLoss() << " "
             << player.getDraw() << endl;
    }
}

int PlayerManager::findPlayerIndex(string name)
{
    string target = toLowerCase(normalizeName(name));

    for (int i = 0; i < players.size(); i++)
    {
        if (toLowerCase(players[i].getName()) == target)
        {
            return i;
        }
    }

    return -1;
}

void PlayerManager::addPlayerIfNotExist(string name)
{
    name = normalizeName(name);

    if (!isValidName(name))
    {
        cout << "Invalid player name: " << name << endl;
        return;
    }

    if (findPlayerIndex(name) == -1)
    {
        players.push_back(Player(name, ' ', 0, 0, 0));
    }
}

void PlayerManager::updateWinLoss(string winnerName, string loserName)
{
    loadPlayers();

    winnerName = normalizeName(winnerName);
    loserName = normalizeName(loserName);

    addPlayerIfNotExist(winnerName);
    addPlayerIfNotExist(loserName);

    int winnerIndex = findPlayerIndex(winnerName);
    int loserIndex = findPlayerIndex(loserName);

    if (winnerIndex == -1 || loserIndex == -1)
    {
        cout << "Cannot update player statistics because a name is invalid.\n";
        return;
    }

    players[winnerIndex].addWin();
    players[loserIndex].addLoss();

    savePlayers();
}

void PlayerManager::updateDraw(string player1Name, string player2Name)
{
    loadPlayers();

    player1Name = normalizeName(player1Name);
    player2Name = normalizeName(player2Name);

    addPlayerIfNotExist(player1Name);
    addPlayerIfNotExist(player2Name);

    int p1Index = findPlayerIndex(player1Name);
    int p2Index = findPlayerIndex(player2Name);

    if (p1Index == -1 || p2Index == -1)
    {
        cout << "Cannot update player statistics because a name is invalid.\n";
        return;
    }

    players[p1Index].addDraw();
    players[p2Index].addDraw();

    savePlayers();
}

void PlayerManager::showAllPlayers()
{
    loadPlayers();

    if (players.empty())
    {
        cout << "No player information found.\n";
        return;
    }

    // Mac dinh sap xep theo ti le thang giam dan.
    sort(players.begin(), players.end(),
         [](const Player &leftPlayer, const Player &rightPlayer)
         {
             return winRate(leftPlayer) > winRate(rightPlayer);
         });

    cout << "\n===== PLAYER INFORMATION =====\n";
    printPlayerHeader();

    for (const Player &player : players)
    {
        printPlayerRow(player);
    }
}

void PlayerManager::searchPlayerByName()
{
    loadPlayers();

    string name;

    cout << "Enter player name: ";
    cin >> name;

    int index = findPlayerIndex(name);

    if (index == -1)
    {
        cout << "Player not found.\n";
        return;
    }

    cout << "\n===== PLAYER INFORMATION =====\n";
    printPlayerHeader();
    printPlayerRow(players[index]);
}

void PlayerManager::findEquivalentPlayer()
{
    loadPlayers();

    string name;

    cout << "Enter player name: ";
    cin >> name;

    int index = findPlayerIndex(name);

    if (index == -1)
    {
        cout << "Player not found.\n";
        return;
    }

    int targetTotal = totalMatches(players[index]);

    if (targetTotal == 0)
    {
        cout << "This player does not have enough data to calculate win rate.\n";
        return;
    }

    int bestIndex = -1;
    double minDifference = 999999.0;
    double targetRate = winRate(players[index]);

    for (int i = 0; i < players.size(); i++)
    {
        if (i == index || totalMatches(players[i]) == 0)
        {
            continue;
        }

        // Chi so sanh theo ti le thang gan nhat.
        double difference = abs(targetRate - winRate(players[i]));

        if (difference < minDifference)
        {
            minDifference = difference;
            bestIndex = i;
        }
    }

    if (bestIndex == -1)
    {
        cout << "No equivalent player found.\n";
        return;
    }

    cout << "\nEquivalent Player:\n";
    printPlayerHeader();
    printPlayerRow(players[bestIndex]);
}
