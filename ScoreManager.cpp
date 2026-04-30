#include "ScoreManager.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

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

// 获取所有班级列表
vector<string> ScoreManager::getAllClasses() const {
    vector<string> classes;
    for (const auto& student : students) {
        bool exists = false;
        for (const auto& cls : classes) {
            if (cls == student.getClassName()) {
                exists = true;
                break;
            }
        }
        if (!exists) {
            classes.push_back(student.getClassName());
        }
    }
    return classes;
}

// 获取班级统计报告
ScoreManager::ClassStatistics ScoreManager::getClassStatistics(const string& className) const {
    ClassStatistics stats;
    stats.className = className;
    stats.totalStudents = 0;
    stats.totalMaxScore = -1;
    stats.totalMinScore = 101;
    stats.overallAverage = 0.0;
    
    double totalScoreSum = 0.0;
    int validStudentCount = 0;
    
    // 统计每个学生
    for (const auto& student : students) {
        if (student.getClassName() != className) continue;
        
        stats.totalStudents++;
        
        // 统计总分
        double totalScore = student.getTotalScore();
        if (student.getSubjectCount() > 0) {
            totalScoreSum += totalScore;
            validStudentCount++;
            
            if (totalScore > stats.totalMaxScore) {
                stats.totalMaxScore = totalScore;
                stats.totalMaxStudentId = student.getStudentId();
            }
            
            if (totalScore < stats.totalMinScore) {
                stats.totalMinScore = totalScore;
                stats.totalMinStudentId = student.getStudentId();
            }
        }
    }
    
    if (validStudentCount > 0) {
        stats.overallAverage = totalScoreSum / validStudentCount;
    }
    
    // 统计每个科目
    for (const auto& subject : subjects) {
        SubjectStats subjectStats;
        subjectStats.subject = subject;
        subjectStats.average = 0.0;
        subjectStats.maxScore = -1;
        subjectStats.minScore = 101;
        subjectStats.passCount = 0;
        subjectStats.excellentCount = 0;
        
        double scoreSum = 0.0;
        int scoreCount = 0;
        
        for (const auto& student : students) {
            if (student.getClassName() != className) continue;
            if (!student.hasScore(subject)) continue;
            
            double score = student.getScore(subject);
            scoreSum += score;
            scoreCount++;
            
            if (score > subjectStats.maxScore) {
                subjectStats.maxScore = score;
                subjectStats.maxStudentId = student.getStudentId();
            }
            
            if (score < subjectStats.minScore) {
                subjectStats.minScore = score;
                subjectStats.minStudentId = student.getStudentId();
            }
            
            if (score >= 60) {
                subjectStats.passCount++;
            }
            if (score >= 85) {
                subjectStats.excellentCount++;
            }
        }
        
        if (scoreCount > 0) {
            subjectStats.average = scoreSum / scoreCount;
            subjectStats.passRate = static_cast<double>(subjectStats.passCount) / scoreCount * 100;
            subjectStats.excellentRate = static_cast<double>(subjectStats.excellentCount) / scoreCount * 100;
        }
        
        stats.subjectStats.push_back(subjectStats);
    }
    
    return stats;
}

// 获取分数段统计
vector<ScoreManager::ScoreRangeStats> ScoreManager::getScoreRangeStats(const string& subject) const {
    vector<ScoreRangeStats> stats;
    stats = {
        {"90-100", 0, 0.0},
        {"80-89", 0, 0.0},
        {"70-79", 0, 0.0},
        {"60-69", 0, 0.0},
        {"0-59", 0, 0.0}
    };
    
    int totalCount = 0;
    
    for (const auto& student : students) {
        if (subject.empty()) {
            // 按总分统计
            if (student.getSubjectCount() > 0) {
                double avgScore = student.getAverageScore();
                totalCount++;
                if (avgScore >= 90) stats[0].count++;
                else if (avgScore >= 80) stats[1].count++;
                else if (avgScore >= 70) stats[2].count++;
                else if (avgScore >= 60) stats[3].count++;
                else stats[4].count++;
            }
        } else {
            // 按单科统计
            if (student.hasScore(subject)) {
                double score = student.getScore(subject);
                totalCount++;
                if (score >= 90) stats[0].count++;
                else if (score >= 80) stats[1].count++;
                else if (score >= 70) stats[2].count++;
                else if (score >= 60) stats[3].count++;
                else stats[4].count++;
            }
        }
    }
    
    if (totalCount > 0) {
        for (auto& s : stats) {
            s.percentage = static_cast<double>(s.count) / totalCount * 100;
        }
    }
    
    return stats;
}

// 保存到二进制文件
bool ScoreManager::saveToBinaryFile(const string& filename) const {
    ofstream file(filename, ios::binary);
    if (!file.is_open()) {
        return false;
    }
    
    // 写入科目数量
    size_t subjectCount = subjects.size();
    file.write(reinterpret_cast<const char*>(&subjectCount), sizeof(subjectCount));
    
    // 写入每个科目
    for (const auto& subject : subjects) {
        size_t len = subject.length();
        file.write(reinterpret_cast<const char*>(&len), sizeof(len));
        file.write(subject.c_str(), len);
    }
    
    // 写入学生数量
    size_t studentCount = students.size();
    file.write(reinterpret_cast<const char*>(&studentCount), sizeof(studentCount));
    
    // 写入每个学生
    for (const auto& student : students) {
        // 学号
        string id = student.getStudentId();
        size_t len = id.length();
        file.write(reinterpret_cast<const char*>(&len), sizeof(len));
        file.write(id.c_str(), len);
        
        // 姓名
        string name = student.getName();
        len = name.length();
        file.write(reinterpret_cast<const char*>(&len), sizeof(len));
        file.write(name.c_str(), len);
        
        // 班级
        string className = student.getClassName();
        len = className.length();
        file.write(reinterpret_cast<const char*>(&len), sizeof(len));
        file.write(className.c_str(), len);
        
        // 成绩数量
        map<string, double> scores = student.getAllScores();
        size_t scoreCount = scores.size();
        file.write(reinterpret_cast<const char*>(&scoreCount), sizeof(scoreCount));
        
        // 每个成绩
        for (const auto& pair : scores) {
            // 科目名
            len = pair.first.length();
            file.write(reinterpret_cast<const char*>(&len), sizeof(len));
            file.write(pair.first.c_str(), len);
            
            // 分数
            double score = pair.second;
            file.write(reinterpret_cast<const char*>(&score), sizeof(score));
        }
    }
    
    file.close();
    return true;
}

// 从二进制文件加载
bool ScoreManager::loadFromBinaryFile(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        return false;
    }
    
    vector<string> tempSubjects;
    vector<Student> tempStudents;
    
    // 读取科目数量
    size_t subjectCount;
    if (!file.read(reinterpret_cast<char*>(&subjectCount), sizeof(subjectCount))) {
        file.close();
        return false;
    }
    
    // 读取每个科目
    for (size_t i = 0; i < subjectCount; i++) {
        size_t len;
        if (!file.read(reinterpret_cast<char*>(&len), sizeof(len))) {
            file.close();
            return false;
        }
        string subject(len, '\0');
        if (!file.read(&subject[0], len)) {
            file.close();
            return false;
        }
        tempSubjects.push_back(subject);
    }
    
    // 读取学生数量
    size_t studentCount;
    if (!file.read(reinterpret_cast<char*>(&studentCount), sizeof(studentCount))) {
        file.close();
        return false;
    }
    
    // 读取每个学生
    for (size_t i = 0; i < studentCount; i++) {
        // 学号
        size_t len;
        if (!file.read(reinterpret_cast<char*>(&len), sizeof(len))) {
            file.close();
            return false;
        }
        string id(len, '\0');
        if (!file.read(&id[0], len)) {
            file.close();
            return false;
        }
        
        // 姓名
        if (!file.read(reinterpret_cast<char*>(&len), sizeof(len))) {
            file.close();
            return false;
        }
        string name(len, '\0');
        if (!file.read(&name[0], len)) {
            file.close();
            return false;
        }
        
        // 班级
        if (!file.read(reinterpret_cast<char*>(&len), sizeof(len))) {
            file.close();
            return false;
        }
        string className(len, '\0');
        if (!file.read(&className[0], len)) {
            file.close();
            return false;
        }
        
        Student student(id, name, className);
        
        // 成绩数量
        size_t scoreCount;
        if (!file.read(reinterpret_cast<char*>(&scoreCount), sizeof(scoreCount))) {
            file.close();
            return false;
        }
        
        // 每个成绩
        for (size_t j = 0; j < scoreCount; j++) {
            // 科目名
            if (!file.read(reinterpret_cast<char*>(&len), sizeof(len))) {
                file.close();
                return false;
            }
            string subject(len, '\0');
            if (!file.read(&subject[0], len)) {
                file.close();
                return false;
            }
            
            // 分数
            double score;
            if (!file.read(reinterpret_cast<char*>(&score), sizeof(score))) {
                file.close();
                return false;
            }
            
            student.setScore(subject, score);
        }
        
        tempStudents.push_back(student);
    }
    
    file.close();
    
    students.swap(tempStudents);
    subjects.swap(tempSubjects);
    
    return true;
}

// 保存到文本文件
bool ScoreManager::saveToTextFile(const string& filename) const {
    ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    file << "=== 学生成绩管理系统数据文件 ===" << endl;
    file << "版本: 2.0" << endl;
    file << endl;
    
    // 科目列表
    file << "=== 科目列表 ===" << endl;
    file << "科目数量: " << subjects.size() << endl;
    for (size_t i = 0; i < subjects.size(); i++) {
        file << (i + 1) << ". " << subjects[i] << endl;
    }
    file << endl;
    
    // 学生列表
    file << "=== 学生列表 ===" << endl;
    file << "学生数量: " << students.size() << endl;
    file << endl;
    
    for (size_t i = 0; i < students.size(); i++) {
        file << "--- 学生 " << (i + 1) << " ---" << endl;
        file << "学号: " << students[i].getStudentId() << endl;
        file << "姓名: " << students[i].getName() << endl;
        file << "班级: " << students[i].getClassName() << endl;
        
        map<string, double> scores = students[i].getAllScores();
        file << "成绩数量: " << scores.size() << endl;
        
        for (const auto& pair : scores) {
            file << pair.first << ": " << fixed << setprecision(1) << pair.second << endl;
        }
        
        if (students[i].getSubjectCount() > 0) {
            file << "总分: " << fixed << setprecision(1) << students[i].getTotalScore() << endl;
            file << "平均分: " << fixed << setprecision(1) << students[i].getAverageScore() << endl;
            file << "等级: " << students[i].getGrade() << endl;
        }
        file << endl;
    }
    
    file.close();
    return true;
}

// 从文本文件加载（简单解析，仅读取关键数据）
bool ScoreManager::loadFromTextFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    vector<string> tempSubjects;
    vector<Student> tempStudents;
    
    string line;
    bool inSubjectSection = false;
    bool inStudentSection = false;
    
    while (getline(file, line)) {
        if (line == "=== 科目列表 ===") {
            inSubjectSection = true;
            inStudentSection = false;
            continue;
        }
        
        if (line == "=== 学生列表 ===") {
            inSubjectSection = false;
            inStudentSection = true;
            continue;
        }
        
        if (inSubjectSection) {
            if (line.substr(0, 5) == "科目数量:") continue;
            // 解析 "1. 语文" 格式
            size_t dotPos = line.find(". ");
            if (dotPos != string::npos) {
                string subject = line.substr(dotPos + 2);
                tempSubjects.push_back(subject);
            }
        }
        
        if (inStudentSection) {
            // 简单格式解析，这里使用原有格式更好
            // 为了简化，我们复用原有文本格式的saveToFile/loadFromFile
        }
    }
    
    file.close();
    
    // 文本文件加载比较复杂，这里我们提示用户使用标准格式
    // 实际上文本格式主要用于导出查看，导入建议使用dat或csv
    return tempSubjects.empty() && tempStudents.empty() ? false : true;
}

// 保存到CSV文件
bool ScoreManager::saveToCSVFile(const string& filename) const {
    ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    // 写入表头
    file << "学号,姓名,班级";
    for (const auto& subject : subjects) {
        file << "," << subject;
    }
    file << ",总分,平均分,等级" << endl;
    
    // 写入每个学生
    for (const auto& student : students) {
        file << student.getStudentId() << ","
             << student.getName() << ","
             << student.getClassName();
        
        for (const auto& subject : subjects) {
            if (student.hasScore(subject)) {
                file << "," << fixed << setprecision(1) << student.getScore(subject);
            } else {
                file << ",";
            }
        }
        
        if (student.getSubjectCount() > 0) {
            file << "," << fixed << setprecision(1) << student.getTotalScore()
                 << "," << fixed << setprecision(1) << student.getAverageScore()
                 << "," << student.getGrade();
        } else {
            file << ",,,";
        }
        file << endl;
    }
    
    file.close();
    return true;
}

// 从CSV文件加载
bool ScoreManager::loadFromCSVFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    vector<string> tempSubjects;
    vector<Student> tempStudents;
    
    string line;
    
    // 读取第一行（表头）
    if (!getline(file, line)) {
        file.close();
        return false;
    }
    
    // 解析表头
    vector<string> headers;
    istringstream headerStream(line);
    string cell;
    
    while (getline(headerStream, cell, ',')) {
        headers.push_back(cell);
    }
    
    // 提取科目（从第4列开始，到"总分"之前）
    for (size_t i = 3; i < headers.size(); i++) {
        if (headers[i] == "总分") break;
        tempSubjects.push_back(headers[i]);
    }
    
    // 读取数据行
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        vector<string> cells;
        istringstream lineStream(line);
        
        while (getline(lineStream, cell, ',')) {
            cells.push_back(cell);
        }
        
        if (cells.size() < 3) continue;
        
        string id = cells[0];
        string name = cells[1];
        string className = cells[2];
        
        Student student(id, name, className);
        
        // 读取成绩
        for (size_t i = 0; i < tempSubjects.size(); i++) {
            size_t colIndex = 3 + i;
            if (colIndex < cells.size() && !cells[colIndex].empty()) {
                try {
                    double score = stod(cells[colIndex]);
                    student.setScore(tempSubjects[i], score);
                } catch (...) {
                    // 忽略解析错误
                }
            }
        }
        
        tempStudents.push_back(student);
    }
    
    file.close();
    
    if (tempSubjects.empty() && tempStudents.empty()) {
        return false;
    }
    
    // 如果文件中没有科目，使用默认科目
    if (tempSubjects.empty()) {
        tempSubjects = {"语文", "数学", "英语", "物理", "化学", "生物", "历史", "地理", "政治"};
    }
    
    students.swap(tempStudents);
    subjects.swap(tempSubjects);
    
    return true;
}

// 保存到JSON文件（简单实现）
bool ScoreManager::saveToJSONFile(const string& filename) const {
    ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    file << "{" << endl;
    file << "  \"version\": \"2.0\"," << endl;
    file << "  \"subjects\": [";
    
    // 科目
    for (size_t i = 0; i < subjects.size(); i++) {
        file << "\"" << subjects[i] << "\"";
        if (i < subjects.size() - 1) file << ", ";
    }
    file << "]," << endl;
    
    // 学生
    file << "  \"students\": [" << endl;
    
    for (size_t i = 0; i < students.size(); i++) {
        const auto& student = students[i];
        file << "    {" << endl;
        file << "      \"id\": \"" << student.getStudentId() << "\"," << endl;
        file << "      \"name\": \"" << student.getName() << "\"," << endl;
        file << "      \"class\": \"" << student.getClassName() << "\"," << endl;
        
        map<string, double> scores = student.getAllScores();
        file << "      \"scores\": {" << endl;
        
        size_t j = 0;
        for (const auto& pair : scores) {
            file << "        \"" << pair.first << "\": " << fixed << setprecision(1) << pair.second;
            if (j < scores.size() - 1) file << ",";
            file << endl;
            j++;
        }
        file << "      }," << endl;
        
        if (student.getSubjectCount() > 0) {
            file << "      \"total\": " << fixed << setprecision(1) << student.getTotalScore() << "," << endl;
            file << "      \"average\": " << fixed << setprecision(1) << student.getAverageScore() << "," << endl;
            file << "      \"grade\": \"" << student.getGrade() << "\"" << endl;
        } else {
            file << "      \"total\": 0," << endl;
            file << "      \"average\": 0," << endl;
            file << "      \"grade\": \"-\"" << endl;
        }
        
        file << "    }";
        if (i < students.size() - 1) file << ",";
        file << endl;
    }
    
    file << "  ]" << endl;
    file << "}" << endl;
    
    file.close();
    return true;
}

// 从JSON文件加载（简单实现）
bool ScoreManager::loadFromJSONFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();
    
    // 简单解析（仅处理基本格式）
    // 实际项目中应该使用JSON库，但为了保持无依赖，使用简单解析
    
    vector<string> tempSubjects;
    vector<Student> tempStudents;
    
    // 解析subjects数组
    size_t subjStart = content.find("\"subjects\":");
    if (subjStart != string::npos) {
        size_t arrStart = content.find("[", subjStart);
        size_t arrEnd = content.find("]", arrStart);
        
        string subjArr = content.substr(arrStart + 1, arrEnd - arrStart - 1);
        istringstream iss(subjArr);
        string item;
        
        while (getline(iss, item, ',')) {
            size_t quote1 = item.find("\"");
            size_t quote2 = item.find("\"", quote1 + 1);
            if (quote1 != string::npos && quote2 != string::npos) {
                string subject = item.substr(quote1 + 1, quote2 - quote1 - 1);
                tempSubjects.push_back(subject);
            }
        }
    }
    
    // 解析students数组（简化版）
    // 实际项目建议使用JSON库
    // 这里为了简化，不实现复杂的JSON解析
    // 用户可以通过CSV或二进制格式导入
    
    if (tempSubjects.empty() && tempStudents.empty()) {
        return false;
    }
    
    subjects.swap(tempSubjects);
    students.swap(tempStudents);
    
    return true;
}

// 保存到XML文件
bool ScoreManager::saveToXMLFile(const string& filename) const {
    ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;
    file << "<ScoreManagement version=\"2.0\">" << endl;
    
    // 科目
    file << "  <Subjects>" << endl;
    for (const auto& subject : subjects) {
        file << "    <Subject name=\"" << subject << "\"/>" << endl;
    }
    file << "  </Subjects>" << endl;
    
    // 学生
    file << "  <Students>" << endl;
    for (const auto& student : students) {
        file << "    <Student id=\"" << student.getStudentId() << "\">" << endl;
        file << "      <Name>" << student.getName() << "</Name>" << endl;
        file << "      <Class>" << student.getClassName() << "</Class>" << endl;
        
        map<string, double> scores = student.getAllScores();
        if (!scores.empty()) {
            file << "      <Scores>" << endl;
            for (const auto& pair : scores) {
                file << "        <Score subject=\"" << pair.first << "\">" 
                     << fixed << setprecision(1) << pair.second << "</Score>" << endl;
            }
            file << "      </Scores>" << endl;
            
            file << "      <Statistics>" << endl;
            file << "        <Total>" << fixed << setprecision(1) << student.getTotalScore() << "</Total>" << endl;
            file << "        <Average>" << fixed << setprecision(1) << student.getAverageScore() << "</Average>" << endl;
            file << "        <Grade>" << student.getGrade() << "</Grade>" << endl;
            file << "      </Statistics>" << endl;
        }
        
        file << "    </Student>" << endl;
    }
    file << "  </Students>" << endl;
    
    file << "</ScoreManagement>" << endl;
    
    file.close();
    return true;
}

// 从XML文件加载（简化版）
bool ScoreManager::loadFromXMLFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    // XML解析较复杂，这里只提供基本的导出功能
    // 导入建议使用CSV或二进制格式
    file.close();
    return false;
}
