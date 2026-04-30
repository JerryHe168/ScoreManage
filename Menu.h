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

    // 数据持久化
    void loadData();
    void saveData();

    // 运行系统
    void run();
};

#endif // MENU_H
