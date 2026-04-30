#include "Menu.h"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

void setConsoleEncoding() {
#ifdef _WIN32
    // 设置控制台输出编码为UTF-8
    SetConsoleOutputCP(CP_UTF8);
    // 设置控制台输入编码为UTF-8
    SetConsoleCP(CP_UTF8);
    
    // 尝试设置控制台字体以支持中文
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = 0;
    cfi.dwFontSize.Y = 16;
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    wcscpy_s(cfi.FaceName, L"Consolas");
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
#endif
}

int main() {
    setConsoleEncoding();
    cout << "======================================" << endl;
    cout << "        学生成绩管理系统 v1.0        " << endl;
    cout << "======================================" << endl;
    cout << "欢迎使用学生成绩管理系统！" << endl;
    cout << "系统支持以下功能：" << endl;
    cout << "1. 学生信息管理（增删改查）" << endl;
    cout << "2. 成绩管理（科目设置、录入、修改、验证）" << endl;
    cout << "3. 成绩排序（总分、平均分、单科、学号）" << endl;
    cout << "4. 综合查询（班级、分数段、姓名、最低分）" << endl;
    cout << "======================================" << endl;
    
    Menu menu;
    menu.run();
    
    return 0;
}
