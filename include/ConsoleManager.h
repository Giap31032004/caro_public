#ifndef CONSOLE_MANAGER_H
#define CONSOLE_MANAGER_H
// module ConsoleManager quản lý các thao tác liên quan đến console, tách module để dùng cho nhiều file , có thể phát triển cho sau này để thao tác với console

// xóa màn hình console
void clearScreen();
// tạm dừng màn hình console để người dùng có thể đọc thông tin trước khi tiếp tục tránh việc xóa màn hình quá nhanh
void pauseScreen();

#endif