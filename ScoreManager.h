#ifndef SCOREMANAGER_H
#define SCOREMANAGER_H

#include "Student.h"
#include <vector>
#include <string>

using namespace std;

// 排序类型枚举
enum SortType {
    SORT_BY_TOTAL_DESC,      // 按总分降序
    SORT_BY_TOTAL_ASC,       // 按总分升序
    SORT_BY_AVERAGE,         // 按平均分排序
    SORT_BY_SUBJECT,         // 按单科成绩排序
    SORT_BY_STUDENT_ID       // 按学号排序
};

// 查询类型枚举
enum QueryType {
    QUERY_BY_CLASS,          // 按班级查询
    QUERY_BY_SCORE_RANGE,    // 按分数段查询
    QUERY_BY_NAME,           // 按姓名模糊查询
    QUERY_BY_MIN_SCORE       // 按成绩范围查询（≥某分数）
};

class ScoreManager {
private:
    vector<Student> students;
    vector<string> subjects;  // 支持的科目列表

public:
    // 构造函数
    ScoreManager();

    // 科目管理
    void initDefaultSubjects();  // 初始化默认科目（语数英数理化史地生）
    bool addSubject(const string& subject);
    bool removeSubject(const string& subject);
    vector<string> getSubjects() const;
    bool hasSubject(const string& subject) const;

    // 学生信息管理
    bool addStudent(const Student& student);
    bool removeStudent(const string& studentId);
    bool updateStudent(const Student& student);
    Student* findStudentById(const string& studentId);
    vector<Student> getAllStudents() const;
    int getStudentCount() const;

    // 成绩管理
    bool setStudentScore(const string& studentId, const string& subject, double score);
    bool validateScore(double score) const;  // 验证分数是否有效（0-100）

    // 排序功能
    vector<Student> sortStudents(SortType sortType, const string& subject = "") const;

    // 查询功能
    vector<Student> queryStudents(QueryType queryType, const string& param1 = "", 
                                    double param2 = 0, double param3 = 0) const;

    // 排名计算
    int getClassRank(const Student& student) const;

    // 数据持久化
    bool saveToFile(const string& filename) const;
    bool loadFromFile(const string& filename);

    // 统计功能
    double getClassAverage(const string& className) const;
    double getSubjectAverage(const string& subject) const;
    int getPassCount(const string& subject) const;
    int getExcellentCount(const string& subject) const;
    
    // 班级统计报告结构
    struct SubjectStats {
        string subject;
        double average;
        double maxScore;
        string maxStudentId;
        string maxStudentName;
        double minScore;
        string minStudentId;
        string minStudentName;
        int passCount;
        int excellentCount;
        double passRate;
        double excellentRate;
    };
    
    struct ClassStatistics {
        string className;
        int totalStudents;
        vector<SubjectStats> subjectStats;
        double totalMaxScore;
        string totalMaxStudentId;
        string totalMaxStudentName;
        double totalMinScore;
        string totalMinStudentId;
        string totalMinStudentName;
        double overallAverage;
    };
    
    // 分数段统计结构
    struct ScoreRangeStats {
        string range;
        int count;
        double percentage;
    };
    
    // 班级统计
    ClassStatistics getClassStatistics(const string& className) const;
    
    // 分数段统计
    vector<ScoreRangeStats> getScoreRangeStats(const string& subject = "") const;
    
    // 所有班级列表
    vector<string> getAllClasses() const;
    
    // 多格式数据持久化
    bool saveToBinaryFile(const string& filename) const;
    bool loadFromBinaryFile(const string& filename);
    
    bool saveToTextFile(const string& filename) const;
    bool loadFromTextFile(const string& filename);
    
    bool saveToCSVFile(const string& filename) const;
    bool loadFromCSVFile(const string& filename);
    
    bool saveToJSONFile(const string& filename) const;
    bool loadFromJSONFile(const string& filename);
    
    bool saveToXMLFile(const string& filename) const;
    bool loadFromXMLFile(const string& filename);
};

#endif // SCOREMANAGER_H
