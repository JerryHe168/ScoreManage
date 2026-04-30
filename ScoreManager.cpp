#include "ScoreManager.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

// 构造函数
ScoreManager::ScoreManager() {
    initDefaultSubjects();
}

// 初始化默认科目
void ScoreManager::initDefaultSubjects() {
    subjects = {"语文", "数学", "英语", "物理", "化学", "生物", "历史", "地理", "政治"};
}

// 添加科目
bool ScoreManager::addSubject(const string& subject) {
    if (hasSubject(subject)) {
        return false;  // 科目已存在
    }
    subjects.push_back(subject);
    return true;
}

// 删除科目
bool ScoreManager::removeSubject(const string& subject) {
    auto it = find(subjects.begin(), subjects.end(), subject);
    if (it != subjects.end()) {
        subjects.erase(it);
        // 同时删除所有学生的该科目成绩
        for (auto& student : students) {
            student.removeScore(subject);
        }
        return true;
    }
    return false;
}

// 获取所有科目
vector<string> ScoreManager::getSubjects() const {
    return subjects;
}

// 检查是否有某科目
bool ScoreManager::hasSubject(const string& subject) const {
    return find(subjects.begin(), subjects.end(), subject) != subjects.end();
}

// 添加学生
bool ScoreManager::addStudent(const Student& student) {
    // 检查学号是否已存在
    for (const auto& s : students) {
        if (s.getStudentId() == student.getStudentId()) {
            return false;  // 学号已存在
        }
    }
    students.push_back(student);
    return true;
}

// 删除学生
bool ScoreManager::removeStudent(const string& studentId) {
    for (auto it = students.begin(); it != students.end(); ++it) {
        if (it->getStudentId() == studentId) {
            students.erase(it);
            return true;
        }
    }
    return false;
}

// 更新学生信息
bool ScoreManager::updateStudent(const Student& student) {
    for (auto& s : students) {
        if (s.getStudentId() == student.getStudentId()) {
            s = student;
            return true;
        }
    }
    return false;
}

// 根据学号查找学生
Student* ScoreManager::findStudentById(const string& studentId) {
    for (auto& student : students) {
        if (student.getStudentId() == studentId) {
            return &student;
        }
    }
    return nullptr;
}

// 获取所有学生
vector<Student> ScoreManager::getAllStudents() const {
    return students;
}

// 获取学生数量
int ScoreManager::getStudentCount() const {
    return students.size();
}

// 设置学生成绩
bool ScoreManager::setStudentScore(const string& studentId, const string& subject, double score) {
    if (!validateScore(score)) {
        return false;  // 分数无效
    }
    
    Student* student = findStudentById(studentId);
    if (student == nullptr) {
        return false;  // 学生不存在
    }
    
    return student->setScore(subject, score);
}

// 验证分数
bool ScoreManager::validateScore(double score) const {
    return score >= 0 && score <= 100;
}

// 排序学生
vector<Student> ScoreManager::sortStudents(SortType sortType, const string& subject) const {
    vector<Student> sortedStudents = students;
    
    switch (sortType) {
        case SORT_BY_TOTAL_DESC:
            sort(sortedStudents.begin(), sortedStudents.end(), 
                [](const Student& a, const Student& b) {
                    return a.getTotalScore() > b.getTotalScore();
                });
            break;
            
        case SORT_BY_TOTAL_ASC:
            sort(sortedStudents.begin(), sortedStudents.end(), 
                [](const Student& a, const Student& b) {
                    return a.getTotalScore() < b.getTotalScore();
                });
            break;
            
        case SORT_BY_AVERAGE:
            sort(sortedStudents.begin(), sortedStudents.end(), 
                [](const Student& a, const Student& b) {
                    return a.getAverageScore() > b.getAverageScore();
                });
            break;
            
        case SORT_BY_SUBJECT:
            if (!subject.empty()) {
                sort(sortedStudents.begin(), sortedStudents.end(), 
                    [&](const Student& a, const Student& b) {
                        double scoreA = a.hasScore(subject) ? a.getScore(subject) : -1;
                        double scoreB = b.hasScore(subject) ? b.getScore(subject) : -1;
                        return scoreA > scoreB;
                    });
            }
            break;
            
        case SORT_BY_STUDENT_ID:
            sort(sortedStudents.begin(), sortedStudents.end(), 
                [](const Student& a, const Student& b) {
                    return a.getStudentId() < b.getStudentId();
                });
            break;
    }
    
    return sortedStudents;
}

// 查询学生
vector<Student> ScoreManager::queryStudents(QueryType queryType, const string& param1, 
                                                double param2, double param3) const {
    vector<Student> result;
    
    for (const auto& student : students) {
        bool match = false;
        
        switch (queryType) {
            case QUERY_BY_CLASS:
                match = (student.getClassName() == param1);
                break;
                
            case QUERY_BY_SCORE_RANGE:
                // param1: 科目名, param2: 最低分, param3: 最高分
                if (student.hasScore(param1)) {
                    double score = student.getScore(param1);
                    match = (score >= param2 && score <= param3);
                }
                break;
                
            case QUERY_BY_NAME:
                // 模糊查询
                if (student.getName().find(param1) != string::npos) {
                    match = true;
                }
                break;
                
            case QUERY_BY_MIN_SCORE:
                // param1: 科目名, param2: 最低分
                if (student.hasScore(param1)) {
                    double score = student.getScore(param1);
                    match = (score >= param2);
                }
                break;
        }
        
        if (match) {
            result.push_back(student);
        }
    }
    
    return result;
}

// 获取班级排名
int ScoreManager::getClassRank(const Student& student) const {
    string className = student.getClassName();
    double average = student.getAverageScore();
    
    int rank = 1;
    for (const auto& s : students) {
        if (s.getClassName() == className && s.getAverageScore() > average) {
            rank++;
        }
    }
    
    return rank;
}

// 保存到文件
bool ScoreManager::saveToFile(const string& filename) const {
    ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    // 保存科目列表
    file << subjects.size() << endl;
    for (const auto& subject : subjects) {
        file << subject << endl;
    }
    
    // 保存学生数量
    file << students.size() << endl;
    
    // 保存每个学生的信息和成绩
    for (const auto& student : students) {
        file << student.getStudentId() << endl;
        file << student.getName() << endl;
        file << student.getClassName() << endl;
        
        // 保存成绩
        map<string, double> scores = student.getAllScores();
        file << scores.size() << endl;
        for (const auto& pair : scores) {
            file << pair.first << " " << pair.second << endl;
        }
    }
    
    file.close();
    return true;
}

// 从文件加载
bool ScoreManager::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    // 使用临时变量存储读取的数据，只有读取成功后才替换现有数据
    vector<string> tempSubjects;
    vector<Student> tempStudents;
    
    // 读取科目列表
    int subjectCount;
    if (!(file >> subjectCount)) {
        file.close();
        return false;
    }
    file.ignore();  // 忽略换行符
    
    for (int i = 0; i < subjectCount; i++) {
        string subject;
        if (!getline(file, subject)) {
            file.close();
            return false;
        }
        tempSubjects.push_back(subject);
    }
    
    // 读取学生数量
    int studentCount;
    if (!(file >> studentCount)) {
        file.close();
        return false;
    }
    file.ignore();
    
    // 读取每个学生的信息和成绩
    for (int i = 0; i < studentCount; i++) {
        string studentId, name, className;
        if (!getline(file, studentId) || !getline(file, name) || !getline(file, className)) {
            file.close();
            return false;
        }
        
        Student student(studentId, name, className);
        
        // 读取成绩
        int scoreCount;
        if (!(file >> scoreCount)) {
            file.close();
            return false;
        }
        file.ignore();
        
        for (int j = 0; j < scoreCount; j++) {
            string line;
            if (!getline(file, line)) {
                file.close();
                return false;
            }
            istringstream iss(line);
            string subject;
            double score;
            if (!(iss >> subject >> score)) {
                file.close();
                return false;
            }
            student.setScore(subject, score);
        }
        
        tempStudents.push_back(student);
    }
    
    file.close();
    
    // 只有读取完全成功后，才替换现有数据
    students.swap(tempStudents);
    subjects.swap(tempSubjects);
    
    return true;
}

// 获取班级平均分
double ScoreManager::getClassAverage(const string& className) const {
    double total = 0.0;
    int count = 0;
    
    for (const auto& student : students) {
        if (student.getClassName() == className && student.getSubjectCount() > 0) {
            total += student.getAverageScore();
            count++;
        }
    }
    
    return count > 0 ? total / count : 0.0;
}

// 获取科目平均分
double ScoreManager::getSubjectAverage(const string& subject) const {
    double total = 0.0;
    int count = 0;
    
    for (const auto& student : students) {
        if (student.hasScore(subject)) {
            total += student.getScore(subject);
            count++;
        }
    }
    
    return count > 0 ? total / count : 0.0;
}

// 获取及格人数
int ScoreManager::getPassCount(const string& subject) const {
    int count = 0;
    
    for (const auto& student : students) {
        if (student.hasScore(subject) && student.getScore(subject) >= 60) {
            count++;
        }
    }
    
    return count;
}

// 获取优秀人数
int ScoreManager::getExcellentCount(const string& subject) const {
    int count = 0;
    
    for (const auto& student : students) {
        if (student.hasScore(subject) && student.getScore(subject) >= 90) {
            count++;
        }
    }
    
    return count;
}
