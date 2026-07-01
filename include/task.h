#ifndef TASK_H
#define TASK_H

#include <string>
#include <vector>

enum class TaskStatus {
    Pending,
    Completed
};

struct Task {
    int id;
    std::string title;
    std::string description;
    std::string dueDate;
    int priority;
    TaskStatus status;
};

// 增 — 添加待办事项
int getNextTaskId(const std::vector<Task>& tasks);
void addTask(std::vector<Task>& tasks, const Task& task);

// 删 — 删除待办事项
bool deleteTaskById(std::vector<Task>& tasks, int id);

// 改 — 修改待办事项
Task* findTaskById(std::vector<Task>& tasks, int id);
bool updateTask(Task& task,
                const std::string& title,
                const std::string& description,
                const std::string& dueDate,
                int priority);
bool markTaskCompleted(Task& task);
bool markTaskPending(Task& task);

// 查 — 查询待办事项
std::vector<Task*> findTasksByTitle(std::vector<Task>& tasks,
                                    const std::string& keyword);
std::vector<Task*> findTasksByStatus(std::vector<Task>& tasks,
                                     TaskStatus status);
std::vector<Task*> findTasksByPriority(std::vector<Task>& tasks,
                                       int priority);

// 排序
void sortTasksByPriority(std::vector<Task>& tasks, bool ascending = true);
void sortTasksByDueDate(std::vector<Task>& tasks, bool ascending = true);

// 统计
int countTasksByStatus(const std::vector<Task>& tasks, TaskStatus status);
int countAllTasks(const std::vector<Task>& tasks);

// 工具
std::string taskStatusToString(TaskStatus status);
TaskStatus taskStatusFromString(const std::string& value);

#endif
