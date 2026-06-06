#include "../include/Game.h"
#include "../include/ConsoleManager.h"


#include <cctype>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

using namespace std;

Game::Game()
    : player1("Player 1", 'X'), // khởi tạo player1 với tên "Player 1" và ký hiệu 'X'
      player2("Player 2", 'O') // khởi tạo player2 với tên "Player 2" và ký hiệu 'O'
{
}

bool Game::startPvP()
{
    player1.inputName(); // yêu cầu người chơi 1 nhập tên
    player2.inputName(); // yêu cầu người chơi 2 nhập tên

    board.resetBoard(); // đặt lại bàn cờ về trạng thái ban đầu 
    moveHistory.clear(); // xóa lịch sử di chuyển trước đó nếu có

    Player *currentPlayer = &player1; // bắt đầu bằng người chơi 1

    while (true)
    {
        displayCurrentBoard(); // hiển thị bàn cờ hiện tại cùng với thông tin người chơi và hướng dẫn lệnh

        int row, col;
        if (!inputMoveOrCommand(*currentPlayer, row, col)) // yêu cầu người chơi nhập nước đi hoặc lệnh, nếu trả về false nghĩa là người chơi muốn thoát trận đấu
        {
            clearScreen(); // xóa màn hình trước khi thoát
            return false;
        }

        if (!board.placeMove(row, col, currentPlayer->getSymbol())) // cố gắng đặt nước đi trên bàn cờ, nếu không hợp lệ (ô đã có người đi hoặc ngoài phạm vi), yêu cầu người chơi nhập lại
        {
            pauseScreen(); // tạm dừng màn hình để người chơi có thể đọc thông báo lỗi trước khi nhập lại
            continue;
        }

        moveHistory.push_back({row, col, currentPlayer->getSymbol()}); // lưu lại nước đi vào lịch sử di chuyển để có thể lưu replay sau này

        if (board.checkWin(row, col, currentPlayer->getSymbol())) // kiểm tra xem nước đi vừa thực hiện có tạo thành chuỗi 5 ký hiệu liên tiếp nào đó hay không, nếu có nghĩa là người chơi hiện tại đã thắng
        {
            displayCurrentBoard();

            cout << currentPlayer->getName() << " wins!\n";

            PlayerManager playerManager; // tạo đối tượng PlayerManager để quản lý thông tin người chơi và cập nhật thống kê thắng thua

            if (currentPlayer == &player1)
            {
                playerManager.updateWinLoss(player1.getName(), player2.getName()); // cập nhật thống kê thắng thua cho player1 là thắng và player2 là thua
            }
            else
            {
                playerManager.updateWinLoss(player2.getName(), player1.getName()); // cập nhật thống kê thắng thua cho player2 là thắng và player1 là thua
            }

            askSaveReplay(); // hỏi người chơi có muốn lưu lại lịch sử trận đấu dưới dạng replay không
            clearScreen();
            return true;
        }

        if (board.checkFull()) // kiểm tra xem bàn cờ đã đầy chưa (tức là không còn ô trống nào)
        {
            displayCurrentBoard();

            cout << "It's a draw!\n";

            PlayerManager playerManager; // tạo đối tượng PlayerManager để quản lý thông tin người chơi và cập nhật thống kê hòa
            playerManager.updateDraw(player1.getName(), player2.getName()); // cập nhật thống kê hòa cho cả hai người chơi

            askSaveReplay(); // hỏi người chơi có muốn lưu lại lịch sử trận đấu dưới dạng replay không
            clearScreen();
            return true;
        }

        if (currentPlayer == &player1) // chuyển lượt sang người chơi 2 nếu hiện tại là người chơi 1 
        {
            currentPlayer = &player2;
        }
        else // chuyển lượt sang người chơi 1 nếu hiện tại là người chơi 2
        {
            currentPlayer = &player1;
        }
    }

    return true;
}

bool Game::startPlayerVsBot(BotLevel selectedLevel)
{
    player1.inputName(); // yêu cầu người chơi nhập tên, ký hiệu của người chơi sẽ mặc định là 'X'
    player2.setName("Bot"); // đặt tên người chơi 2 thành "Bot" vì đây là chế độ chơi với máy, ký hiệu của bot sẽ mặc định là 'O'
    player2.setSymbol('O'); // đặt ký hiệu của bot thành 'O', mặc định người chơi sẽ là 'X'

    Bot bot(selectedLevel, 'O', player1.getSymbol()); // tạo đối tượng Bot với độ khó được chọn, ký hiệu của bot là 'O' và ký hiệu của người chơi là 'X'

    board.resetBoard(); // đặt lại bàn cờ về trạng thái ban đầu
    moveHistory.clear(); // xóa lịch sử di chuyển trước đó nếu có

    bool playerTurn = true; // biến boolean để theo dõi lượt chơi, bắt đầu với người chơi trước, nếu playerTurn là true thì đến lượt người chơi, nếu false thì đến lượt bot

    while (true)
    {
        displayCurrentBoard(); // hiển thị bàn cờ hiện tại cùng với thông tin người chơi và hướng dẫn lệnh

        int row, col; // biến để lưu tọa độ nước đi mà người chơi hoặc bot sẽ thực hiện
        char currentSymbol; // biến để lưu ký hiệu của nước đi hiện tại, sẽ được cập nhật sau khi người chơi hoặc bot thực hiện nước đi

        if (playerTurn) // nếu đến lượt người chơi thực hiện nước đi
        {
            if (!inputMoveOrCommand(player1, row, col)) // yêu cầu người chơi nhập nước đi hoặc lệnh, nếu trả về false nghĩa là người chơi muốn thoát trận đấu
            {
                clearScreen();
                return false;
            }

            currentSymbol = player1.getSymbol(); // cập nhật ký hiệu của nước đi hiện tại thành ký hiệu của người chơi

            if (!board.placeMove(row, col, currentSymbol)) // cố gắng đặt nước đi trên bàn cờ, nếu không hợp lệ (ô đã có người đi hoặc ngoài phạm vi), yêu cầu người chơi nhập lại
            {
                pauseScreen();  // tạm dừng màn hình để người chơi có thể đọc thông báo lỗi trước khi nhập lại
                continue;
            }
        }
        else
        {
            bot.makeMove(board, row, col); // gọi phương thức makeMove của bot để bot thực hiện nước đi, phương thức này sẽ cập nhật row và col với tọa độ nước đi mà bot chọn
            currentSymbol = bot.getSymbol(); // cập nhật ký hiệu của nước đi hiện tại thành ký hiệu của bot

            board.placeMove(row, col, currentSymbol); 

            displayCurrentBoard();
            cout << "Bot moved at: " << row << " " << col << endl;
        }

        moveHistory.push_back({row, col, currentSymbol}); // lưu lại nước đi vào lịch sử di chuyển để có thể lưu replay sau này

        if (board.checkWin(row, col, currentSymbol)) //    kiểm tra xem nước đi vừa thực hiện có tạo thành chuỗi 5 ký hiệu liên tiếp nào đó hay không, nếu có nghĩa là người chơi hoặc bot đã thắng
        {
            displayCurrentBoard();

            if (playerTurn) // nếu lượt hiện tại là người chơi thì thông báo người chơi thắng, ngược lại nếu lượt hiện tại là bot thì thông báo bot thắng
            {
                cout << player1.getName() << " wins!\n";
            }
            else
            {
                cout << "Bot wins!\n";
            }

            askSaveReplay(); // hỏi người chơi có muốn lưu lại lịch sử trận đấu dưới dạng replay không
            clearScreen();
            return true;
        }

        if (board.checkFull()) // kiểm tra xem bàn cờ đã đầy chưa (tức là không còn ô trống nào)
        {
            displayCurrentBoard();

            cout << "It's a draw!\n";

            askSaveReplay(); // hỏi người chơi có muốn lưu lại lịch sử trận đấu dưới dạng replay không
            clearScreen();
            return true;
        }

        playerTurn = !playerTurn; 
    }

    return true;
}

void Game::displayCurrentBoard() const // hàm hiển thị bàn cờ hiện tại cùng với thông tin người chơi và hướng dẫn lệnh, được gọi sau mỗi nước đi để cập nhật giao diện cho người chơi
{
    clearScreen();
    cout << "\nPlayers: " << player1.getName() << " [" << player1.getSymbol() << "]"
         << "  |  " << player2.getName() << " [" << player2.getSymbol() << "]\n";
    cout << "Commands: enter row col, help, or exit.\n\n";
    board.displayBoard();
}

bool Game::inputMoveOrCommand(const Player &player, int &row, int &col) // hàm yêu cầu người chơi nhập nước đi hoặc lệnh, nếu người chơi nhập lệnh thoát sẽ trả về false, nếu nhập nước đi hợp lệ sẽ trả về true và cập nhật row, col với tọa độ nước đi đó
{
    while (true) // vòng lặp để liên tục yêu cầu người chơi nhập cho đến khi nhận được đầu vào hợp lệ (nước đi hoặc lệnh)
    {
        cout << "\n" << player.getName() << " [" << player.getSymbol()
             << "], enter move: "; 

        string line;
        getline(cin >> ws, line); // đọc toàn bộ dòng nhập vào, bao gồm cả khoảng trắng, và loại bỏ khoảng trắng ở đầu dòng nếu có

        string command = line; // sao chép dòng nhập vào thành một biến command để xử lý lệnh, nếu không phải là lệnh thì sẽ tiếp tục xử lý như nước đi
        // chuyển thành chữ thành 
        for (char &ch : command)
        {
            ch = static_cast<char>(tolower(static_cast<unsigned char>(ch))); 
        }

        if (command == "help" || command == "h" || command == "?") // nếu nhận được command dạng help,h, ?
        {
            displayHelp(); // hiển thị hướng dẫn 
            if (askExitMatch())  
            {
                return false;
            }
            continue;
        }

        if (command == "exit" || command == "quit" || command == "q") // nếu nhận được command dạng quit
        {
            if (askExitMatch()) // hỏi có muốn thoát hay không
            {
                return false; 
            }
            continue;
        }

        // đọc tọa độ
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
    char choice; // biến để lưu lựa chọn của người chơi khi được hỏi có muốn thoát trận đấu hay không

    cout << "Do you want to exit the current match? (y/n): ";
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // xóa bỏ bất kỳ ký tự nào còn lại trong bộ đệm nhập, đảm bảo rằng khi người dùng nhấn Enter, nó sẽ không bị bỏ qua

    if (choice == 'y' || choice == 'Y')
    {
        askSaveReplay(); // hỏi xem có lưu lại lịch sử trận đấu dưới dạng replay không trước khi thoát
        return true;
    }

    return false;
}

void Game::displayHelp() const // hàm hiển thị hướng dẫn sử dụng lệnh và quy tắc chơi
{
    cout << "\n===== HELP =====\n";
    cout << "Move  : type row and column, for example: 3 4\n";
    cout << "Marker: the latest move is displayed as [X] or [O]\n";
    cout << "Players: each player's symbol is shown above the board\n";
    cout << "Exit  : type exit, quit, q, help, h, or ? to choose leaving the match\n";
}

void Game::askSaveReplay()
{
    char saveChoice; // biến để lưu lựa chọn của người chơi khi được hỏi có muốn lưu lại lịch sử trận đấu dưới dạng replay không

    cout << "Do you want to save the game history? (y/n): ";
    cin >> saveChoice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // xóa bỏ bất kỳ ký tự nào còn lại trong bộ đệm nhập, đảm bảo rằng khi người dùng nhấn Enter, nó sẽ không bị bỏ qua

    if (saveChoice == 'y' || saveChoice == 'Y')
    {
        ReplayManager replayManager; // tạo đối tượng ReplayManager để quản lý việc lưu replay
        replayManager.saveReplay(moveHistory); // gọi phương thức saveReplay của ReplayManager để lưu moveHistory vào file
        cout << "Game history saved successfully.\n";
    }
}


