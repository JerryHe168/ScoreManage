#include "ScoreManager.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <unordered_set>
#include <cmath>

using namespace std;

// ============================ 缓存机制实现 ============================

// 使缓存失效 - 当学生数据变更时调用
void ScoreManager::invalidateCache() const {
    cacheValid = false;
    classesCacheValid = false;  // 班级列表缓存也失效
    sortCacheValid = false;     // 排序缓存也失效
    classStatsCache.clear();
    studentIdIndex.clear();
    classesCache.clear();  // 清空班级列表缓存
    sortCache.clear();     // 清空排序缓存
}

// 构建学生ID索引 - O(n)一次构建，后续O(1)查找
void ScoreManager::buildStudentIdIndex() const {
    if (cacheValid && !studentIdIndex.empty()) {
        return;  // 索引已有效
    }
    
    studentIdIndex.clear();
    for (size_t i = 0; i < students.size(); ++i) {
        studentIdIndex[students[i].getStudentId()] = i;
    }
    cacheValid = true;
}

// 构造函数
ScoreManager::ScoreManager() 
    : cacheValid(false), classesCacheValid(false), sortCacheValid(false) {
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
    invalidateCache();  // 数据变更，使缓存失效
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
        invalidateCache();  // 数据变更，使缓存失效
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
    // 使用索引检查学号是否已存在（先构建索引）
    buildStudentIdIndex();
    
    if (studentIdIndex.find(student.getStudentId()) != studentIdIndex.end()) {
        return false;  // 学号已存在
    }
    
    students.push_back(student);
    invalidateCache();  // 数据变更，使缓存失效
    return true;
}

// 删除学生
bool ScoreManager::removeStudent(const string& studentId) {
    for (auto it = students.begin(); it != students.end(); ++it) {
        if (it->getStudentId() == studentId) {
            students.erase(it);
            invalidateCache();  // 数据变更，使缓存失效
            return true;
        }
    }
    return false;
}

// 更新学生信息
bool ScoreManager::updateStudent(const Student& student) {
    // 使用索引快速查找
    buildStudentIdIndex();
    
    auto it = studentIdIndex.find(student.getStudentId());
    if (it != studentIdIndex.end()) {
        students[it->second] = student;
        invalidateCache();  // 数据变更，使缓存失效
        return true;
    }
    return false;
}

// 根据学号查找学生（非const版本）
Student* ScoreManager::findStudentById(const string& studentId) {
    // 使用索引O(1)查找
    buildStudentIdIndex();
    
    auto it = studentIdIndex.find(studentId);
    if (it != studentIdIndex.end()) {
        return &students[it->second];
    }
    return nullptr;
}

// 根据学号查找学生（const版本）
const Student* ScoreManager::findStudentById(const string& studentId) const {
    // 使用索引O(1)查找
    buildStudentIdIndex();
    
    auto it = studentIdIndex.find(studentId);
    if (it != studentIdIndex.end()) {
        return &students[it->second];
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
    if (score < MIN_SCORE || score > MAX_SCORE) {
        return false;  // 分数无效
    }
    
    Student* student = findStudentById(studentId);
    if (student == nullptr) {
        return false;  // 学生不存在
    }
    
    bool result = student->setScore(subject, score);
    if (result) {
        invalidateCache();  // 数据变更，使缓存失效
    }
    return result;
}

// 排序学生（带缓存机制）
// 设计模式：Lazy Cache - 首次计算，后续直接用缓存
// 优化点：
// 1. 缓存排序结果，相同排序方式直接返回缓存
// 2. 使用移动语义返回，避免额外拷贝
// 3. 数据变更时缓存自动失效
vector<Student> ScoreManager::sortStudents(SortType sortType, const string& subject) const {
    // ============================ 缓存检查 ============================
    SortCacheKey key{sortType, subject};
    
    if (sortCacheValid) {
        auto it = sortCache.find(key);
        if (it != sortCache.end()) {
            // 缓存命中，返回副本（因为调用方可能需要修改结果）
            // 注意：如果调用方只读，可以考虑返回const引用
            return it->second;
        }
    }
    
    // ============================ 缓存未命中，重新计算 ============================
    vector<Student> sortedStudents = students;  // 必须拷贝，因为sort会修改原数组
    
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
    
    // ============================ 存入缓存 ============================
    sortCache[key] = sortedStudents;
    sortCacheValid = true;
    
    // 使用移动语义返回，避免额外拷贝
    return move(sortedStudents);
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

// 获取班级排名（同分处理）
// 设计原则：同分同名次，例如：
// 分数: 100, 95, 95, 90
// 排名: 1,   2,  2,  4
RankResult ScoreManager::getClassRankWithTie(const Student& student) const {
    RankResult result;
    result.rank = 1;
    result.truePosition = 1;
    result.isTied = false;
    result.tiedCount = 0;
    
    string className = student.getClassName();
    double average = student.getAverageScore();
    
    // 统计：
    // - 比当前学生分数高的人数 → 用于计算rank
    // - 与当前学生同分的人数 → 用于判断是否同分
    // - 当前学生的真实位置
    int higherCount = 0;
    int equalCount = 0;
    
    for (const auto& s : students) {
        if (s.getClassName() == className) {
            if (s.getAverageScore() > average) {
                higherCount++;
            } else if (s.getAverageScore() == average) {
                equalCount++;
            }
        }
    }
    
    result.rank = higherCount + 1;
    result.truePosition = higherCount + equalCount;
    result.isTied = equalCount > 1;
    result.tiedCount = equalCount;
    
    return result;
}

// 获取年级排名（同分处理）
// 年级排名 = 全校范围内的排名
RankResult ScoreManager::getGradeRank(const Student& student) const {
    RankResult result;
    result.rank = 1;
    result.truePosition = 1;
    result.isTied = false;
    result.tiedCount = 0;
    
    double average = student.getAverageScore();
    
    int higherCount = 0;
    int equalCount = 0;
    
    for (const auto& s : students) {
        // 全校范围内排名，不限制班级
        if (s.getAverageScore() > average) {
            higherCount++;
        } else if (s.getAverageScore() == average) {
            equalCount++;
        }
    }
    
    result.rank = higherCount + 1;
    result.truePosition = higherCount + equalCount;
    result.isTied = equalCount > 1;
    result.tiedCount = equalCount;
    
    return result;
}

// 计算标准差
// 标准差 = sqrt( sum((x - mean)^2) / N )
double ScoreManager::calculateStandardDeviation(const vector<double>& values, double mean) const {
    if (values.empty()) {
        return 0.0;
    }
    
    double sum = 0.0;
    for (double value : values) {
        double diff = value - mean;
        sum += diff * diff;
    }
    
    return sqrt(sum / values.size());
}

// 计算中位数
// 中位数：排序后中间位置的数值（奇数个取中间，偶数个取中间两个的平均）
double ScoreManager::calculateMedian(vector<double> values) const {
    if (values.empty()) {
        return 0.0;
    }
    
    sort(values.begin(), values.end());
    
    size_t size = values.size();
    if (size % 2 == 1) {
        // 奇数个，取中间值
        return values[size / 2];
    } else {
        // 偶数个，取中间两个的平均
        return (values[size / 2 - 1] + values[size / 2]) / 2.0;
    }
}

// 获取全校平均分
double ScoreManager::getSchoolAverage() const {
    if (students.empty()) {
        return 0.0;
    }
    
    double sum = 0.0;
    int count = 0;
    
    for (const auto& student : students) {
        if (student.isComplete()) {
            sum += student.getAverageScore();
            count++;
        }
    }
    
    return count > 0 ? sum / count : 0.0;
}

// 获取某科目的全校平均分
double ScoreManager::getSchoolSubjectAverage(const string& subject) const {
    if (students.empty()) {
        return 0.0;
    }
    
    double sum = 0.0;
    int count = 0;
    
    for (const auto& student : students) {
        if (student.hasScore(subject)) {
            sum += student.getScore(subject);
            count++;
        }
    }
    
    return count > 0 ? sum / count : 0.0;
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
    
    invalidateCache();  // 数据变更，使缓存失效
    
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
        if (student.hasScore(subject) && student.getScore(subject) >= PASS_SCORE) {
            count++;
        }
    }
    
    return count;
}

// 获取优秀人数（与等级评定一致：A=90+）
int ScoreManager::getExcellentCount(const string& subject) const {
    int count = 0;
    
    for (const auto& student : students) {
        if (student.hasScore(subject) && student.getScore(subject) >= EXCELLENT_SCORE) {
            count++;
        }
    }
    
    return count;
}

// 获取所有班级列表（带缓存机制）
// 设计模式：Lazy Cache - 首次计算，后续直接用缓存
// 原实现：O(n²)双重循环检查重复
// 新实现：unordered_set去重(O(n)) + 缓存(后续调用O(1))
vector<string> ScoreManager::getAllClasses() const {
    // ============================ 缓存检查 ============================
    if (classesCacheValid) {
        return classesCache;  // 缓存命中，O(1)返回
    }
    
    // ============================ 缓存未命中，重新计算 ============================
    // 使用unordered_set自动去重，避免O(n²)双重循环
    unordered_set<string> classSet;
    for (const auto& student : students) {
        classSet.insert(student.getClassName());
    }
    
    // 转换为vector
    vector<string> classes;
    classes.reserve(classSet.size());  // 预分配空间，避免重分配
    for (const auto& cls : classSet) {
        classes.push_back(cls);
    }
    
    // ============================ 存入缓存 ============================
    classesCache = classes;
    classesCacheValid = true;
    
    return classes;
}

// 获取班级统计报告（带缓存）
// 设计模式：Lazy Cache - 首次计算，后续直接用缓存
ClassStatistics ScoreManager::getClassStatistics(const string& className) const {
    // ============================ 缓存检查 ============================
    // 如果缓存有效且存在，直接返回缓存结果
    if (cacheValid) {
        auto it = classStatsCache.find(className);
        if (it != classStatsCache.end()) {
            return it->second;  // 缓存命中，O(1)返回
        }
    }
    
    // ============================ 缓存未命中，重新计算 ============================
    ClassStatistics stats;
    stats.className = className;
    stats.totalStudents = 0;
    stats.totalMaxScore = MIN_SCORE - 1;  // 使用常量，比最低分还低
    stats.totalMinScore = MAX_SCORE + 1;  // 使用常量，比最高分还高
    stats.overallAverage = 0.0;
    
    // 新字段初始化
    stats.standardDeviation = 0.0;
    stats.median = 0.0;
    stats.schoolAverage = 0.0;
    stats.differenceFromSchool = 0.0;
    stats.classRankByAverage = 0;
    stats.classRankByPassRate = 0;
    stats.classRankByExcellentRate = 0;
    stats.totalClasses = 0;
    
    double totalScoreSum = 0.0;
    int validStudentCount = 0;
    vector<double> classTotalScores;  // 用于计算标准差和中位数
    
    // 统计每个学生
    for (const auto& student : students) {
        if (student.getClassName() != className) continue;
        
        stats.totalStudents++;
        
        // 统计总分
        double totalScore = student.getTotalScore();
        if (student.getSubjectCount() > 0) {
            totalScoreSum += totalScore;
            validStudentCount++;
            classTotalScores.push_back(totalScore);
            
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
        
        // 计算标准差
        stats.standardDeviation = calculateStandardDeviation(classTotalScores, stats.overallAverage);
        
        // 计算中位数
        stats.median = calculateMedian(classTotalScores);
        
        // 计算与全校平均分的对比
        stats.schoolAverage = getSchoolAverage();
        stats.differenceFromSchool = stats.overallAverage - stats.schoolAverage;
    }
    
    // 统计每个科目
    for (const auto& subject : subjects) {
        SubjectStats subjectStats;
        subjectStats.subject = subject;
        subjectStats.average = 0.0;
        subjectStats.maxScore = MIN_SCORE - 1;  // 使用常量
        subjectStats.minScore = MAX_SCORE + 1;  // 使用常量
        subjectStats.passCount = 0;
        subjectStats.excellentCount = 0;
        
        // 新字段初始化
        subjectStats.standardDeviation = 0.0;
        subjectStats.median = 0.0;
        subjectStats.schoolAverage = 0.0;
        subjectStats.differenceFromSchool = 0.0;
        subjectStats.passRateRank = 0;
        subjectStats.excellentRateRank = 0;
        subjectStats.totalClasses = 0;
        
        double scoreSum = 0.0;
        int scoreCount = 0;
        vector<double> subjectScores;  // 用于计算标准差和中位数
        
        for (const auto& student : students) {
            if (student.getClassName() != className) continue;
            if (!student.hasScore(subject)) continue;
            
            double score = student.getScore(subject);
            scoreSum += score;
            scoreCount++;
            subjectScores.push_back(score);
            
            if (score > subjectStats.maxScore) {
                subjectStats.maxScore = score;
                subjectStats.maxStudentId = student.getStudentId();
            }
            
            if (score < subjectStats.minScore) {
                subjectStats.minScore = score;
                subjectStats.minStudentId = student.getStudentId();
            }
            
            // 使用常量：PASS_SCORE = 60
            if (score >= PASS_SCORE) {
                subjectStats.passCount++;
            }
            // 使用常量：EXCELLENT_SCORE = 90（与等级评定一致）
            // 修复：原来用85，现在统一用90
            if (score >= EXCELLENT_SCORE) {
                subjectStats.excellentCount++;
            }
        }
        
        if (scoreCount > 0) {
            subjectStats.average = scoreSum / scoreCount;
            subjectStats.passRate = static_cast<double>(subjectStats.passCount) / scoreCount * 100;
            subjectStats.excellentRate = static_cast<double>(subjectStats.excellentCount) / scoreCount * 100;
            
            // 计算标准差和中位数
            subjectStats.standardDeviation = calculateStandardDeviation(subjectScores, subjectStats.average);
            subjectStats.median = calculateMedian(subjectScores);
            
            // 计算与全校平均分的对比
            subjectStats.schoolAverage = getSchoolSubjectAverage(subject);
            subjectStats.differenceFromSchool = subjectStats.average - subjectStats.schoolAverage;
        }
        
        stats.subjectStats.push_back(subjectStats);
    }
    
    // ============================ 计算班级排名 ============================
    vector<string> allClasses = getAllClasses();
    stats.totalClasses = allClasses.size();
    
    // 只有一个班级时，排名都是1
    if (allClasses.size() == 1) {
        stats.classRankByAverage = 1;
        stats.classRankByPassRate = 1;
        stats.classRankByExcellentRate = 1;
        for (auto& subj : stats.subjectStats) {
            subj.passRateRank = 1;
            subj.excellentRateRank = 1;
            subj.totalClasses = 1;
        }
    } else if (allClasses.size() > 1) {
        // 计算各班级的统计指标用于排名
        // 这里使用一个简化的方法：收集所有班级的相同指标，然后计算排名
        
        // 1. 按平均分排名
        vector<pair<double, string>> classAverages;
        for (const auto& cls : allClasses) {
            double avg = getClassAverage(cls);
            classAverages.emplace_back(avg, cls);
        }
        sort(classAverages.rbegin(), classAverages.rend());  // 降序排序
        
        for (size_t i = 0; i < classAverages.size(); i++) {
            if (classAverages[i].second == className) {
                stats.classRankByAverage = i + 1;
                break;
            }
        }
        
        // 2. 按科目及格率和优秀率排名
        for (auto& subj : stats.subjectStats) {
            // 收集所有班级该科目的及格率
            vector<pair<double, string>> passRates;
            vector<pair<double, string>> excellentRates;
            
            for (const auto& cls : allClasses) {
                // 计算该班级该科目的及格率和优秀率
                int passCount = 0;
                int excellentCount = 0;
                int totalCount = 0;
                
                for (const auto& student : students) {
                    if (student.getClassName() != cls) continue;
                    if (!student.hasScore(subj.subject)) continue;
                    
                    totalCount++;
                    if (student.getScore(subj.subject) >= PASS_SCORE) {
                        passCount++;
                    }
                    if (student.getScore(subj.subject) >= EXCELLENT_SCORE) {
                        excellentCount++;
                    }
                }
                
                if (totalCount > 0) {
                    double passRate = static_cast<double>(passCount) / totalCount * 100;
                    double excellentRate = static_cast<double>(excellentCount) / totalCount * 100;
                    passRates.emplace_back(passRate, cls);
                    excellentRates.emplace_back(excellentRate, cls);
                }
            }
            
            // 计算及格率排名
            sort(passRates.rbegin(), passRates.rend());
            for (size_t i = 0; i < passRates.size(); i++) {
                if (passRates[i].second == className) {
                    subj.passRateRank = i + 1;
                    break;
                }
            }
            
            // 计算优秀率排名
            sort(excellentRates.rbegin(), excellentRates.rend());
            for (size_t i = 0; i < excellentRates.size(); i++) {
                if (excellentRates[i].second == className) {
                    subj.excellentRateRank = i + 1;
                    break;
                }
            }
            
            subj.totalClasses = allClasses.size();
        }
        
        // 3. 按班级整体及格率和优秀率排名
        // 简化：使用第一个科目的排名作为班级排名（或者可以计算综合排名）
        if (!stats.subjectStats.empty()) {
            stats.classRankByPassRate = stats.subjectStats[0].passRateRank;
            stats.classRankByExcellentRate = stats.subjectStats[0].excellentRateRank;
        }
    }
    
    // ============================ 存入缓存 ============================
    classStatsCache[className] = stats;
    cacheValid = true;
    
    return stats;
}

// 获取分数段统计
// 设计原则：使用可配置的分数段，消除硬编码
vector<ScoreRangeStats> ScoreManager::getScoreRangeStats(const string& subject) const {
    // 使用可配置的分数段（与等级评定对应）
    const vector<ScoreRangeDef>& ranges = DEFAULT_SCORE_RANGES;
    
    vector<ScoreRangeStats> stats;
    // 动态初始化分数段
    for (const auto& range : ranges) {
        ScoreRangeStats s;
        s.range = range.displayName;
        s.count = 0;
        s.percentage = 0.0;
        stats.push_back(s);
    }
    
    int totalCount = 0;
    
    for (const auto& student : students) {
        double score;
        bool hasValidScore = false;
        
        if (subject.empty()) {
            // 按平均分统计
            if (student.getSubjectCount() > 0) {
                score = student.getAverageScore();
                hasValidScore = true;
            }
        } else {
            // 按单科统计
            if (student.hasScore(subject)) {
                score = student.getScore(subject);
                hasValidScore = true;
            }
        }
        
        if (hasValidScore) {
            totalCount++;
            // 使用可配置的分数段进行判断
            for (size_t i = 0; i < ranges.size(); ++i) {
                if (score >= ranges[i].minScore && score <= ranges[i].maxScore) {
                    stats[i].count++;
                    break;
                }
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
    
    invalidateCache();  // 数据变更，使缓存失效
    
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
// 修复Excel中文乱码：添加UTF-8 BOM头
// UTF-8 BOM字节：0xEF 0xBB 0xBF
// Excel需要BOM才能正确识别UTF-8编码的中文
bool ScoreManager::saveToCSVFile(const string& filename) const {
    ofstream file(filename, ios::binary);  // 二进制模式，避免BOM被转换
    if (!file.is_open()) {
        return false;
    }
    
    // 写入UTF-8 BOM头，解决Excel中文乱码问题
    const unsigned char utf8BOM[] = {0xEF, 0xBB, 0xBF};
    file.write(reinterpret_cast<const char*>(utf8BOM), sizeof(utf8BOM));
    
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
// 修复：处理UTF-8 BOM头（文件可能以EF BB BF开头）
bool ScoreManager::loadFromCSVFile(const string& filename) {
    ifstream file(filename, ios::binary);  // 二进制模式读取，正确检测BOM
    if (!file.is_open()) {
        return false;
    }
    
    // 检测并跳过UTF-8 BOM
    char bom[3];
    if (file.read(bom, 3)) {
        // 检查是否是UTF-8 BOM：0xEF 0xBB 0xBF
        if (!(static_cast<unsigned char>(bom[0]) == 0xEF &&
              static_cast<unsigned char>(bom[1]) == 0xBB &&
              static_cast<unsigned char>(bom[2]) == 0xBF)) {
            // 不是BOM，把读取的3个字节放回流中
            file.seekg(0);
        }
    } else {
        // 文件太短，重置到开头
        file.seekg(0);
    }
    
    // 现在用文本模式重新打开，方便getline处理
    file.close();
    ifstream textFile(filename);
    if (!textFile.is_open()) {
        return false;
    }
    
    // 再次检测BOM并跳过
    char bomCheck[3] = {0};
    textFile.read(bomCheck, 3);
    if (!(static_cast<unsigned char>(bomCheck[0]) == 0xEF &&
          static_cast<unsigned char>(bomCheck[1]) == 0xBB &&
          static_cast<unsigned char>(bomCheck[2]) == 0xBF)) {
        // 不是BOM，重置到开头
        textFile.seekg(0);
    }
    
    vector<string> tempSubjects;
    vector<Student> tempStudents;
    
    string line;
    
    // 读取第一行（表头）
    if (!getline(textFile, line)) {
        textFile.close();
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
    while (getline(textFile, line)) {
        if (line.empty()) continue;
        
        vector<string> cells;
        istringstream lineStream(line);
        string cell;
        
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
    
    textFile.close();
    
    if (tempSubjects.empty() && tempStudents.empty()) {
        return false;
    }
    
    // 如果文件中没有科目，使用默认科目
    if (tempSubjects.empty()) {
        tempSubjects = {"语文", "数学", "英语", "物理", "化学", "生物", "历史", "地理", "政治"};
    }
    
    students.swap(tempStudents);
    subjects.swap(tempSubjects);
    
    invalidateCache();  // 数据变更，使缓存失效
    
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
