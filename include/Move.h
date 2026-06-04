#ifndef MOVE_H
#define Move_H

// module Move quản lý thông tin về một nước đi trong trò chơi caro
struct Move
{
    int row; // hàng của nước đi
    int col; // cột của nước đi 
    char symbol; // ký hiệu của người chơi (X hoặc O)
};

#endif