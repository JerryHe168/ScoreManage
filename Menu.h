#ifndef MENU_H
#define MENU_H

#include "ScoreManager.h"
#include <string>
#include <memory>

using namespace std;

class Menu {
private:
    unique_ptr<ScoreManager> manager;
    string dataFile;  // 数据文件路径

public:
    // 构造函数和析构函数
    Menu();
    ~Menu();

    // 主菜单
    void showMainMenu();
    void handleMainMenuChoice(int choice);

    // 信息管理子菜单
    void showInfoMenu();
    void handleInfoMenuChoice(int choice);

    // 成绩管理子菜单
    void showScoreMenu();
    void handleScoreMenuChoice(int choice);

    // 成绩排序子菜单
    void showSortMenu();
    void handleSortMenuChoice(int choice);

    // 综合查询子菜单
    void showQueryMenu();
    void handleQueryMenuChoice(int choice);

    // 统计分析子菜单
    void showStatisticsMenu();
    void handleStatisticsMenuChoice(int choice);

    // 数据管理子菜单
    void showDataMenu();
    void handleDataMenuChoice(int choice);

    // 辅助功能
    void addStudentUI();
    void deleteStudentUI();
    void updateStudentUI();
    void viewStudentsUI();
    void searchStudentUI();

    void inputScoreUI();
    void modifyScoreUI();
    void validateScoreUI();
    void subjectSettingsUI();

    void sortByTotalUI();
    void sortByTotalDirect(SortType sortType);
    void sortByAverageUI();
    void sortBySubjectUI();
    void sortByStudentIdUI();

    void queryByClassUI();
    void queryByScoreRangeUI();
    void queryByNameUI();
    void queryByMinScoreUI();

    // 统计分析功能
    void personalStatisticsUI();
    void classStatisticsUI();
    void scoreRangeStatisticsUI();

    // 数据管理功能
    void exportDataUI();
    void importDataUI();
    void selectExportFormat(int& format);
    void selectImportFormat(int& format);

    void displayStudentReport(const Student& student);
    
    // ============================ 公共显示函数（消除重复代码） ============================
    // 显示学生列表表格（带排名）
    void displayStudentList(const vector<Student>& students, const string& title, 
                             bool showRank = true, bool showGrade = false);
    
    // 显示学生列表表格（按单科排序）
    void displayStudentListBySubject(const vector<Student>& students, const string& subject, 
                                       const string& title);
    
    // ============================ 输入验证函数 ============================
    // 验证学号：非空、长度限制、格式验证
    bool validateStudentId(const string& id, string& errorMsg) const;
    
    // 验证姓名：非空、长度限制、格式验证（不允许特殊字符）
    bool validateStudentName(const string& name, string& errorMsg) const;
    
    // 验证班级：非空、长度限制
    bool validateClassName(const string& className, string& errorMsg) const;
    
    // 验证分数范围（0-100）
    bool validateScoreRange(double score, string& errorMsg) const;

    // 数据持久化
    void loadData();
    void saveData();

    // 运行系统
    void run();
};

#endif // MENU_H
