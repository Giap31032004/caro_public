#include "../include/Board.h"
using namespace std;

Board::Board()
{
    resetBoard();
}

void Board::resetBoard() 
{
    moveCount = 0; // đặt lại số nước đi đã thực hiện về 0
    lastMoveRow = -1; // đặt lại hàng của nước đi cuối cùng về -1 để không hiển thị dấu hiệu trên bàn cờ
    lastMoveCol = -1; // đặt lại cột của nước đi cuối cùng về -1 để không hiển thị dấu hiệu trên bàn cờ

    for (int row = 0; row < size; row++)  // lặp qua tất cả hàng của bàn cờ
    {
        for (int col = 0; col < size; col++) // lặp qua tất cả cột của bàn cờ
        {
            board[row][col] = ' '; // đặt tất cả ô trên bàn cờ về trạng thái trống (ký hiệu ' ')
        }
    }
}

void Board::displayBoard() const
{
    cout << "    ";
    for (int col = 0; col < size; col++)  // in số cột ở đầu mỗi cột để người chơi dễ nhập nước đi
    {
        cout << " " << col << "  ";
    }
    cout << endl;

    for (int row = 0; row < size; row++) // lặp qua tất cả hàng của bàn cờ để hiển thị
    {
        cout << "   ";
        for (int col = 0; col < size; col++) // in đường kẻ ngang giữa các hàng để tạo thành lưới cho bàn cờ
        {
            cout << "|---";
        }
        cout << "|\n";

        cout << " " << row << " "; // in số hàng ở đầu mỗi hàng để người chơi dễ nhập nước đi
        for (int col = 0; col < size; col++) 
        {
            if (row == lastMoveRow && col == lastMoveCol)
            {
                cout << "|[" << board[row][col] << "]"; // nếu ô này là nước đi cuối cùng, hiển thị nó trong dấu ngoặc vuông để đánh dấu
            }
            else
            {
                cout << "| " << board[row][col] << " "; // hiển thị ký hiệu tại ô (row, col), có thể là 'X', 'O', hoặc ' ' (trống) với định dạng để tạo thành lưới cho bàn cờ
            }
        }
        cout << "|\n";
    }

    cout << "   ";
    for (int col = 0; col < size; col++) // in đường kẻ ngang cuối cùng dưới cùng của bàn cờ để tạo thành lưới cho bàn cờ
    {
        cout << "|---";
    }
    cout << "|\n";
}

bool Board::checkFull() const
{
    return moveCount >= size * size;  // nếu số nước đi đã thực hiện lớn hơn hoặc bằng tổng số ô trên bàn cờ, nghĩa là bàn cờ đã đầy
}

bool Board::isInsideBoard(int row, int col) const
{
    if (row < 0 || row >= size || col < 0 || col >= size) // kiểm tra xem hàng và cột có nằm trong phạm vi của bàn cờ hay không
    {
        cout << "Row or column is outside the board. Please enter a value from 0 to 9.\n";
        return false;
    }

    return true;
}

bool Board::isEmpty(int row, int col) const
{
    if (!isInsideBoard(row, col)) // kiểm tra xem hàng và cột có nằm trong phạm vi của bàn cờ hay không
    {
        return false;
    }

    if (board[row][col] != ' ') // kiểm tra xem ô (row, col) có trống hay không, nếu không trống nghĩa là đã có người đi vào ô đó
    {
        cout << "This cell is already occupied. Please choose another cell.\n";
        return false;
    }

    return true;
}

bool Board::placeMove(int row, int col, char symbol)
{
    if (!isEmpty(row, col)) // kiểm tra xem ô (row, col) có trống hay không
    {
        return false;
    }
    // nếu ô trống, đặt ký hiệu vào ô đó, cập nhật moveCount và lastMoveRow/Col để hiển thị dấu hiệu trên bàn cờ
    board[row][col] = symbol;
    lastMoveRow = row;
    lastMoveCol = col;
    moveCount++;

    return true;
}

char Board::getCell(int row, int col) const
{
    if (!isInsideBoard(row, col)) // kiểm tra xem hàng và cột có nằm trong phạm vi của bàn cờ hay không
    {
        return '\0';
    }

    return board[row][col]; // trả về ký hiệu tại ô (row, col), có thể là 'X', 'O', hoặc ' ' (trống)
}

void Board::setCellForSimulation(int row, int col, char symbol)
{
    if (isInsideBoard(row, col)) // kiểm tra xem hàng và cột có nằm trong phạm vi của bàn cờ hay không
    {
        board[row][col] = symbol; // đặt ký hiệu vào ô (row, col) mà không cập nhật moveCount hay lastMoveRow/Col, dùng cho việc mô phỏng nước đi của bot
    }
}

bool Board::checkWin(int row, int col, char symbol) const
{
    int directions[4][2] = { // các hướng cần kiểm tra: ngang, dọc, chéo chính, chéo phụ
        {0, 1}, // hướng ngang
        {1, 0}, // hướng dọc
        {1, 1}, // hướng chéo chính
        {1, -1}}; // hướng chéo phụ

    for (int i = 0; i < 4; i++) // duyệt qua từng hướng 
    {
        int count = 1; // ô hiện tại đã đánh nên được tính là 1 

        int dx = directions[i][0]; // chọn phần tử trong mảng directions cho hướng muốn kiểm tra
        int dy = directions[i][1]; //  chọn phần tử trong mảng directions cho hướng muốn kiểm tra

        int r = row + dx; // hàng tăng lên dx
        int c = col + dy; // cột tăng lên dy 

        while (r >= 0 && r < size &&
               c >= 0 && c < size &&
               board[r][c] == symbol) // tiếp tục kiểm tra theo hướng đó nếu ô tiếp theo vẫn nằm trong phạm vi bàn cờ và có cùng ký hiệu
        {
            count++; // tăng số lượng ký hiệu liên tiếp lên 1
            r += dx; // tiếp tục di chuyển theo hướng đó
            c += dy;
        }

        r = row - dx; // hàng giảm xuống dx để kiểm tra ngược lại hướng đó
        c = col - dy;

        while (r >= 0 && r < size &&
               c >= 0 && c < size &&
               board[r][c] == symbol) // tiếp tục kiểm tra theo hướng ngược lại nếu ô tiếp theo vẫn nằm trong phạm vi bàn cờ và có cùng ký hiệu
        {
            count++;
            r -= dx; // tiếp tục di chuyển theo hướng ngược lại
            c -= dy;
        }

        if (count >= 5) // nếu có 5 ký hiệu liên tiếp trở lên theo hàng, cột hoặc đường chéo nào đó, trả về true để báo thắng
        {
            return true;
        }
    }

    return false;
}
