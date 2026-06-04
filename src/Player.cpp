#include "../include/Player.h"
#include <iostream>
#include <limits>

using namespace std;

Player::Player(string n, char s, int w, int l, int d)
    : name(n),
      symbol(s),
      win(w),
      loss(l),
      draw(d)
{
}

void Player::inputName()
{
    cout << "Enter player name: ";
    getline(cin >> ws, name);
}


void Player::inputMove(int &row, int &col)
{
    while (true)
    {
        cout << "Enter position (row and column, 0-9): ";
        if (!(cin >> row >> col)) // nếu người dùng nhập không phải là số, sẽ hiển thị thông báo lỗi và yêu cầu nhập lại
        {
            cin.clear(); // xóa lỗi nhập
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // bỏ qua phần còn lại của dòng nhập để chuẩn bị cho lần nhập tiếp theo
            cout << "Invalid input. Please try again.\n";
            continue;
        }

        if (row >= 0 && row <= 9 && col >= 0 && col <= 9) // nếu người dùng nhập số hợp lệ trong khoảng từ 0 đến 9, chấp nhận nước đi đó và thoát khỏi vòng lặp
        {
            break;
        }

        cout << "Coordinates must be between 0 and 9. Please try again.\n";
    }
}

int Player::getWin() const
{
    return win; // trả về số trận thắng của người chơi
}

int Player::getLoss() const
{
    return loss; // trả về số trận thua của người chơi
}

int Player::getDraw() const
{
    return draw; // trả về số trận hòa của người chơi
}


int Player::getTotalMatch() const
{
    return win + loss + draw; // trả về tổng số trận đã chơi của người chơi bằng cách cộng số trận thắng, thua và hòa lại với nhau
}

//
double Player::getWinRate() const
{
    if (win + loss == 0)
    {
        return 0.0; // nếu người chơi chưa chơi trận nào (tổng số trận thắng và thua bằng 0), trả về tỷ lệ thắng là 0 để tránh chia cho 0
    }

    return static_cast<double>(win) / (win + loss); // trả về tỷ lệ thắng của người chơi bằng cách chia số trận thắng cho tổng số trận thắng và thua (không tính trận hòa)
}

string Player::getName() const
{
    return name; // trả về tên của người chơi
}

char Player::getSymbol() const
{
    return symbol; // trả về ký hiệu của người chơi (X hoặc O)
}

void Player::setName(string n)
{
    name = n; // đặt tên của người chơi bằng giá trị được truyền vào tham số n
}

void Player::setSymbol(char s)
{
    symbol = s; // đặt ký hiệu của người chơi bằng giá trị được truyền vào tham số s (X hoặc O)
}

void Player::setWin(int w)
{
    win = w; // cheat game hehe
}

void Player::setLoss(int l)
{
    loss = l; // thiết lập số trận thua của người chơi bằng giá trị được truyền vào tham số l
}

void Player::setDraw(int d)
{
    draw = d; // thiết lập số trận hòa của người chơi bằng giá trị được truyền vào tham số d
}

void Player::addWin()
{
    win++; // tăng số trận thắng của người chơi lên 1 mỗi khi người chơi thắng một trận đấu
}

void Player::addLoss()
{
    loss++; // tăng số trận thua của người chơi lên 1 mỗi khi người chơi thua một trận đấu
}


void Player::addDraw()
{
    draw++; // tăng số trận hòa của người chơi lên 1 mỗi khi người chơi hòa một trận đấu
}

void Player::resetStatistic()
{
    win = 0;
    loss = 0;
    draw = 0;
}


void Player::displayInformation() const
{
    cout << "\n===== PLAYER INFORMATION =====\n";
    cout << "Name      : " << name << endl;
    cout << "Symbol    : " << symbol << endl;
    cout << "Wins      : " << win << endl;
    cout << "Losses    : " << loss << endl;
    cout << "Draws     : " << draw << endl;
    cout << "Total     : " << getTotalMatch() << endl;
    cout << "Win Rate  : " << getWinRate() * 100 << "%" << endl;
}
