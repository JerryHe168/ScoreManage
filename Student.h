#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <vector>
#include <map>

using namespace std;

class Student {
private:
    string studentId;
    string name;
    string className;
    map<string, double> scores;  // 科目名 -> 分数
    bool isComplete;  // 成绩是否完整录入

public:
    // 构造函数
    Student();
    Student(const string& id, const string& n, const string& c);

    // 获取基本信息（返回const引用，避免字符串拷贝）
    const string& getStudentId() const;
    const string& getName() const;
    const string& getClassName() const;

    // 设置基本信息
    void setStudentId(const string& id);
    void setName(const string& n);
    void setClassName(const string& c);

    // 成绩管理
    bool setScore(const string& subject, double score);
    double getScore(const string& subject) const;
    bool hasScore(const string& subject) const;
    void removeScore(const string& subject);
    map<string, double> getAllScores() const;

    // 计算相关
    double getTotalScore() const;
    double getAverageScore() const;
    int getSubjectCount() const;

    // 状态管理
    bool getIsComplete() const;
    void setIsComplete(bool complete);

    // 等级评定
    char getGrade() const;

    // 辅助函数
    void display() const;
};

#endif // STUDENT_H
