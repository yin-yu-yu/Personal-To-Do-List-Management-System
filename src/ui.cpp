#include "ui.h"

#include "storage.h"

#include <iostream>
#include <limits>

static void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static std::string readLine(const std::string& prompt) {
    std::string value;
    std::cout << prompt;
    std::getline(std::cin, value);
    return value;
}

static int readInt(const std::string& prompt) {
    int value = 0;
    std::cout << prompt;
    while (!(std::cin >> value)) {
        clearInput();
        std::cout << "输入无效，请重新输入数字：";
    }
    clearInput();
    return value;
}

void showMainMenu() {
    std::cout << "\n========== 个人待办事项管理系统 ==========\n";
    std::cout << "1. 添加待办事项\n";
    std::cout << "2. 查看全部待办事项\n";
    std::cout << "3. 修改待办事项\n";
    std::cout << "4. 删除待办事项\n";
    std::cout << "5. 标记为已完成\n";
    std::cout << "0. 保存并退出\n";
    std::cout << "请选择功能：";
}

void showTasks(const std::vector<Task>& tasks) {
    if (tasks.empty()) {
        std::cout << "暂无待办事项。\n";
        return;
    }

    std::cout << "\nID | 标题 | 描述 | 截止日期 | 优先级 | 状态\n";
    std::cout << "--------------------------------------------------\n";
    for (const Task& task : tasks) {
        std::cout << task.id << " | "
                  << task.title << " | "
                  << task.description << " | "
                  << task.dueDate << " | "
                  << task.priority << " | "
                  << taskStatusToString(task.status) << "\n";
    }
}

static void handleAddTask(std::vector<Task>& tasks) {
    Task task;
    task.id = getNextTaskId(tasks);
    task.title = readLine("请输入标题：");
    task.description = readLine("请输入描述：");
    task.dueDate = readLine("请输入截止日期，例如 2026-07-01：");
    task.priority = readInt("请输入优先级，1 最高，5 最低：");
    task.status = TaskStatus::Pending;

    addTask(tasks, task);
    std::cout << "添加成功。\n";
}

static void handleEditTask(std::vector<Task>& tasks) {
    int id = readInt("请输入要修改的待办事项 ID：");
    Task* task = findTaskById(tasks, id);
    if (task == 0) {
        std::cout << "未找到该待办事项。\n";
        return;
    }

    task->title = readLine("请输入新标题：");
    task->description = readLine("请输入新描述：");
    task->dueDate = readLine("请输入新截止日期：");
    task->priority = readInt("请输入新优先级：");
    std::cout << "修改成功。\n";
}

static void handleDeleteTask(std::vector<Task>& tasks) {
    int id = readInt("请输入要删除的待办事项 ID：");
    if (deleteTaskById(tasks, id)) {
        std::cout << "删除成功。\n";
    } else {
        std::cout << "未找到该待办事项。\n";
    }
}

static void handleCompleteTask(std::vector<Task>& tasks) {
    int id = readInt("请输入要标记完成的待办事项 ID：");
    Task* task = findTaskById(tasks, id);
    if (task == 0) {
        std::cout << "未找到该待办事项。\n";
        return;
    }

    task->status = TaskStatus::Completed;
    std::cout << "状态已更新为已完成。\n";
}

void runApplication(const std::string& dataFilePath) {
    std::vector<Task> tasks;
    loadTasksFromFile(dataFilePath, tasks);

    int choice = -1;
    while (choice != 0) {
        showMainMenu();
        std::cin >> choice;
        clearInput();

        switch (choice) {
            case 1:
                handleAddTask(tasks);
                break;
            case 2:
                showTasks(tasks);
                break;
            case 3:
                handleEditTask(tasks);
                break;
            case 4:
                handleDeleteTask(tasks);
                break;
            case 5:
                handleCompleteTask(tasks);
                break;
            case 0:
                saveTasksToFile(dataFilePath, tasks);
                std::cout << "数据已保存，程序退出。\n";
                break;
            default:
                std::cout << "无效选择，请重新输入。\n";
                break;
        }
    }
}

