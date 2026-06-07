#include "../include/ReplayManager.h"

#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>

using namespace std;

namespace
{
    struct ReplayIndexEntry
    {
        int id;
        string name;
        string fileName;
        int totalMoves;
    };

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

    string getReplayPath(const string &fileName)
    {
        // Ho tro ca du lieu cu: replay_1.txt va du lieu moi: data\\replay_1.txt.
        if (fileName.find("\\") != string::npos ||
            fileName.find("/") != string::npos ||
            fileName.find(":") != string::npos)
        {
            return fileName;
        }

        return "data\\" + fileName;
    }

    bool parseReplayIndexLine(const string &line, ReplayIndexEntry &entry)
    {
        size_t p1 = line.find('|');
        size_t p2 = line.find('|', p1 + 1);

        if (p1 == string::npos || p2 == string::npos)
        {
            return false;
        }

        size_t p3 = line.find('|', p2 + 1);
        string moveText = "-1";

        entry.id = 0;
        entry.name = trim(line.substr(p1 + 1, p2 - p1 - 1));

        if (p3 == string::npos)
        {
            entry.fileName = trim(line.substr(p2 + 1));
        }
        else
        {
            entry.fileName = trim(line.substr(p2 + 1, p3 - p2 - 1));
            moveText = trim(line.substr(p3 + 1));
        }

        try
        {
            entry.id = stoi(trim(line.substr(0, p1)));
            entry.totalMoves = stoi(moveText);
        }
        catch (...)
        {
            if (entry.id == 0)
            {
                return false;
            }

            entry.totalMoves = -1;
        }

        return !entry.fileName.empty();
    }

    bool readReplayId(int &targetId)
    {
        cout << "Enter replay ID: ";

        if (cin >> targetId)
        {
            // xoa het phan nhap du sau ID.
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return true;
        }

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid ID.\n";
        return false;
    }

    bool readReplayMoves(const string &fileName, vector<Move> &moves)
    {
        ifstream replayFile(fileName);

        if (!replayFile)
        {
            return false;
        }

        string line;
        int lineNumber = 0;

        while (getline(replayFile, line))
        {
            lineNumber++;
            line = trim(line);

            if (line.empty() || line[0] == '#')
            {
                continue;
            }

            Move move;
            string extra;
            // sstream: istringstream tach mot dong replay thanh row, col va symbol.
            istringstream lineStream(line);

            if (!(lineStream >> move.row >> move.col >> move.symbol) ||
                (lineStream >> extra))
            {
                cout << "Warning: invalid replay move at line "
                     << lineNumber << ". Skipped.\n";
                continue;
            }

            moves.push_back(move);
        }

        return true;
    }

}

ReplayManager::ReplayManager()
{
    indexFileName = "data/replays_index.txt";
}

void ReplayManager::saveReplay(const vector<Move> &moveHistory)
{
    string replayName;

    cin.ignore();
    cout << "Enter replay name: ";
    getline(cin, replayName);

    replayName = trim(replayName);

    if (replayName.empty())
    {
        replayName = "Replay";
    }

    int id = 1;
    string line;
    ifstream indexFile(indexFileName);

    // Lay max id + 1 de tranh trung id sau khi xoa replay.
    while (getline(indexFile, line))
    {
        ReplayIndexEntry entry;

        if (parseReplayIndexLine(line, entry) && entry.id >= id)
        {
            id = entry.id + 1;
        }
    }

    string replayFileName = "replay_" + to_string(id) + ".txt";
    ofstream replayFile("data/" + replayFileName);

    if (!replayFile)
    {
        cout << "Cannot save replay file.\n";
        return;
    }

    replayFile << "# Replay ID: " << id << endl;
    replayFile << "# Replay Name: " << replayName << endl;
    replayFile << "# Total Moves: " << moveHistory.size() << endl;

    for (const Move &move : moveHistory)
    {
        replayFile << move.row << " "
                   << move.col << " "
                   << move.symbol << endl;
    }

    // fstream: mo index o che do append de them replay moi vao cuoi file.
    ofstream outputIndex(indexFileName, ios::app);

    if (!outputIndex)
    {
        cout << "Cannot update replay index.\n";
        return;
    }

    outputIndex << id << "|"
                << replayName << "|"
                << replayFileName << "|"
                << moveHistory.size() << endl;

    cout << "Replay saved successfully.\n";
}

void ReplayManager::showReplayList()
{
    ifstream file(indexFileName);

    if (!file)
    {
        cout << "No replay found.\n";
        return;
    }

    string line;
    bool hasReplay = false;
    int lineNumber = 0;

    cout << "\n===== REPLAY LIST =====\n";
    cout << left
         << setw(6) << "ID"
         << setw(22) << "Name"
         << setw(18) << "File"
         << "Moves" << endl;
    cout << string(55, '-') << endl;

    while (getline(file, line))
    {
        lineNumber++;

        ReplayIndexEntry entry;

        if (!parseReplayIndexLine(line, entry))
        {
            cout << "Warning: invalid replay index at line "
                 << lineNumber << ". Skipped.\n";
            continue;
        }

        string replayPath = getReplayPath(entry.fileName);
        vector<Move> moves;

        hasReplay = true;

        if (entry.totalMoves < 0 && readReplayMoves(replayPath, moves))
        {
            entry.totalMoves = static_cast<int>(moves.size());
        }

        cout << left
             << setw(6) << entry.id
             << setw(22) << entry.name
             << setw(18) << entry.fileName
             << (entry.totalMoves >= 0 ? to_string(entry.totalMoves) : "-")
             << endl;
    }

    if (!hasReplay)
    {
        cout << "No replay found.\n";
    }
}

void ReplayManager::replayById()
{
    int targetId;

    if (!readReplayId(targetId))
    {
        return;
    }

    string line;
    ReplayIndexEntry selected = {0, "", "", -1};
    ifstream indexFile(indexFileName); // doc indexfile de tim id tuong ung

    while (getline(indexFile, line))
    {
        ReplayIndexEntry entry;

        if (parseReplayIndexLine(line, entry) && entry.id == targetId)
        {
            selected = entry;
            break;
        }
    }

    if (selected.id == 0)
    {
        cout << "Replay not found.\n";
        return;
    }

    Board board;
    vector<Move> moves;
    string replayPath = getReplayPath(selected.fileName);

    if (!readReplayMoves(replayPath, moves))
    {
        cout << "Replay file not found: " << replayPath << endl;
        return;
    }

    bool autoPlay = false;

    for (int i = 0; i < moves.size(); i++)
    {
        Move move = moves[i];

        board.placeMove(move.row, move.col, move.symbol);
        board.displayBoard();

        cout << "Step " << i + 1 << "/" << moves.size()
             << ": " << move.symbol << " -> ("
             << move.row << ", " << move.col << ")\n";

        if (autoPlay)
        {
            continue;
        }

        cout << "Press Enter to continue, q to quit, a to autoplay: ";

        string command;
        getline(cin, command);
        command = trim(command);

        if (command == "q" || command == "Q")
        {
            cout << "Replay stopped.\n";
            return;
        }

        if (command == "a" || command == "A")
        {
            autoPlay = true;
        }
    }
}

void ReplayManager::deleteReplayById()
{
    int targetId;

    if (!readReplayId(targetId))
    {
        return;
    }

    string line;
    string fileNameToDelete;
    vector<string> keptLines;
    ifstream input(indexFileName);

    while (getline(input, line))
    {
        ReplayIndexEntry entry;

        if (parseReplayIndexLine(line, entry) && entry.id == targetId)
        {
            fileNameToDelete = entry.fileName;
            continue;
        }

        keptLines.push_back(line);
    }

    if (fileNameToDelete.empty())
    {
        cout << "Replay not found.\n";
        return;
    }

    cout << "Are you sure you want to delete replay ID "
         << targetId << "? (y/n): ";

    char confirm;
    cin >> confirm;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // xoa input sau khi confirm

    if (confirm != 'y' && confirm != 'Y')
    {
        cout << "Delete cancelled.\n";
        return;
    }
    // ghi index moi vao file, thay the index cu, giu lai cac index khong bi xoa.
    ofstream output("data\\replays_index.tmp");

    if (!output)
    {
        cout << "Cannot write temporary replay index. Delete cancelled.\n";
        return;
    }

    for (const string &keptLine : keptLines)
    {
        output << keptLine << endl;
    }

    output.close();

    // xoa index cu, doi file tam thanh index moi.
    if (remove(indexFileName.c_str()) != 0 ||
        rename("data\\replays_index.tmp", indexFileName.c_str()) != 0)
    {
        cout << "Cannot update replay index. Replay file was not deleted.\n";
        return;
    }

    string replayPath = getReplayPath(fileNameToDelete);

    // remove xoa file replay tuong ung voi ID vua xoa.
    remove(replayPath.c_str());
    cout << "Replay deleted.\n";
}
