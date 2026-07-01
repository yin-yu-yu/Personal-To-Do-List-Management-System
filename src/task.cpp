#include "task.h"

#include <algorithm>

int getNextTaskId(const std::vector<Task>& tasks) {
    int maxId = 0;
    for (const Task& task : tasks) {
        if (task.id > maxId) {
            maxId = task.id;
        }
    }
    return maxId + 1;
}

std::string taskStatusToString(TaskStatus status) {
    return status == TaskStatus::Completed ? "Completed" : "Pending";
}

TaskStatus taskStatusFromString(const std::string& value) {
    return value == "Completed" ? TaskStatus::Completed : TaskStatus::Pending;
}

void addTask(std::vector<Task>& tasks, const Task& task) {
    tasks.push_back(task);
}

bool deleteTaskById(std::vector<Task>& tasks, int id) {
    std::vector<Task>::iterator it = std::remove_if(tasks.begin(), tasks.end(), [id](const Task& task) {
        return task.id == id;
    });

    if (it == tasks.end()) {
        return false;
    }

    tasks.erase(it, tasks.end());
    return true;
}

Task* findTaskById(std::vector<Task>& tasks, int id) {
    for (Task& task : tasks) {
        if (task.id == id) {
            return &task;
        }
    }
    return 0;
}

