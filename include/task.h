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

int getNextTaskId(const std::vector<Task>& tasks);
std::string taskStatusToString(TaskStatus status);
TaskStatus taskStatusFromString(const std::string& value);
void addTask(std::vector<Task>& tasks, const Task& task);
bool deleteTaskById(std::vector<Task>& tasks, int id);
Task* findTaskById(std::vector<Task>& tasks, int id);

#endif

