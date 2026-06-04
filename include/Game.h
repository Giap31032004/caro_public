#ifndef GAME_H
#define GAME_H

#include <vector>

#include "Board.h"
#include "Player.h"
#include "Bot.h"
#include "ReplayManager.h"
#include "PlayerManager.h"

using namespace std;
// module Game quản lý logic của trò chơi caro
class Game
{
private:
    Board board; // bàn cờ 
    Player player1; // người chơi 1
    Player player2; // người chơi 2 hoặc bot
    vector<Move> moveHistory; // lưu trữ lịch sử các nước đi trong trận đấu

public:
    Game(); // constructer mặc định

    void startPvP(); // bắt đầu trò chơi Player vs Player
    void startPlayerVsBot(BotLevel selectedLevel); // bắt đầu trò chơi Player vs Bot với độ khó được chọn

private:
    void displayCurrentBoard() const; // xóa màn hình và hiển thị bàn cờ hiện tại 
    void askSaveReplay(); // hỏi người chơi xem có muốn lưu lịch sử trận đấu hay không 
    void waitForEnter() const; // dừng màn hình để người chơi đọc thông báo trước khi tiếp tục
    void clearScreen() const; // xóa màn hình console
};

#endif
