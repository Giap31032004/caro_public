#include <iostream>
#include <limits>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include "../include/Menu.h"
using namespace std;

int main()
{
    
    srand(time(nullptr)); // khởi tạo seed cho hàm rand() bằng thời gian hiện tại để đảm bảo rằng mỗi lần chạy chương trình sẽ có kết quả ngẫu nhiên khác nhau, đặc biệt là khi bot chọn nước đi ngẫu nhiên ở chế độ Easy
    menu(); // gọi hàm menu() để hiển thị menu chính và bắt đầu trò chơi caro
    return 0;
}
