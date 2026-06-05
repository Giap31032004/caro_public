#include "../include/Menu.h"



int inputMenuChoice()
{
    int choice; // biến để lưu lựa chọn của người dùng

    while (true)
    {
        if (cin >> choice) // nếu người dùng nhập một số hợp lệ, trả về lựa chọn đó
        {
            return choice; // trả về lựa chọn của người dùng
        }

        cin.clear(); // nếu người dùng nhập không phải là số, xóa lỗi và bỏ qua phần còn lại của dòng nhập
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // bỏ qua phần còn lại của dòng nhập để chuẩn bị cho lần nhập tiếp theo
        cout << "Invalid input. Please enter a number.\n";
        cout << "Enter your choice: ";
    }
}

void menu()
{
    int choice; // biến để lưu lựa chọn của người dùng trong menu chính

    do
    {
        clearScreen();
        cout << "\n========== CARO GAME ==========\n";
        cout << "1. Player vs Player\n";
        cout << "2. Player vs Bot\n";
        cout << "3. Replay Game\n";
        cout << "4. Player Information\n";
        cout << "5. Search Player\n";
        cout << "6. Find Equivalent Player\n";
        cout << "7. Guide\n";
        cout << "8. Exit\n";
        cout << "================================\n";

        cout << "Enter your choice: ";
        choice = inputMenuChoice();

        switch (choice)
        {
        case 1: // nếu người dùng chọn 1, bắt đầu trò chơi Player vs Player
        {
            Game game;
            game.startPvP();
            break;
        }
        case 2: // nếu người dùng chọn 2, hiển thị menu phụ để chọn độ khó của bot và bắt đầu trò chơi Player vs Bot
        {
            playVsBotMenu();
            break;
        }
        case 3: // xem lại replay đã lưu, hiển thị menu phụ cho phần replay
        {
            replayMenu();
            break;
        }

        case 4: // xem thông tin người chơi
        {
            clearScreen();
            PlayerManager playerManager;
            playerManager.showAllPlayers();
            pauseScreen();
            break;
        }

        case 5: // tìm kiếm người chơi theo tên
        {
            clearScreen();
            PlayerManager playerManager;
            playerManager.searchPlayerByName();
            pauseScreen();
            break;
        }

        case 6: // tìm kiếm người chơi có thống kê thắng thua hòa tương đương nhau , theo level
        {
            clearScreen();
            PlayerManager playerManager;
            playerManager.findEquivalentPlayer();
            pauseScreen();
            break;
        }

        case 7: // hiển thị hướng dẫn chơi game
        {
            clearScreen();
            showGuide();
            pauseScreen();
            break;
        }

        case 8: // nếu người dùng chọn 8, thoát khỏi chương trình
            cout << "Thank you for playing!\n";
            break;

        default: // in báo nhập lựa chọn lỗi 
            cout << "Invalid choice. Please try again!\n";
        }

    } while (choice != 8);
}

void replayMenu()
{

    ReplayManager replayManager;

    int choice;

    do
    {
        cout << "\n===== REPLAY MENU =====\n";
        cout << "1. Show Replay List\n";
        cout << "2. Watch Replay By ID\n";
        cout << "3. Delete Replay By ID\n";
        cout << "4. Back\n";

        choice = inputMenuChoice();

        
        switch (choice)
        {
        case 1: // hiểu thị danh sách replay
            clearScreen();
            replayManager.showReplayList();
            pauseScreen();
            break;

        case 2: // xem replay theo id
            clearScreen();
            replayManager.replayById();
            pauseScreen();
            break;

        case 3: // xóa replay theo id
            clearScreen();
            replayManager.deleteReplayById();
            pauseScreen();
            break;
        }

    } while (choice != 4);
}

void playVsBotMenu()
{
    int choice;

    //
    do
    {
        cout << "\n===== BOT MENU =====\n";
        cout << "1. Easy Bot\n";
        cout << "2. Medium Bot\n";
        cout << "3. Hard Bot\n";
        cout << "4. Back\n";
        cout << "Enter your choice: ";
        choice = inputMenuChoice();

        //
        switch (choice)
        {
        case 1: // bắt đầu trò chơi Player vs Bot với độ khó Easy
        {
            Game game;
            if (!game.startPlayerVsBot(Easy))
            {
                choice = 4;
            }
            break;
        }

        case 2: // bắt đầu trò chơi Player vs Bot với độ khó Medium
        {
            Game game;
            if (!game.startPlayerVsBot(Medium))
            {
                choice = 4;
            }
            break;
        }

        case 3: // bắt đầu trò chơi Player vs Bot với độ khó Hard
        {
            Game game;
            if (!game.startPlayerVsBot(Hard))
            {
                choice = 4;
            }
            break;
        }

        case 4: // quay lại menu chính
            break;

        default:
            cout << "Invalid choice. Please try again!\n";
        }

    } while (choice != 4);
}

//
void showGuide()
{
    cout << "\n========== GAME GUIDE ==========\n";
    cout << "1. Board size: 10x10.\n";
    cout << "2. Player 1 uses X, Player 2/Bot uses O.\n";
    cout << "3. Enter move by row and column, for example: 3 5.\n";
    cout << "4. Row and column must be from 0 to 9.\n";
    cout << "5. You cannot choose a cell that already has X or O.\n";
    cout << "6. The first player who has 5 symbols in a row wins.\n";
    cout << "7. Winning lines can be horizontal, vertical, or diagonal.\n";
    cout << "8. If the board is full and nobody wins, the game is draw.\n";
    cout << "9. Replay lets you watch saved games by replay ID.\n";
    cout << "10. Player Information stores wins, losses, and draws.\n";
    cout << "================================\n";
}
