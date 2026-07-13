#include "task.h"

#include <algorithm>
#include <cctype>

// ========== �� �� ���Ӵ������� ==========

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

// ========== ɾ �� ɾ���������� ==========

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

// ========== �� �� �޸Ĵ������� ==========

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
                int priority,
                const std::string& tag) {
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
    task.tag = tag;
    return true;
}

bool markTaskCompleted(Task& task) {
    if (task.status == TaskStatus::Completed) {
        return false;  // �Ѿ������״̬
    }
    task.status = TaskStatus::Completed;
    return true;
}

bool markTaskPending(Task& task) {
    if (task.status == TaskStatus::Pending) {
        return false;  // �Ѿ��Ǵ���״̬
    }
    task.status = TaskStatus::Pending;
    return true;
}

// ========== �� �� ��ѯ�������� ==========

std::vector<Task*> findTasksByTitle(std::vector<Task>& tasks,
                                    const std::string& keyword) {
    std::vector<Task*> result;
    if (keyword.empty()) {
        return result;
    }

    // ���ؼ���תΪСд���ڲ����ִ�Сд�ıȽ�
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

std::vector<Task*> findTasksByTag(std::vector<Task>& tasks,
                                  const std::string& tag) {
    std::vector<Task*> result;
    if (tag.empty()) {
        return result;
    }

    std::string lowerTag = tag;
    for (char& c : lowerTag) {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }

    for (Task& task : tasks) {
        std::string lowerTaskTag = task.tag;
        for (char& c : lowerTaskTag) {
            c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        }
        if (lowerTaskTag.find(lowerTag) != std::string::npos) {
            result.push_back(&task);
        }
    }
    return result;
}

// ========== ���� ==========

void sortTasksByPriority(std::vector<Task>& tasks, bool ascending) {
    if (ascending) {
        // ���ȼ�����ԽСԽ�ߣ����򼴴Ӹߵ���
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
        // ���������ֹ������ǰ
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

// ========== ͳ�� ==========

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

// ========== ���ߺ��� ==========

std::string taskStatusToString(TaskStatus status) {
    return status == TaskStatus::Completed ? "Completed" : "Pending";
}

TaskStatus taskStatusFromString(const std::string& value) {
    return value == "Completed" ? TaskStatus::Completed : TaskStatus::Pending;
}
