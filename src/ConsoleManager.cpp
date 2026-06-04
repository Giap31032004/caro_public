#include "../include/ConsoleManager.h"

#include <cstdlib>
#include <iostream>
#include <limits>

using namespace std;


void clearScreen()
{
#ifdef _WIN32 // nếu hệ điều hành là Windows
    system("cls"); // lệnh xóa
#else
    system("clear"); // nếu hệ điều hành là Unix/Linux/MacOS, sử dụng lệnh xóa khác
#endif
}


void pauseScreen()
{
    cout << "Press Enter to continue..."; // hiển thị thông báo yêu cầu người dùng nhấn Enter để tiếp tục
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // xóa bỏ bất kỳ ký tự nào còn lại trong bộ đệm nhập, đảm bảo rằng khi người dùng nhấn Enter, nó sẽ không bị bỏ qua
    cin.get(); // đợi người dùng nhấn Enter để tiếp tục chương trình
}
