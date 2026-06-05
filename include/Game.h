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
    Board board; // đối tượng bàn cờ để quản lý trạng thái của trò chơi
    Player player1; // đối tượng người chơi 1, mặc định tên là "Player 1" và ký hiệu 'X', sẽ được cập nhật khi người chơi nhập tên
    Player player2; // đối tượng người chơi 2, mặc định tên là "Player 2" và ký hiệu 'O', sẽ được cập nhật khi người chơi nhập tên hoặc đặt thành "Bot" nếu chơi với máy
    vector<Move> moveHistory; // vector lưu trữ lịch sử các nước đi đã thực hiện trong trận đấu, mỗi Move bao gồm hàng, cột và ký hiệu của nước đi đó, dùng để lưu lại replay của trận đấu

public:
    Game();

    bool startPvP(); // bắt đầu trò chơi Player vs Player
    bool startPlayerVsBot(BotLevel selectedLevel); // bắt đầu trò chơi Player vs Bot với độ khó được chọn (Easy, Medium, Hard)

private:
    void displayCurrentBoard() const; // hiển thị bàn cờ hiện tại cùng với thông tin người chơi và hướng dẫn lệnh, được gọi sau mỗi nước đi để cập nhật giao diện cho người chơi
    bool inputMoveOrCommand(const Player &player, int &row, int &col); // yêu cầu người chơi nhập nước đi hoặc lệnh, nếu người chơi nhập lệnh thoát sẽ trả về false, nếu nhập nước đi hợp lệ sẽ trả về true và cập nhật row, col với tọa độ nước đi đó
    bool askExitMatch(); // hỏi người chơi có muốn thoát trận đấu hiện tại không
    void displayHelp() const; // hiển thị hướng dẫn sử dụng lệnh và quy tắc chơi
    void askSaveReplay(); // hỏi người chơi có muốn lưu lại lịch sử trận đấu dưới dạng replay không, nếu có sẽ gọi ReplayManager để lưu moveHistory vào file
    void waitForEnter() const; // tạm dừng màn hình
    void clearScreen() const; // xóa màn hình
};

#endif
