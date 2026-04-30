#include "Menu.h"
#include <iostream>
#include <iomanip>
#include <limits>

using namespace std;

// 构造函数
Menu::Menu() {
    manager = unique_ptr<ScoreManager>(new ScoreManager());
    dataFile = "students.dat";
    loadData();
}

// 析构函数
Menu::~Menu() {
    saveData();
}

// 加载数据
void Menu::loadData() {
    if (manager->loadFromFile(dataFile)) {
        cout << "数据加载成功！" << endl;
    } else {
        cout << "暂无数据文件，将创建新数据。" << endl;
    }
}

// 保存数据
void Menu::saveData() {
    if (manager->saveToFile(dataFile)) {
        cout << "数据保存成功！" << endl;
    } else {
        cout << "数据保存失败！" << endl;
    }
}

// 显示主菜单
void Menu::showMainMenu() {
    cout << endl;
    cout << "╔══════════════════════════════════╗" << endl;
    cout << "║     学生成绩管理系统 v2.0         ║" << endl;
    cout << "╠══════════════════════════════════╣" << endl;
    cout << "║  1. 信息管理                      ║" << endl;
    cout << "║  2. 成绩管理                      ║" << endl;
    cout << "║  3. 统计分析                      ║" << endl;
    cout << "║  4. 成绩排序                      ║" << endl;
    cout << "║  5. 综合查询                      ║" << endl;
    cout << "║  6. 数据管理                      ║" << endl;
    cout << "║  0. 退出系统                      ║" << endl;
    cout << "╚══════════════════════════════════╝" << endl;
    cout << "请选择功能：";
}

// 处理主菜单选择
void Menu::handleMainMenuChoice(int choice) {
    switch (choice) {
        case 1:
            showInfoMenu();
            break;
        case 2:
            showScoreMenu();
            break;
        case 3:
            showStatisticsMenu();
            break;
        case 4:
            showSortMenu();
            break;
        case 5:
            showQueryMenu();
            break;
        case 6:
            showDataMenu();
            break;
        case 0:
            cout << "感谢使用，再见！" << endl;
            break;
        default:
            cout << "无效的选择，请重新输入。" << endl;
    }
}

// 显示信息管理菜单
void Menu::showInfoMenu() {
    int choice;
    do {
        cout << endl;
        cout << "┌───────────── 学生信息管理 ────────────┐" << endl;
        cout << "│ 1. 添加学生                            │" << endl;
        cout << "│ 2. 删除学生                            │" << endl;
        cout << "│ 3. 修改学生信息                        │" << endl;
        cout << "│ 4. 查看学生列表                        │" << endl;
        cout << "│ 5. 查询学生                            │" << endl;
        cout << "│ 0. 返回主菜单                          │" << endl;
        cout << "└──────────────────────────────────┘" << endl;
        cout << "请选择功能：";
        cin >> choice;
        
        // 清除输入缓冲区
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        handleInfoMenuChoice(choice);
    } while (choice != 0);
}

// 处理信息菜单选择
void Menu::handleInfoMenuChoice(int choice) {
    switch (choice) {
        case 1:
            addStudentUI();
            break;
        case 2:
            deleteStudentUI();
            break;
        case 3:
            updateStudentUI();
            break;
        case 4:
            viewStudentsUI();
            break;
        case 5:
            searchStudentUI();
            break;
        case 0:
            break;
        default:
            cout << "无效的选择，请重新输入。" << endl;
    }
}

// 显示成绩管理菜单
void Menu::showScoreMenu() {
    int choice;
    do {
        cout << endl;
        cout << "┌───────────── 成绩管理 ────────────┐" << endl;
        cout << "│ 1. 科目设置                        │" << endl;
        cout << "│ 2. 录入成绩                        │" << endl;
        cout << "│ 3. 修改成绩                        │" << endl;
        cout << "│ 4. 验证成绩                        │" << endl;
        cout << "│ 0. 返回主菜单                      │" << endl;
        cout << "└────────────────────────────────────┘" << endl;
        cout << "请选择功能：";
        cin >> choice;
        
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        handleScoreMenuChoice(choice);
    } while (choice != 0);
}

// 处理成绩菜单选择
void Menu::handleScoreMenuChoice(int choice) {
    switch (choice) {
        case 1:
            subjectSettingsUI();
            break;
        case 2:
            inputScoreUI();
            break;
        case 3:
            modifyScoreUI();
            break;
        case 4:
            validateScoreUI();
            break;
        case 0:
            break;
        default:
            cout << "无效的选择，请重新输入。" << endl;
    }
}

// 显示成绩排序菜单
void Menu::showSortMenu() {
    int choice;
    do {
        cout << endl;
        cout << "┌───────────── 成绩排序 ────────────┐" << endl;
        cout << "│ 1. 按总分降序排序                  │" << endl;
        cout << "│ 2. 按总分升序排序                  │" << endl;
        cout << "│ 3. 按平均分排序                    │" << endl;
        cout << "│ 4. 按单科成绩排序                  │" << endl;
        cout << "│ 5. 按学号排序                      │" << endl;
        cout << "│ 0. 返回主菜单                      │" << endl;
        cout << "└────────────────────────────────────┘" << endl;
        cout << "请选择功能：";
        cin >> choice;
        
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        handleSortMenuChoice(choice);
    } while (choice != 0);
}

// 处理排序菜单选择
void Menu::handleSortMenuChoice(int choice) {
    switch (choice) {
        case 1:
            sortByTotalDirect(SORT_BY_TOTAL_DESC);
            break;
        case 2:
            sortByTotalDirect(SORT_BY_TOTAL_ASC);
            break;
        case 3:
            sortByAverageUI();
            break;
        case 4:
            sortBySubjectUI();
            break;
        case 5:
            sortByStudentIdUI();
            break;
        case 0:
            break;
        default:
            cout << "无效的选择，请重新输入。" << endl;
    }
}

// 显示综合查询菜单
void Menu::showQueryMenu() {
    int choice;
    do {
        cout << endl;
        cout << "┌───────────── 综合查询 ────────────┐" << endl;
        cout << "│ 1. 按班级查询                      │" << endl;
        cout << "│ 2. 按分数段查询                    │" << endl;
        cout << "│ 3. 按姓名模糊查询                  │" << endl;
        cout << "│ 4. 按成绩范围查询（≥某分数）       │" << endl;
        cout << "│ 0. 返回主菜单                      │" << endl;
        cout << "└────────────────────────────────────┘" << endl;
        cout << "请选择功能：";
        cin >> choice;
        
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        handleQueryMenuChoice(choice);
    } while (choice != 0);
}

// 处理查询菜单选择
void Menu::handleQueryMenuChoice(int choice) {
    switch (choice) {
        case 1:
            queryByClassUI();
            break;
        case 2:
            queryByScoreRangeUI();
            break;
        case 3:
            queryByNameUI();
            break;
        case 4:
            queryByMinScoreUI();
            break;
        case 0:
            break;
        default:
            cout << "无效的选择，请重新输入。" << endl;
    }
}

// 添加学生界面
void Menu::addStudentUI() {
    string id, name, className;
    
    cout << "请输入学号：";
    getline(cin, id);
    
    cout << "请输入姓名：";
    getline(cin, name);
    
    cout << "请输入班级：";
    getline(cin, className);
    
    Student student(id, name, className);
    
    if (manager->addStudent(student)) {
        cout << "学生添加成功！" << endl;
        saveData();
    } else {
        cout << "学生添加失败，学号可能已存在。" << endl;
    }
}

// 删除学生界面
void Menu::deleteStudentUI() {
    string id;
    
    cout << "请输入要删除的学生学号：";
    getline(cin, id);
    
    Student* student = manager->findStudentById(id);
    if (student == nullptr) {
        cout << "未找到该学生。" << endl;
        return;
    }
    
    cout << "确认要删除以下学生吗？" << endl;
    student->display();
    
    char confirm;
    cout << "确认删除？(y/n): ";
    cin >> confirm;
    cin.ignore();
    
    if (confirm == 'y' || confirm == 'Y') {
        if (manager->removeStudent(id)) {
            cout << "学生删除成功！" << endl;
            saveData();
        } else {
            cout << "学生删除失败。" << endl;
        }
    } else {
        cout << "已取消删除。" << endl;
    }
}

// 修改学生信息界面
void Menu::updateStudentUI() {
    string id;
    
    cout << "请输入要修改的学生学号：";
    getline(cin, id);
    
    Student* student = manager->findStudentById(id);
    if (student == nullptr) {
        cout << "未找到该学生。" << endl;
        return;
    }
    
    cout << "当前学生信息：" << endl;
    student->display();
    
    string newName, newClassName;
    
    cout << "请输入新姓名（直接回车保留原姓名）：";
    getline(cin, newName);
    
    cout << "请输入新班级（直接回车保留原班级）：";
    getline(cin, newClassName);
    
    if (!newName.empty()) {
        student->setName(newName);
    }
    
    if (!newClassName.empty()) {
        student->setClassName(newClassName);
    }
    
    cout << "学生信息修改成功！" << endl;
    saveData();
}

// 查看学生列表界面
void Menu::viewStudentsUI() {
    vector<Student> students = manager->getAllStudents();
    
    if (students.empty()) {
        cout << "暂无学生信息。" << endl;
        return;
    }
    
    vector<string> subjects = manager->getSubjects();
    
    // 显示表头
    cout << "+------------+--------+---------+";
    for (const auto& subject : subjects) {
        cout << "---------+";
    }
    cout << "-------+-------+" << endl;
    
    cout << "|   学号     |  姓名  |  班级   |";
    for (const auto& subject : subjects) {
        cout << "  " << setw(5) << subject << " |";
    }
    cout << " 平均分 |  等级 |" << endl;
    
    cout << "+------------+--------+---------+";
    for (const auto& subject : subjects) {
        cout << "---------+";
    }
    cout << "-------+-------+" << endl;
    
    // 显示每个学生
    for (const auto& student : students) {
        cout << "| " << setw(10) << student.getStudentId() << " |";
        cout << " " << setw(6) << student.getName() << " |";
        cout << " " << setw(7) << student.getClassName() << " |";
        
        for (const auto& subject : subjects) {
            if (student.hasScore(subject)) {
                cout << " " << fixed << setprecision(1) << setw(6) << student.getScore(subject) << " |";
            } else {
                cout << "  --.-- |";
            }
        }
        
        cout << " " << fixed << setprecision(1) << setw(5) << student.getAverageScore() << " |";
        cout << "   " << student.getGrade() << "  |" << endl;
    }
    
    cout << "+------------+--------+---------+";
    for (const auto& subject : subjects) {
        cout << "---------+";
    }
    cout << "-------+-------+" << endl;
}

// 查询学生界面
void Menu::searchStudentUI() {
    string id;
    
    cout << "请输入要查询的学生学号：";
    getline(cin, id);
    
    Student* student = manager->findStudentById(id);
    if (student == nullptr) {
        cout << "未找到该学生。" << endl;
        return;
    }
    
    displayStudentReport(*student);
}

// 显示学生成绩报告
void Menu::displayStudentReport(const Student& student) {
    cout << endl;
    cout << "═══════════════════════════════════════" << endl;
    cout << "          个人成绩报告" << endl;
    cout << "═══════════════════════════════════════" << endl;
    cout << "学号：" << student.getStudentId() << endl;
    cout << "姓名：" << student.getName() << endl;
    cout << "班级：" << student.getClassName() << endl;
    cout << "───────────────────────────────────────" << endl;
    cout << "各科成绩：" << endl;
    
    map<string, double> scores = student.getAllScores();
    for (const auto& pair : scores) {
        cout << "  " << pair.first << ": " << fixed << setprecision(1) << pair.second << endl;
    }
    
    cout << "───────────────────────────────────────" << endl;
    cout << "总分：" << fixed << setprecision(1) << student.getTotalScore() << endl;
    cout << "平均分：" << fixed << setprecision(1) << student.getAverageScore() << endl;
    cout << "班级排名：" << manager->getClassRank(student) << endl;
    cout << "等级评定：" << student.getGrade() << endl;
    
    // 等级说明
    cout << "───────────────────────────────────────" << endl;
    cout << "等级说明：" << endl;
    cout << "  A: 90-100分" << endl;
    cout << "  B: 80-89分" << endl;
    cout << "  C: 70-79分" << endl;
    cout << "  D: 60-69分" << endl;
    cout << "  F: 60分以下" << endl;
    cout << "═══════════════════════════════════════" << endl;
}

// 科目设置界面
void Menu::subjectSettingsUI() {
    int choice;
    do {
        cout << endl;
        cout << "┌───────────── 科目设置 ────────────┐" << endl;
        cout << "│ 当前支持的科目：" << endl;
        
        vector<string> subjects = manager->getSubjects();
        for (size_t i = 0; i < subjects.size(); i++) {
            cout << "│   " << (i + 1) << ". " << subjects[i] << endl;
        }
        
        cout << "│" << endl;
        cout << "│ 1. 添加科目                        │" << endl;
        cout << "│ 2. 删除科目                        │" << endl;
        cout << "│ 0. 返回上级菜单                    │" << endl;
        cout << "└────────────────────────────────────┘" << endl;
        cout << "请选择功能：";
        cin >> choice;
        
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                string subject;
                cout << "请输入要添加的科目名称：";
                getline(cin, subject);
                
                if (manager->addSubject(subject)) {
                    cout << "科目添加成功！" << endl;
                    saveData();
                } else {
                    cout << "科目添加失败，可能已存在。" << endl;
                }
                break;
            }
            case 2: {
                string subject;
                cout << "请输入要删除的科目名称：";
                getline(cin, subject);
                
                cout << "警告：删除科目将同时删除所有学生的该科目成绩！" << endl;
                char confirm;
                cout << "确认删除？(y/n): ";
                cin >> confirm;
                cin.ignore();
                
                if (confirm == 'y' || confirm == 'Y') {
                    if (manager->removeSubject(subject)) {
                        cout << "科目删除成功！" << endl;
                        saveData();
                    } else {
                        cout << "科目删除失败，可能不存在。" << endl;
                    }
                } else {
                    cout << "已取消删除。" << endl;
                }
                break;
            }
            case 0:
                break;
            default:
                cout << "无效的选择，请重新输入。" << endl;
        }
    } while (choice != 0);
}

// 录入成绩界面
void Menu::inputScoreUI() {
    string id;
    
    cout << "请输入学生学号：";
    getline(cin, id);
    
    Student* student = manager->findStudentById(id);
    if (student == nullptr) {
        cout << "未找到该学生。" << endl;
        return;
    }
    
    vector<string> subjects = manager->getSubjects();
    cout << "请输入 " << student->getName() << " 的各科成绩：" << endl;
    
    for (const auto& subject : subjects) {
        double score;
        cout << subject << ": ";
        cin >> score;
        
        while (!manager->validateScore(score)) {
            cout << "分数无效，请输入0-100之间的分数：";
            cin >> score;
        }
        
        student->setScore(subject, score);
    }
    
    student->setIsComplete(true);
    cout << "成绩录入成功！" << endl;
    saveData();
}

// 修改成绩界面
void Menu::modifyScoreUI() {
    string id;
    
    cout << "请输入学生学号：";
    getline(cin, id);
    
    Student* student = manager->findStudentById(id);
    if (student == nullptr) {
        cout << "未找到该学生。" << endl;
        return;
    }
    
    cout << "当前成绩：" << endl;
    map<string, double> scores = student->getAllScores();
    for (const auto& pair : scores) {
        cout << pair.first << ": " << fixed << setprecision(1) << pair.second << endl;
    }
    
    string subject;
    cout << "请输入要修改的科目名称：";
    getline(cin, subject);
    
    if (!manager->hasSubject(subject)) {
        cout << "科目不存在。" << endl;
        return;
    }
    
    double newScore;
    cout << "请输入新的 " << subject << " 成绩：";
    cin >> newScore;
    
    while (!manager->validateScore(newScore)) {
        cout << "分数无效，请输入0-100之间的分数：";
        cin >> newScore;
    }
    
    if (student->setScore(subject, newScore)) {
        cout << "成绩修改成功！" << endl;
        saveData();
    } else {
        cout << "成绩修改失败。" << endl;
    }
}

// 验证成绩界面
void Menu::validateScoreUI() {
    vector<Student> students = manager->getAllStudents();
    
    if (students.empty()) {
        cout << "暂无学生信息。" << endl;
        return;
    }
    
    cout << "成绩验证结果：" << endl;
    cout << "═══════════════════════════════════════" << endl;
    
    int validCount = 0;
    int invalidCount = 0;
    
    for (const auto& student : students) {
        bool hasInvalid = false;
        map<string, double> scores = student.getAllScores();
        
        for (const auto& pair : scores) {
            if (!manager->validateScore(pair.second)) {
                hasInvalid = true;
                break;
            }
        }
        
        if (hasInvalid) {
            cout << "❌ " << student.getName() << " (" << student.getStudentId() << "): 存在无效成绩" << endl;
            invalidCount++;
        } else {
            cout << "✅ " << student.getName() << " (" << student.getStudentId() << "): 成绩有效" << endl;
            validCount++;
        }
    }
    
    cout << "═══════════════════════════════════════" << endl;
    cout << "验证完成：有效 " << validCount << " 人，无效 " << invalidCount << " 人" << endl;
}

// 按总分排序界面
void Menu::sortByTotalUI() {
    int orderChoice;
    cout << "请选择排序方式：" << endl;
    cout << "1. 降序（从高到低）" << endl;
    cout << "2. 升序（从低到高）" << endl;
    cout << "请选择：";
    cin >> orderChoice;
    cin.ignore();
    
    SortType sortType;
    if (orderChoice == 2) {
        sortType = SORT_BY_TOTAL_ASC;
    } else {
        sortType = SORT_BY_TOTAL_DESC;
    }
    
    vector<Student> sortedStudents = manager->sortStudents(sortType);
    
    if (sortedStudents.empty()) {
        cout << "暂无学生信息。" << endl;
        return;
    }
    
    cout << "按总分排序结果：" << endl;
    cout << "+------+------------+--------+---------+---------+-------+" << endl;
    cout << "| 排名 |   学号     |  姓名  |  班级   |  总分   | 平均分 |" << endl;
    cout << "+------+------------+--------+---------+---------+-------+" << endl;
    
    for (size_t i = 0; i < sortedStudents.size(); i++) {
        cout << "| " << setw(4) << (i + 1) << " |";
        cout << " " << setw(10) << sortedStudents[i].getStudentId() << " |";
        cout << " " << setw(6) << sortedStudents[i].getName() << " |";
        cout << " " << setw(7) << sortedStudents[i].getClassName() << " |";
        cout << " " << fixed << setprecision(1) << setw(7) << sortedStudents[i].getTotalScore() << " |";
        cout << " " << fixed << setprecision(1) << setw(5) << sortedStudents[i].getAverageScore() << " |" << endl;
    }
    
    cout << "+------+------------+--------+---------+---------+-------+" << endl;
}

// 按总分排序（直接指定排序方式，不再让用户选择）
void Menu::sortByTotalDirect(SortType sortType) {
    vector<Student> sortedStudents = manager->sortStudents(sortType);
    
    if (sortedStudents.empty()) {
        cout << "暂无学生信息。" << endl;
        return;
    }
    
    string orderText = (sortType == SORT_BY_TOTAL_DESC) ? "降序" : "升序";
    cout << "按总分" << orderText << "排序结果：" << endl;
    cout << "+------+------------+--------+---------+---------+-------+" << endl;
    cout << "| 排名 |   学号     |  姓名  |  班级   |  总分   | 平均分 |" << endl;
    cout << "+------+------------+--------+---------+---------+-------+" << endl;
    
    for (size_t i = 0; i < sortedStudents.size(); i++) {
        cout << "| " << setw(4) << (i + 1) << " |";
        cout << " " << setw(10) << sortedStudents[i].getStudentId() << " |";
        cout << " " << setw(6) << sortedStudents[i].getName() << " |";
        cout << " " << setw(7) << sortedStudents[i].getClassName() << " |";
        cout << " " << fixed << setprecision(1) << setw(7) << sortedStudents[i].getTotalScore() << " |";
        cout << " " << fixed << setprecision(1) << setw(5) << sortedStudents[i].getAverageScore() << " |" << endl;
    }
    
    cout << "+------+------------+--------+---------+---------+-------+" << endl;
}

// 按平均分排序界面
void Menu::sortByAverageUI() {
    vector<Student> sortedStudents = manager->sortStudents(SORT_BY_AVERAGE);
    
    if (sortedStudents.empty()) {
        cout << "暂无学生信息。" << endl;
        return;
    }
    
    cout << "按平均分排序结果：" << endl;
    cout << "+------+------------+--------+---------+---------+-------+------+" << endl;
    cout << "| 排名 |   学号     |  姓名  |  班级   |  总分   | 平均分 | 等级 |" << endl;
    cout << "+------+------------+--------+---------+---------+-------+------+" << endl;
    
    for (size_t i = 0; i < sortedStudents.size(); i++) {
        cout << "| " << setw(4) << (i + 1) << " |";
        cout << " " << setw(10) << sortedStudents[i].getStudentId() << " |";
        cout << " " << setw(6) << sortedStudents[i].getName() << " |";
        cout << " " << setw(7) << sortedStudents[i].getClassName() << " |";
        cout << " " << fixed << setprecision(1) << setw(7) << sortedStudents[i].getTotalScore() << " |";
        cout << " " << fixed << setprecision(1) << setw(5) << sortedStudents[i].getAverageScore() << " |";
        cout << "  " << sortedStudents[i].getGrade() << "  |" << endl;
    }
    
    cout << "+------+------------+--------+---------+---------+-------+------+" << endl;
}

// 按单科成绩排序界面
void Menu::sortBySubjectUI() {
    vector<string> subjects = manager->getSubjects();
    
    if (subjects.empty()) {
        cout << "暂无科目信息。" << endl;
        return;
    }
    
    cout << "请选择要排序的科目：" << endl;
    for (size_t i = 0; i < subjects.size(); i++) {
        cout << (i + 1) << ". " << subjects[i] << endl;
    }
    
    int choice;
    cout << "请选择：";
    cin >> choice;
    cin.ignore();
    
    if (choice < 1 || choice > (int)subjects.size()) {
        cout << "无效的选择。" << endl;
        return;
    }
    
    string selectedSubject = subjects[choice - 1];
    vector<Student> sortedStudents = manager->sortStudents(SORT_BY_SUBJECT, selectedSubject);
    
    if (sortedStudents.empty()) {
        cout << "暂无学生信息。" << endl;
        return;
    }
    
    cout << "按 " << selectedSubject << " 成绩排序结果：" << endl;
    cout << "+------+------------+--------+---------+---------+-------+" << endl;
    cout << "| 排名 |   学号     |  姓名  |  班级   | " << setw(6) << selectedSubject << " | 平均分 |" << endl;
    cout << "+------+------------+--------+---------+---------+-------+" << endl;
    
    for (size_t i = 0; i < sortedStudents.size(); i++) {
        cout << "| " << setw(4) << (i + 1) << " |";
        cout << " " << setw(10) << sortedStudents[i].getStudentId() << " |";
        cout << " " << setw(6) << sortedStudents[i].getName() << " |";
        cout << " " << setw(7) << sortedStudents[i].getClassName() << " |";
        
        if (sortedStudents[i].hasScore(selectedSubject)) {
            cout << " " << fixed << setprecision(1) << setw(7) << sortedStudents[i].getScore(selectedSubject) << " |";
        } else {
            cout << "  --.-- |";
        }
        
        cout << " " << fixed << setprecision(1) << setw(5) << sortedStudents[i].getAverageScore() << " |" << endl;
    }
    
    cout << "+------+------------+--------+---------+---------+-------+" << endl;
}

// 按学号排序界面
void Menu::sortByStudentIdUI() {
    vector<Student> sortedStudents = manager->sortStudents(SORT_BY_STUDENT_ID);
    
    if (sortedStudents.empty()) {
        cout << "暂无学生信息。" << endl;
        return;
    }
    
    cout << "按学号排序结果：" << endl;
    cout << "+------------+--------+---------+---------+-------+------+" << endl;
    cout << "|   学号     |  姓名  |  班级   |  总分   | 平均分 | 等级 |" << endl;
    cout << "+------------+--------+---------+---------+-------+------+" << endl;
    
    for (const auto& student : sortedStudents) {
        cout << "| " << setw(10) << student.getStudentId() << " |";
        cout << " " << setw(6) << student.getName() << " |";
        cout << " " << setw(7) << student.getClassName() << " |";
        cout << " " << fixed << setprecision(1) << setw(7) << student.getTotalScore() << " |";
        cout << " " << fixed << setprecision(1) << setw(5) << student.getAverageScore() << " |";
        cout << "  " << student.getGrade() << "  |" << endl;
    }
    
    cout << "+------------+--------+---------+---------+-------+------+" << endl;
}

// 按班级查询界面
void Menu::queryByClassUI() {
    string className;
    cout << "请输入要查询的班级名称：";
    getline(cin, className);
    
    vector<Student> result = manager->queryStudents(QUERY_BY_CLASS, className);
    
    if (result.empty()) {
        cout << "未找到该班级的学生。" << endl;
        return;
    }
    
    cout << className << " 学生列表：" << endl;
    cout << "+------------+--------+---------+---------+-------+------+" << endl;
    cout << "|   学号     |  姓名  |  班级   |  总分   | 平均分 | 等级 |" << endl;
    cout << "+------------+--------+---------+---------+-------+------+" << endl;
    
    for (const auto& student : result) {
        cout << "| " << setw(10) << student.getStudentId() << " |";
        cout << " " << setw(6) << student.getName() << " |";
        cout << " " << setw(7) << student.getClassName() << " |";
        cout << " " << fixed << setprecision(1) << setw(7) << student.getTotalScore() << " |";
        cout << " " << fixed << setprecision(1) << setw(5) << student.getAverageScore() << " |";
        cout << "  " << student.getGrade() << "  |" << endl;
    }
    
    cout << "+------------+--------+---------+---------+-------+------+" << endl;
    cout << "共查询到 " << result.size() << " 名学生。" << endl;
}

// 按分数段查询界面
void Menu::queryByScoreRangeUI() {
    vector<string> subjects = manager->getSubjects();
    
    if (subjects.empty()) {
        cout << "暂无科目信息。" << endl;
        return;
    }
    
    cout << "请选择要查询的科目：" << endl;
    for (size_t i = 0; i < subjects.size(); i++) {
        cout << (i + 1) << ". " << subjects[i] << endl;
    }
    
    int choice;
    cout << "请选择：";
    cin >> choice;
    
    if (choice < 1 || choice > (int)subjects.size()) {
        cout << "无效的选择。" << endl;
        cin.ignore();
        return;
    }
    
    string selectedSubject = subjects[choice - 1];
    
    double minScore, maxScore;
    cout << "请输入最低分：";
    cin >> minScore;
    cout << "请输入最高分：";
    cin >> maxScore;
    cin.ignore();
    
    if (minScore > maxScore) {
        cout << "最低分不能大于最高分。" << endl;
        return;
    }
    
    vector<Student> result = manager->queryStudents(QUERY_BY_SCORE_RANGE, selectedSubject, minScore, maxScore);
    
    if (result.empty()) {
        cout << "未找到符合条件的学生。" << endl;
        return;
    }
    
    cout << selectedSubject << " 成绩在 " << minScore << "-" << maxScore << " 之间的学生：" << endl;
    cout << "+------------+--------+---------+---------+-------+" << endl;
    cout << "|   学号     |  姓名  |  班级   | " << setw(6) << selectedSubject << " | 平均分 |" << endl;
    cout << "+------------+--------+---------+---------+-------+" << endl;
    
    for (const auto& student : result) {
        cout << "| " << setw(10) << student.getStudentId() << " |";
        cout << " " << setw(6) << student.getName() << " |";
        cout << " " << setw(7) << student.getClassName() << " |";
        cout << " " << fixed << setprecision(1) << setw(7) << student.getScore(selectedSubject) << " |";
        cout << " " << fixed << setprecision(1) << setw(5) << student.getAverageScore() << " |" << endl;
    }
    
    cout << "+------------+--------+---------+---------+-------+" << endl;
    cout << "共查询到 " << result.size() << " 名学生。" << endl;
}

// 按姓名模糊查询界面
void Menu::queryByNameUI() {
    string keyword;
    cout << "请输入要查询的姓名关键词：";
    getline(cin, keyword);
    
    vector<Student> result = manager->queryStudents(QUERY_BY_NAME, keyword);
    
    if (result.empty()) {
        cout << "未找到包含关键词 \"" << keyword << "\" 的学生。" << endl;
        return;
    }
    
    cout << "姓名包含 \"" << keyword << "\" 的学生：" << endl;
    cout << "+------------+--------+---------+---------+-------+------+" << endl;
    cout << "|   学号     |  姓名  |  班级   |  总分   | 平均分 | 等级 |" << endl;
    cout << "+------------+--------+---------+---------+-------+------+" << endl;
    
    for (const auto& student : result) {
        cout << "| " << setw(10) << student.getStudentId() << " |";
        cout << " " << setw(6) << student.getName() << " |";
        cout << " " << setw(7) << student.getClassName() << " |";
        cout << " " << fixed << setprecision(1) << setw(7) << student.getTotalScore() << " |";
        cout << " " << fixed << setprecision(1) << setw(5) << student.getAverageScore() << " |";
        cout << "  " << student.getGrade() << "  |" << endl;
    }
    
    cout << "+------------+--------+---------+---------+-------+------+" << endl;
    cout << "共查询到 " << result.size() << " 名学生。" << endl;
}

// 按最低分数查询界面
void Menu::queryByMinScoreUI() {
    vector<string> subjects = manager->getSubjects();
    
    if (subjects.empty()) {
        cout << "暂无科目信息。" << endl;
        return;
    }
    
    cout << "请选择要查询的科目：" << endl;
    for (size_t i = 0; i < subjects.size(); i++) {
        cout << (i + 1) << ". " << subjects[i] << endl;
    }
    
    int choice;
    cout << "请选择：";
    cin >> choice;
    
    if (choice < 1 || choice > (int)subjects.size()) {
        cout << "无效的选择。" << endl;
        cin.ignore();
        return;
    }
    
    string selectedSubject = subjects[choice - 1];
    
    double minScore;
    cout << "请输入最低分数：";
    cin >> minScore;
    cin.ignore();
    
    vector<Student> result = manager->queryStudents(QUERY_BY_MIN_SCORE, selectedSubject, minScore);
    
    if (result.empty()) {
        cout << "未找到 " << selectedSubject << " 成绩 ≥ " << minScore << " 的学生。" << endl;
        return;
    }
    
    cout << selectedSubject << " 成绩 ≥ " << minScore << " 的学生：" << endl;
    cout << "+------------+--------+---------+---------+-------+" << endl;
    cout << "|   学号     |  姓名  |  班级   | " << setw(6) << selectedSubject << " | 平均分 |" << endl;
    cout << "+------------+--------+---------+---------+-------+" << endl;
    
    for (const auto& student : result) {
        cout << "| " << setw(10) << student.getStudentId() << " |";
        cout << " " << setw(6) << student.getName() << " |";
        cout << " " << setw(7) << student.getClassName() << " |";
        cout << " " << fixed << setprecision(1) << setw(7) << student.getScore(selectedSubject) << " |";
        cout << " " << fixed << setprecision(1) << setw(5) << student.getAverageScore() << " |" << endl;
    }
    
    cout << "+------------+--------+---------+---------+-------+" << endl;
    cout << "共查询到 " << result.size() << " 名学生。" << endl;
}

// 显示统计分析菜单
void Menu::showStatisticsMenu() {
    int choice;
    do {
        cout << endl;
        cout << "┌───────────── 统计分析 ────────────┐" << endl;
        cout << "│ 1. 个人统计                        │" << endl;
        cout << "│ 2. 班级统计                        │" << endl;
        cout << "│ 3. 分数段统计                      │" << endl;
        cout << "│ 0. 返回主菜单                      │" << endl;
        cout << "└────────────────────────────────────┘" << endl;
        cout << "请选择功能：";
        cin >> choice;
        
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        handleStatisticsMenuChoice(choice);
    } while (choice != 0);
}

// 处理统计分析菜单选择
void Menu::handleStatisticsMenuChoice(int choice) {
    switch (choice) {
        case 1:
            personalStatisticsUI();
            break;
        case 2:
            classStatisticsUI();
            break;
        case 3:
            scoreRangeStatisticsUI();
            break;
        case 0:
            break;
        default:
            cout << "无效的选择，请重新输入。" << endl;
    }
}

// 个人统计界面
void Menu::personalStatisticsUI() {
    string id;
    
    cout << "请输入要查询的学生学号：";
    getline(cin, id);
    
    Student* student = manager->findStudentById(id);
    if (student == nullptr) {
        cout << "未找到该学生。" << endl;
        return;
    }
    
    displayStudentReport(*student);
}

// 班级统计界面
void Menu::classStatisticsUI() {
    vector<string> classes = manager->getAllClasses();
    
    if (classes.empty()) {
        cout << "暂无班级信息。" << endl;
        return;
    }
    
    cout << "当前存在的班级：" << endl;
    for (size_t i = 0; i < classes.size(); i++) {
        cout << (i + 1) << ". " << classes[i] << endl;
    }
    
    int choice;
    cout << "请选择要统计的班级：";
    cin >> choice;
    cin.ignore();
    
    if (choice < 1 || choice > (int)classes.size()) {
        cout << "无效的选择。" << endl;
        return;
    }
    
    string className = classes[choice - 1];
    ScoreManager::ClassStatistics stats = manager->getClassStatistics(className);
    
    cout << endl;
    cout << "═══════════════════════════════════════════════════════════" << endl;
    cout << "                 班级统计报告 - " << className << endl;
    cout << "═══════════════════════════════════════════════════════════" << endl;
    cout << endl;
    cout << "【班级基本信息】" << endl;
    cout << "  班级名称：" << stats.className << endl;
    cout << "  班级人数：" << stats.totalStudents << " 人" << endl;
    cout << "  班级总平均分：" << fixed << setprecision(1) << stats.overallAverage << " 分" << endl;
    cout << endl;
    
    if (stats.totalMaxScore >= 0) {
        Student* maxStudent = manager->findStudentById(stats.totalMaxStudentId);
        string maxName = maxStudent ? maxStudent->getName() : "未知";
        cout << "  总分最高分：" << fixed << setprecision(1) << stats.totalMaxScore 
             << " 分 (" << maxName << " " << stats.totalMaxStudentId << ")" << endl;
    }
    if (stats.totalMinScore <= 100) {
        Student* minStudent = manager->findStudentById(stats.totalMinStudentId);
        string minName = minStudent ? minStudent->getName() : "未知";
        cout << "  总分最低分：" << fixed << setprecision(1) << stats.totalMinScore 
             << " 分 (" << minName << " " << stats.totalMinStudentId << ")" << endl;
    }
    cout << endl;
    
    cout << "【各科成绩统计】" << endl;
    cout << "─────────────────────────────────────────────────────────────" << endl;
    cout << " 科目    平均分   最高分          最低分          及格率  优秀率" << endl;
    cout << "─────────────────────────────────────────────────────────────" << endl;
    
    for (const auto& subj : stats.subjectStats) {
        cout << " " << setw(6) << subj.subject;
        cout << " " << fixed << setprecision(1) << setw(6) << subj.average;
        
        if (subj.maxScore >= 0 && !subj.maxStudentId.empty()) {
            Student* maxStudent = manager->findStudentById(subj.maxStudentId);
            string maxName = maxStudent ? maxStudent->getName() : "未知";
            cout << " " << fixed << setprecision(1) << setw(6) << subj.maxScore 
                 << "(" << maxName << ")";
        } else {
            cout << "    --.-- (无数据)";
        }
        
        if (subj.minScore <= 100 && !subj.minStudentId.empty()) {
            Student* minStudent = manager->findStudentById(subj.minStudentId);
            string minName = minStudent ? minStudent->getName() : "未知";
            cout << " " << fixed << setprecision(1) << setw(6) << subj.minScore 
                 << "(" << minName << ")";
        } else {
            cout << "    --.-- (无数据)";
        }
        
        cout << " " << fixed << setprecision(1) << setw(5) << subj.passRate << "%";
        cout << " " << fixed << setprecision(1) << setw(5) << subj.excellentRate << "%";
        cout << endl;
    }
    
    cout << "─────────────────────────────────────────────────────────────" << endl;
    cout << "说明：" << endl;
    cout << "  - 及格率：成绩 ≥ 60分 的学生比例" << endl;
    cout << "  - 优秀率：成绩 ≥ 85分 的学生比例" << endl;
    cout << "═══════════════════════════════════════════════════════════" << endl;
}

// 分数段统计界面
void Menu::scoreRangeStatisticsUI() {
    vector<string> subjects = manager->getSubjects();
    
    cout << endl;
    cout << "请选择统计方式：" << endl;
    cout << "1. 按平均分统计（整体成绩分布）" << endl;
    cout << "2. 按单科成绩统计" << endl;
    cout << "请选择：";
    
    int choice;
    cin >> choice;
    cin.ignore();
    
    string selectedSubject;
    
    if (choice == 2) {
        if (subjects.empty()) {
            cout << "暂无科目信息。" << endl;
            return;
        }
        
        cout << "请选择要统计的科目：" << endl;
        for (size_t i = 0; i < subjects.size(); i++) {
            cout << (i + 1) << ". " << subjects[i] << endl;
        }
        
        int subjChoice;
        cout << "请选择：";
        cin >> subjChoice;
        cin.ignore();
        
        if (subjChoice < 1 || subjChoice > (int)subjects.size()) {
            cout << "无效的选择。" << endl;
            return;
        }
        
        selectedSubject = subjects[subjChoice - 1];
    }
    
    vector<ScoreManager::ScoreRangeStats> stats = manager->getScoreRangeStats(selectedSubject);
    
    cout << endl;
    cout << "═══════════════════════════════════════════════════════════" << endl;
    if (selectedSubject.empty()) {
        cout << "                 平均分分数段分布统计" << endl;
    } else {
        cout << "                 " << selectedSubject << " 分数段分布统计" << endl;
    }
    cout << "═══════════════════════════════════════════════════════════" << endl;
    cout << endl;
    
    int maxCount = 0;
    for (const auto& s : stats) {
        if (s.count > maxCount) maxCount = s.count;
    }
    
    if (maxCount == 0) {
        cout << "  暂无数据。" << endl;
        cout << "═══════════════════════════════════════════════════════════" << endl;
        return;
    }
    
    // 最大显示20个字符
    const int maxBarLength = 20;
    
    for (const auto& s : stats) {
        int barLength = (s.count * maxBarLength + maxCount - 1) / maxCount;  // 向上取整
        
        cout << "  " << setw(8) << s.range << "分: ";
        cout << "█";
        for (int i = 1; i < barLength; i++) {
            cout << "█";
        }
        for (int i = barLength; i < maxBarLength; i++) {
            cout << " ";
        }
        cout << " (" << setw(2) << s.count << "人, " << fixed << setprecision(1) << s.percentage << "%)" << endl;
    }
    
    cout << endl;
    cout << "  分数段说明：" << endl;
    cout << "    90-100分: 优秀 (A)" << endl;
    cout << "    80-89分:  良好 (B)" << endl;
    cout << "    70-79分:  中等 (C)" << endl;
    cout << "    60-69分:  及格 (D)" << endl;
    cout << "    0-59分:   不及格 (F)" << endl;
    cout << "═══════════════════════════════════════════════════════════" << endl;
}

// 显示数据管理菜单
void Menu::showDataMenu() {
    int choice;
    do {
        cout << endl;
        cout << "┌───────────── 数据管理 ────────────┐" << endl;
        cout << "│ 1. 导出数据                        │" << endl;
        cout << "│ 2. 导入数据                        │" << endl;
        cout << "│ 0. 返回主菜单                      │" << endl;
        cout << "└────────────────────────────────────┘" << endl;
        cout << "请选择功能：";
        cin >> choice;
        
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        handleDataMenuChoice(choice);
    } while (choice != 0);
}

// 处理数据管理菜单选择
void Menu::handleDataMenuChoice(int choice) {
    switch (choice) {
        case 1:
            exportDataUI();
            break;
        case 2:
            importDataUI();
            break;
        case 0:
            break;
        default:
            cout << "无效的选择，请重新输入。" << endl;
    }
}

// 选择导出格式
void Menu::selectExportFormat(int& format) {
    cout << endl;
    cout << "请选择导出格式：" << endl;
    cout << "1. 二进制格式 (.dat) - 推荐，完整保存所有数据" << endl;
    cout << "2. 文本格式 (.txt) - 易读，适合查看" << endl;
    cout << "3. CSV格式 (.csv) - 可使用Excel打开" << endl;
    cout << "4. JSON格式 (.json) - 结构化数据" << endl;
    cout << "5. XML格式 (.xml) - 结构化数据" << endl;
    cout << "请选择：";
    cin >> format;
    cin.ignore();
}

// 选择导入格式
void Menu::selectImportFormat(int& format) {
    cout << endl;
    cout << "请选择导入格式：" << endl;
    cout << "1. 二进制格式 (.dat) - 完整恢复所有数据" << endl;
    cout << "2. CSV格式 (.csv) - 从Excel等软件导入" << endl;
    cout << "请选择：";
    cin >> format;
    cin.ignore();
}

// 导出数据界面
void Menu::exportDataUI() {
    int format;
    selectExportFormat(format);
    
    if (format < 1 || format > 5) {
        cout << "无效的格式选择。" << endl;
        return;
    }
    
    string filename;
    cout << "请输入导出文件名（不含扩展名）：";
    getline(cin, filename);
    
    if (filename.empty()) {
        cout << "文件名不能为空。" << endl;
        return;
    }
    
    bool success = false;
    string fullFilename;
    
    switch (format) {
        case 1:
            fullFilename = filename + ".dat";
            success = manager->saveToBinaryFile(fullFilename);
            break;
        case 2:
            fullFilename = filename + ".txt";
            success = manager->saveToTextFile(fullFilename);
            break;
        case 3:
            fullFilename = filename + ".csv";
            success = manager->saveToCSVFile(fullFilename);
            break;
        case 4:
            fullFilename = filename + ".json";
            success = manager->saveToJSONFile(fullFilename);
            break;
        case 5:
            fullFilename = filename + ".xml";
            success = manager->saveToXMLFile(fullFilename);
            break;
    }
    
    if (success) {
        cout << "数据导出成功！文件已保存为: " << fullFilename << endl;
    } else {
        cout << "数据导出失败！" << endl;
    }
}

// 导入数据界面
void Menu::importDataUI() {
    cout << "警告：导入数据将覆盖当前所有数据！" << endl;
    cout << "建议先导出当前数据进行备份。" << endl;
    
    char confirm;
    cout << "确定要继续导入吗？(y/n): ";
    cin >> confirm;
    cin.ignore();
    
    if (confirm != 'y' && confirm != 'Y') {
        cout << "已取消导入。" << endl;
        return;
    }
    
    int format;
    selectImportFormat(format);
    
    if (format < 1 || format > 2) {
        cout << "无效的格式选择。" << endl;
        return;
    }
    
    string filename;
    cout << "请输入导入文件名（含扩展名）：";
    getline(cin, filename);
    
    if (filename.empty()) {
        cout << "文件名不能为空。" << endl;
        return;
    }
    
    bool success = false;
    
    switch (format) {
        case 1:
            success = manager->loadFromBinaryFile(filename);
            break;
        case 2:
            success = manager->loadFromCSVFile(filename);
            break;
    }
    
    if (success) {
        cout << "数据导入成功！" << endl;
        saveData();  // 保存到默认数据文件
    } else {
        cout << "数据导入失败！请检查文件是否存在且格式正确。" << endl;
    }
}

// 运行系统
void Menu::run() {
    int choice;
    do {
        showMainMenu();
        cin >> choice;
        
        // 处理无效输入
        while (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "输入无效，请输入数字：";
            cin >> choice;
        }
        
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        handleMainMenuChoice(choice);
        
    } while (choice != 0);
}
