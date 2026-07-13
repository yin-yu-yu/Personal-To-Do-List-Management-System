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
    std::string tag;
};

// �� �� ���Ӵ�������
int getNextTaskId(const std::vector<Task>& tasks);
void addTask(std::vector<Task>& tasks, const Task& task);

// ɾ �� ɾ����������
bool deleteTaskById(std::vector<Task>& tasks, int id);

// �� �� �޸Ĵ�������
Task* findTaskById(std::vector<Task>& tasks, int id);
bool updateTask(Task& task,
                const std::string& title,
                const std::string& description,
                const std::string& dueDate,
                int priority,
                const std::string& tag);
bool markTaskCompleted(Task& task);
bool markTaskPending(Task& task);

// �� �� ��ѯ��������
std::vector<Task*> findTasksByTitle(std::vector<Task>& tasks,
                                    const std::string& keyword);
std::vector<Task*> findTasksByStatus(std::vector<Task>& tasks,
                                     TaskStatus status);
std::vector<Task*> findTasksByPriority(std::vector<Task>& tasks,
                                       int priority);
std::vector<Task*> findTasksByTag(std::vector<Task>& tasks,
                                  const std::string& tag);

// ����
void sortTasksByPriority(std::vector<Task>& tasks, bool ascending = true);
void sortTasksByDueDate(std::vector<Task>& tasks, bool ascending = true);

// ͳ��
int countTasksByStatus(const std::vector<Task>& tasks, TaskStatus status);
int countAllTasks(const std::vector<Task>& tasks);

// ����
std::string taskStatusToString(TaskStatus status);
TaskStatus taskStatusFromString(const std::string& value);

#endif
