#include "../include/Board.h"
using namespace std;

Board::Board()
{
    resetBoard();
}

void Board::resetBoard()
{
    moveCount = 0; // khởi tạo lại số nước đi về 0

    for (int row = 0; row < size; row++) // duyệt qua từng hàng của bàn cờ
    {
        for (int col = 0; col < size; col++) // duyệt qua từng cột của bàn cờ
        {
            board[row][col] = ' '; // khởi tạo tất cả các ô trên bàn cờ thành trống
        }
    }
}

void Board::displayBoard() const
{
    cout << "\n=> Player 1 = X, Player 2/Bot = O\n\n"; //

    cout << "    ";
    for (int col = 0; col < size; col++) // hiển thị số cột ở đầu mỗi cột của bàn cờ
    {
        cout << " " << col << "  ";
    }
    cout << endl;

    for (int row = 0; row < size; row++) // duyệt qua từng hàng của bàn cờ để hiển thị
    {
        cout << "   ";
        for (int col = 0; col < size; col++) // hiển thị đường kẻ ngang giữa các hàng của bàn cờ
        {
            cout << "|---";
        }
        cout << "|\n";

        cout << " " << row << " ";
        for (int col = 0; col < size; col++) // hiển thị giá trị của mỗi ô trên bàn cờ, nếu ô đó trống sẽ hiển thị khoảng trắng, nếu có quân cờ sẽ hiển thị ký hiệu của quân cờ đó (X hoặc O)
        {
            cout << "| " << board[row][col] << " ";
        }
        cout << "|\n";
    }

    cout << "   ";
    for (int col = 0; col < size; col++) // hiển thị đường kẻ ngang ở cuối cùng của bàn cờ
    {
        cout << "|---";
    }
    cout << "|\n";
}

bool Board::checkFull() const
{
    return moveCount >= size * size;
}

bool Board::isInsideBoard(int row, int col) const
{
    if (row < 0 || row >= size || col < 0 || col >= size) // kiểm tra xem hàng hoặc cột có nằm ngoài phạm vi của bàn cờ không(nằm từ 0 - 9)
    {
        cout << "Row or column is outside the board. Please enter a value from 0 to 9.\n";
        return false; // nếu nằm ngoài phạm vi sẽ trả về false và hiển thị thông báo lỗi
    }

    return true; // nếu nằm trong phạm vi sẽ trả về true
}

bool Board::isEmpty(int row, int col) const
{
    if (!isInsideBoard(row, col)) // kiểm tra xem hàng hoặc cột có nằm ngoài phạm vi của bàn cờ không(nằm từ 0 - 9)
    {
        return false; // nếu nằm ngoài phạm vi sẽ trả về false
    }

    if (board[row][col] != ' ') // kiểm tra xem ô đó có trống không
    {
        cout << "This cell is already occupied. Please choose another cell.\n";
        return false; // nếu ô đó đã có quân cờ sẽ trả về false và hiển thị thông báo lỗi
    }

    return true; // nếu ô đó trống sẽ trả về true
}

//
bool Board::placeMove(int row, int col, char symbol)
{
    if (!isEmpty(row, col)) // kiểm tra ô cờ có trống hay không
    {
        return false; // nếu ô đó đã có quân cờ sẽ trả về false và không đặt được quân cờ
    }

    board[row][col] = symbol; // đặt quân cờ vào ô đó trên bàn cờ
    moveCount++;              // tăng số nước đi đã được thực hiện

    return true; // nếu đặt được quân cờ sẽ trả về true
}

char Board::getCell(int row, int col) const
{
    if (!isInsideBoard(row, col)) // kiểm tra xem hàng hoặc cột có nằm ngoài phạm vi của bàn cờ không(nằm từ 0 - 9)
    {
        return '\0'; // nếu nằm ngoài phạm vi sẽ trả về ký tự null để báo lỗi
    }

    return board[row][col]; // trả về giá trị của ô đó trên bàn cờ, có thể là 'X', 'O' hoặc ' ' (trống)
}

void Board::setCellForSimulation(int row, int col, char symbol)
{
    if (isInsideBoard(row, col)) // kiểm tra xem hàng hoặc cột có nằm ngoài phạm vi của bàn cờ không(nằm từ 0 - 9)
    {
        board[row][col] = symbol; // đặt quân cờ
    }
}

bool Board::checkWin(int row, int col, char symbol) const
{
    int directions[4][2] = {
        // 4 hướng cần kiểm tra: ngang, dọc, chéo chính, chéo phụ
        {0, 1}, // ngang
        {1, 0}, // dọc
        {1, 1}, // chéo chính
        {1, -1} // chéo phụ
    };

    for (int i = 0; i < 4; i++) // duyệt 4 hướng quanh quân cờ vừa đặt để kiểm tra xem có 5 quân cờ liên tiếp nào không
    {
        int count = 1; // quân cờ vừa đặt đã được tính là 1

        int dx = directions[i][0]; // lấy độ lệch hàng của hướng đang kiểm tra
        int dy = directions[i][1]; // lấy độ lệch cột của hướng đang kiểm tra

        int r = row + dx; // hàng của ô tiếp theo theo hướng đang kiểm tra
        int c = col + dy; // cột của ô tiếp theo theo hướng đang kiểm tra

        while (r >= 0 && r < size &&
               c >= 0 && c < size &&
               board[r][c] == symbol) // tiếp tục đếm nếu ô đó nằm trong bàn cờ và có cùng ký hiệu với quân cờ vừa đặt
        {
            count++; // tăng số quân cờ liên tiếp lên
            r += dx; // tiếp tục đếm sang ô tiếp theo theo hướng  dx
            c += dy; // tiếp tục đếm sang ô tiếp theo theo hướng dy
        }

        r = row - dx; // bắt đầu đếm từ ô ngay trước quân cờ vừa đặt theo hướng ngược lại của dx
        c = col - dy; // bắt đầu đếm từ ô ngay trước quân cờ vừa đặt theo hướng ngược lại của dy

        while (r >= 0 && r < size &&
               c >= 0 && c < size &&
               board[r][c] == symbol) // tiếp tục đếm nếu ô đó nằm trong bàn cờ và có cùng ký hiệu với quân cờ vừa đặt
        {
            count++; // tăng số quân cờ liên tiếp lên
            r -= dx; // tiếp tục đếm sang ô tiếp theo theo hướng ngược lại của dx
            c -= dy; // tiếp tục đếm sang ô tiếp theo theo hướng ngược lại của dy
        }

        if (count >= 5) // nếu có 5 quân cờ liên tiếp trở lên theo hướng đó thì người chơi đã thắng, trả về true
        {
            return true;
        }
    }

    return false;
}
