#ifndef BOARD_H
#define BOARD_H

#include <iostream>

class Board
{
public:
    static const int size = 10; // kích thước của bàn cờ là 10x10

private:
    int moveCount; // đếm số nước đi đã được thực hiện trên bàn cờ
    int lastMoveRow; // lưu trữ hàng của nước đi cuối cùng để hiển thị dấu hiệu trên bàn cờ
    int lastMoveCol; // lưu trữ cột của nước đi cuối cùng để hiển thị dấu hiệu trên bàn cờ
    char board[size][size]; // mảng 2 chiều đại diện cho bàn cờ, mỗi ô có thể chứa 'X', 'O', hoặc ' ' (trống)

public:
    Board();

    void resetBoard(); // đặt lại bàn cờ về trạng thái ban đầu (tất cả ô trống, moveCount = 0, lastMoveRow/Col = -1)
    char getCell(int row, int col) const; // trả về ký hiệu tại ô (row, col), hoặc '\0' nếu ngoài phạm vi
    void setCellForSimulation(int row, int col, char symbol); // đặt ký hiệu vào ô (row, col) mà không cập nhật moveCount hay lastMoveRow/Col, dùng cho việc mô phỏng nước đi của bot
    bool isInsideBoard(int row, int col) const; // kiểm tra xem (row, col) có nằm trong phạm vi của bàn cờ hay không
    bool placeMove(int row, int col, char symbol); // cố gắng đặt ký hiệu vào ô (row, col), nếu thành công cập nhật moveCount và lastMoveRow/Col, trả về true nếu nước đi hợp lệ, false nếu ô đã có người đi hoặc ngoài phạm vi
    void displayBoard() const; // hiển thị bàn cờ lên console, đánh dấu nước đi cuối cùng bằng cách đặt nó trong dấu ngoặc vuông, ví dụ: [X] hoặc [O]
    bool checkFull() const; // kiểm tra xem bàn cờ đã đầy chưa (tức là không còn ô trống nào), trả về true nếu đầy, false nếu còn ô trống
    bool isEmpty(int row, int col) const; // kiểm tra xem ô (row, col) có trống hay không, trả về true nếu trống, false nếu đã có người đi
    bool checkWin(int row, int col, char symbol) const; // kiểm tra xem việc đặt ký hiệu tại ô (row, col) có tạo thành một chuỗi 5 ký hiệu liên tiếp theo hàng, cột, hoặc đường chéo nào đó hay không, trả về true nếu thắng, false nếu không thắng
};

#endif
