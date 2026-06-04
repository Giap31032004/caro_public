#ifndef BOARD_H
#define BOARD_H

#include <iostream>
// module Board quản lý bàn cờ
class Board
{
public:
    static const int size = 10; // kích thước của bàn cờ (10x10)

private:
    int moveCount; // đếm số nước đi đã được thực hiện trên bàn cờ
    char board[size][size]; // mảng 2 chiều đại diện cho bàn cờ, mỗi ô có thể chứa 'X', 'O' hoặc ' ' (trống)

public:
    // constructer class board
    Board();

    // khởi tạo lại bàn cờ
    void resetBoard();
    // lấy giá trị của ô trên bàn cờ

    char getCell(int row, int col) const;
    // đặt giá trị của ô trên bàn cờ (dùng cho mô phỏng class bot)

    void setCellForSimulation(int row, int col, char symbol);
    // kiểm tra nước đi có nằm trong bàn cờ không

    bool isInsideBoard(int row, int col) const;
    // đặt quân cờ trên bàn cờ 

    bool placeMove(int row, int col, char symbol);
    // hiển thị bàn cờ 

    void displayBoard() const;
    // kiểm tra bàn cờ đã đầy chưa

    bool checkFull() const;
    // kiểm tra xem ô đó được đặt hay chưa

    bool isEmpty(int row, int col) const;
    // kiểm tra xem có 5 quân cờ liên tiếp không

    bool checkWin(int row, int col, char symbol) const;
};

#endif