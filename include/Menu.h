#ifndef MENU_H
#define MENU_H
#include<iostream>
#include<limits>
#include "../include/Game.h"
#include "../include/ReplayManager.h"
#include "../include/Player.h"
#include "../include/PlayerManager.h"
#include "../include/ConsoleManager.h"

using namespace std;
// module Menu quản lý giao diện menu chính và các menu phụ của trò chơi caro

int inputMenuChoice(); // hàm nhập lựa chọn của người dùng trong menu, đảm bảo nhập hợp lệ tránh lỗi khi người dùng nhập không phải là số sẽ nhập lại nhiều lần
void menu(); // hàm hiển thị menu chính và xử lý lựa chọn của người dùng
void replayMenu(); // hàm hiển thị menu phụ cho phần replay, cho phép người dùng xem danh sách replay, xem replay theo ID hoặc xóa replay theo ID
void playVsBotMenu(); // hàm hiển thị menu phụ cho phần chơi với bot, cho phép người dùng chọn độ khó của bot (dễ, trung bình, khó)
void showGuide(); // hàm hiển thị hướng dẫn chơi game, bao gồm quy tắc chơi, cách nhập nước đi, cách thắng và các thông tin liên quan khác để người chơi hiểu rõ hơn về trò chơi caro

#endif