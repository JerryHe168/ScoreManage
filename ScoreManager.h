#ifndef SCOREMANAGER_H
#define SCOREMANAGER_H

#include "Student.h"
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <functional>

using namespace std;

// ============================ 常量定义 ============================
// 设计原则：所有阈值常量集中管理，避免硬编码散落在代码各处
// 与等级评定保持一致：
//   A: 90-100 (优秀)
//   B: 80-89  (良好)
//   C: 70-79  (中等)
//   D: 60-69  (及格)
//   F: 0-59   (不及格)

const double MIN_SCORE = 0.0;
const double MAX_SCORE = 100.0;

const double PASS_SCORE = 60.0;           // 及格分数
const double FAIR_SCORE = 70.0;           // 中等分数
const double GOOD_SCORE = 80.0;            // 良好分数
const double EXCELLENT_SCORE = 90.0;       // 优秀分数（与等级评定一致，A=90+）

// ============================ 分数段定义 ============================
// 可配置的分数段结构，替代硬编码
struct ScoreRangeDef {
    string name;
    string displayName;
    double minScore;
    double maxScore;
};

// 默认分数段配置（与等级评定对应）
const vector<ScoreRangeDef> DEFAULT_SCORE_RANGES = {
    {"90-100", "90-100", EXCELLENT_SCORE, MAX_SCORE},
    {"80-89", "80-89", GOOD_SCORE, EXCELLENT_SCORE - 1.0},
    {"70-79", "70-79", FAIR_SCORE, GOOD_SCORE - 1.0},
    {"60-69", "60-69", PASS_SCORE, FAIR_SCORE - 1.0},
    {"0-59", "0-59", MIN_SCORE, PASS_SCORE - 1.0}
};

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

// ============================ 统计数据结构（类外部声明，解决声明顺序问题） ============================
// 设计原则：只存储学生ID（主键），姓名通过ID实时查询
// 优点：无数据冗余、保证数据一致性、节省内存

struct SubjectStats {
    string subject;
    double average;
    double maxScore;
    string maxStudentId;      // 只存ID，显示时通过ID查姓名
    double minScore;
    string minStudentId;      // 只存ID，显示时通过ID查姓名
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
    string totalMaxStudentId;   // 只存ID
    double totalMinScore;
    string totalMinStudentId;   // 只存ID
    double overallAverage;
};

// 分数段统计结构
struct ScoreRangeStats {
    string range;
    int count;
    double percentage;
};

// ============================ 排序缓存键（类外部声明，解决Visual Studio编译问题） ============================
// 排序缓存键：排序类型 + 科目名（仅单科排序需要）
struct SortCacheKey {
    SortType sortType;
    string subject;
    
    // 用于unordered_map的比较
    bool operator==(const SortCacheKey& other) const {
        return sortType == other.sortType && subject == other.subject;
    }
};

// 排序缓存键的哈希函数（类外部，解决Visual Studio编译问题）
struct SortCacheKeyHash {
    size_t operator()(const SortCacheKey& key) const {
        size_t h1 = hash<int>()(static_cast<int>(key.sortType));
        size_t h2 = hash<string>()(key.subject);
        return h1 ^ (h2 << 1);
    }
};

class ScoreManager {
private:
    vector<Student> students;
    vector<string> subjects;  // 支持的科目列表
    
    // ============================ 缓存机制 ============================
    // 设计模式：Lazy Cache + Invalidation
    // 问题：每次统计都O(n)遍历，效率低下
    // 解决方案：缓存统计结果，数据变更时清除缓存
    
    mutable bool cacheValid;  // 缓存是否有效（mutable允许const方法修改）
    
    // 班级统计缓存： className -> ClassStatistics
    mutable unordered_map<string, ClassStatistics> classStatsCache;
    
    // 学生ID索引缓存： O(1)查找，替代每次O(n)遍历
    mutable unordered_map<string, size_t> studentIdIndex;  // studentId -> vector索引
    
    // 班级列表缓存：避免每次getAllClasses都遍历所有学生
    mutable vector<string> classesCache;
    mutable bool classesCacheValid;  // 班级列表缓存是否有效
    
    // ============================ 排序结果缓存 ============================
    // 设计原则：缓存最近使用的排序结果
    // 问题：sortStudents每次都拷贝整个数组，然后排序
    // 解决方案：缓存最近一次的排序结果，如果排序方式相同则直接返回
    
    // 排序结果缓存
    mutable unordered_map<SortCacheKey, vector<Student>, SortCacheKeyHash> sortCache;
    mutable bool sortCacheValid;  // 排序缓存是否有效
    
    // 私有辅助函数
    void buildStudentIdIndex() const;  // 构建学生ID索引
    void invalidateCache() const;       // 使所有缓存失效（const方法可以调用，因为mutable）

public:
    // 构造函数
    ScoreManager();

    // 科目管理
    void initDefaultSubjects();
    bool addSubject(const string& subject);
    bool removeSubject(const string& subject);
    vector<string> getSubjects() const;
    bool hasSubject(const string& subject) const;

    // 学生信息管理
    bool addStudent(const Student& student);
    bool removeStudent(const string& studentId);
    bool updateStudent(const Student& student);
    Student* findStudentById(const string& studentId);
    const Student* findStudentById(const string& studentId) const;  // const版本
    vector<Student> getAllStudents() const;
    int getStudentCount() const;

    // 成绩管理
    bool setStudentScore(const string& studentId, const string& subject, double score);
    bool validateScore(double score) const;

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
    
    // 班级统计（带缓存）
    ClassStatistics getClassStatistics(const string& className) const;
    
    // 分数段统计（使用可配置的分数段）
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
