#include "task.h"

#include <algorithm>
#include <cctype>

// ========== 增 — 添加待办事项 ==========

int getNextTaskId(const std::vector<Task>& tasks) {
    int maxId = 0;
    for (const Task& task : tasks) {
        if (task.id > maxId) {
            maxId = task.id;
        }
    }
    return maxId + 1;
}

void addTask(std::vector<Task>& tasks, const Task& task) {
    tasks.push_back(task);
}

// ========== 删 — 删除待办事项 ==========

bool deleteTaskById(std::vector<Task>& tasks, int id) {
    std::vector<Task>::iterator it = std::remove_if(
        tasks.begin(), tasks.end(),
        [id](const Task& task) { return task.id == id; });

    if (it == tasks.end()) {
        return false;
    }

    tasks.erase(it, tasks.end());
    return true;
}

// ========== 改 — 修改待办事项 ==========

Task* findTaskById(std::vector<Task>& tasks, int id) {
    for (Task& task : tasks) {
        if (task.id == id) {
            return &task;
        }
    }
    return nullptr;
}

bool updateTask(Task& task,
                const std::string& title,
                const std::string& description,
                const std::string& dueDate,
                int priority) {
    if (title.empty()) {
        return false;
    }
    if (priority < 1 || priority > 5) {
        return false;
    }

    task.title = title;
    task.description = description;
    task.dueDate = dueDate;
    task.priority = priority;
    return true;
}

bool markTaskCompleted(Task& task) {
    if (task.status == TaskStatus::Completed) {
        return false;  // 已经是完成状态
    }
    task.status = TaskStatus::Completed;
    return true;
}

bool markTaskPending(Task& task) {
    if (task.status == TaskStatus::Pending) {
        return false;  // 已经是待办状态
    }
    task.status = TaskStatus::Pending;
    return true;
}

// ========== 查 — 查询待办事项 ==========

std::vector<Task*> findTasksByTitle(std::vector<Task>& tasks,
                                    const std::string& keyword) {
    std::vector<Task*> result;
    if (keyword.empty()) {
        return result;
    }

    // 将关键字转为小写用于不区分大小写的比较
    std::string lowerKeyword = keyword;
    for (char& c : lowerKeyword) {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }

    for (Task& task : tasks) {
        std::string lowerTitle = task.title;
        for (char& c : lowerTitle) {
            c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        }
        if (lowerTitle.find(lowerKeyword) != std::string::npos) {
            result.push_back(&task);
        }
    }
    return result;
}

std::vector<Task*> findTasksByStatus(std::vector<Task>& tasks,
                                     TaskStatus status) {
    std::vector<Task*> result;
    for (Task& task : tasks) {
        if (task.status == status) {
            result.push_back(&task);
        }
    }
    return result;
}

std::vector<Task*> findTasksByPriority(std::vector<Task>& tasks,
                                       int priority) {
    std::vector<Task*> result;
    for (Task& task : tasks) {
        if (task.priority == priority) {
            result.push_back(&task);
        }
    }
    return result;
}

// ========== 排序 ==========

void sortTasksByPriority(std::vector<Task>& tasks, bool ascending) {
    if (ascending) {
        // 优先级数字越小越高，升序即从高到低
        std::sort(tasks.begin(), tasks.end(),
                  [](const Task& a, const Task& b) {
                      return a.priority < b.priority;
                  });
    } else {
        std::sort(tasks.begin(), tasks.end(),
                  [](const Task& a, const Task& b) {
                      return a.priority > b.priority;
                  });
    }
}

void sortTasksByDueDate(std::vector<Task>& tasks, bool ascending) {
    if (ascending) {
        // 升序：最早截止日期在前
        std::sort(tasks.begin(), tasks.end(),
                  [](const Task& a, const Task& b) {
                      return a.dueDate < b.dueDate;
                  });
    } else {
        std::sort(tasks.begin(), tasks.end(),
                  [](const Task& a, const Task& b) {
                      return a.dueDate > b.dueDate;
                  });
    }
}

// ========== 统计 ==========

int countTasksByStatus(const std::vector<Task>& tasks, TaskStatus status) {
    int count = 0;
    for (const Task& task : tasks) {
        if (task.status == status) {
            ++count;
        }
    }
    return count;
}

int countAllTasks(const std::vector<Task>& tasks) {
    return static_cast<int>(tasks.size());
}

// ========== 工具函数 ==========

std::string taskStatusToString(TaskStatus status) {
    return status == TaskStatus::Completed ? "Completed" : "Pending";
}

TaskStatus taskStatusFromString(const std::string& value) {
    return value == "Completed" ? TaskStatus::Completed : TaskStatus::Pending;
}
