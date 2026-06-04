#ifndef PLAYER_H
#define PLAYER_H

#include <string>
using namespace std;

// module Player quản lý thông tin và hành động của người chơi trong trò chơi caro
class Player
{
private:
    string name; // tên người chơi
    char symbol; // ký hiệu của người chơi (X hoặc O)
    int win; // số trận thắng
    int loss; // số trận thua
    int draw; // số trận hòa

public:
    Player(string n = "Player", char s = 'X', int w = 0, int l = 0, int d = 0); // constructer mặc định

    void inputName(); // nhập tên người chơi
    void inputMove(int &row, int &col); // nhập nước đi của người chơi

    int getWin() const; // lấy số trận thắng
    int getLoss() const; // lấy số trận thua
    int getDraw() const; // lấy số trận hòa
    int getTotalMatch() const; // lấy tổng số trận đã chơi

    double getWinRate() const; // lấy tỷ lệ thắng (số trận thắng / tổng số trận đã chơi) 

    string getName() const; // lấy tên người chơi
    char getSymbol() const; //  lấy ký hiệu của người chơi

    void setName(string n); // đặt tên người chơi
    void setSymbol(char s); // đặt ký hiệu của người chơi
    void setWin(int w); // đặt số trận thắng(cheat hehe)
    void setLoss(int l); // đặt số trận thua
    void setDraw(int d); // đặt số trận hòa
    void resetStatistic(); // đặt lại thống kê về số trận thắng, thua, hòa về 0

    void addWin(); // tăng số trận thắng lên 1
    void addLoss(); // tăng số trận thua lên 1
    void addDraw(); // tăng số trận hòa lên 1

    void displayInformation() const; // hiển thị thông tin của người chơi bao gồm tên, ký hiệu, số trận thắng, thua, hòa và tỷ lệ thắng
};

#endif