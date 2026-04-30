#include "Menu.h"
#include <iostream>

using namespace std;

int main() {
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
