#include "Student.h"
#include <iostream>
#include <iomanip>

using namespace std;

// 默认构造函数
Student::Student() : isComplete(false) {}

// 带参数构造函数
Student::Student(const string& id, const string& n, const string& c)
    : studentId(id), name(n), className(c), isComplete(false) {}

// 获取学生ID（返回const引用，避免字符串拷贝）
const string& Student::getStudentId() const {
    return studentId;
}

// 获取学生姓名（返回const引用，避免字符串拷贝）
const string& Student::getName() const {
    return name;
}

// 获取班级（返回const引用，避免字符串拷贝）
const string& Student::getClassName() const {
    return className;
}

// 设置学生ID
void Student::setStudentId(const string& id) {
    studentId = id;
}

// 设置学生姓名
void Student::setName(const string& n) {
    name = n;
}

// 设置班级
void Student::setClassName(const string& c) {
    className = c;
}

// 设置成绩
bool Student::setScore(const string& subject, double score) {
    if (score < 0 || score > 100) {
        return false;  // 分数无效
    }
    scores[subject] = score;
    return true;
}

// 获取成绩
double Student::getScore(const string& subject) const {
    auto it = scores.find(subject);
    if (it != scores.end()) {
        return it->second;
    }
    return -1.0;  // 表示没有该科目成绩
}

// 检查是否有某科目成绩
bool Student::hasScore(const string& subject) const {
    return scores.find(subject) != scores.end();
}

// 删除某科目成绩
void Student::removeScore(const string& subject) {
    scores.erase(subject);
}

// 获取所有成绩
map<string, double> Student::getAllScores() const {
    return scores;
}

// 获取总分
double Student::getTotalScore() const {
    double total = 0.0;
    for (const auto& pair : scores) {
        total += pair.second;
    }
    return total;
}

// 获取平均分
double Student::getAverageScore() const {
    if (scores.empty()) {
        return 0.0;
    }
    return getTotalScore() / scores.size();
}

// 获取科目数量
int Student::getSubjectCount() const {
    return scores.size();
}

// 获取成绩是否完整
bool Student::getIsComplete() const {
    return isComplete;
}

// 设置成绩是否完整
void Student::setIsComplete(bool complete) {
    isComplete = complete;
}

// 获取等级评定
char Student::getGrade() const {
    double average = getAverageScore();
    if (average >= 90) return 'A';
    else if (average >= 80) return 'B';
    else if (average >= 70) return 'C';
    else if (average >= 60) return 'D';
    else return 'F';
}

// 显示学生信息
void Student::display() const {
    cout << "学号: " << studentId << endl;
    cout << "姓名: " << name << endl;
    cout << "班级: " << className << endl;
    
    if (!scores.empty()) {
        cout << "成绩:" << endl;
        for (const auto& pair : scores) {
            cout << "  " << pair.first << ": " << fixed << setprecision(1) << pair.second << endl;
        }
        cout << "总分: " << fixed << setprecision(1) << getTotalScore() << endl;
        cout << "平均分: " << fixed << setprecision(1) << getAverageScore() << endl;
        cout << "等级: " << getGrade() << endl;
    } else {
        cout << "暂无成绩信息" << endl;
    }
}
