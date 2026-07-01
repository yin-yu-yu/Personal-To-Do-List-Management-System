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
    std::cout << "6. 搜索待办事项\n";
    std::cout << "7. 查看统计信息\n";
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

static void showTaskDetail(const Task& task) {
    std::cout << "\n--- 待办事项详情 ---\n";
    std::cout << "ID：" << task.id << "\n";
    std::cout << "标题：" << task.title << "\n";
    std::cout << "描述：" << task.description << "\n";
    std::cout << "截止日期：" << task.dueDate << "\n";
    std::cout << "优先级：" << task.priority << "\n";
    std::cout << "状态：" << taskStatusToString(task.status) << "\n";
}

// ========== 功能处理函数 ==========

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
    if (task == nullptr) {
        std::cout << "未找到该待办事项。\n";
        return;
    }

    showTaskDetail(*task);

    std::string title = readLine("请输入新标题（直接回车保留原值）：");
    std::string description = readLine("请输入新描述（直接回车保留原值）：");
    std::string dueDate = readLine("请输入新截止日期（直接回车保留原值）：");
    int priority = readInt("请输入新优先级，1 最高，5 最低（输入 0 保留原值）：");

    // 保留原值
    if (title.empty()) {
        title = task->title;
    }
    if (description.empty()) {
        description = task->description;
    }
    if (dueDate.empty()) {
        dueDate = task->dueDate;
    }
    if (priority == 0) {
        priority = task->priority;
    }

    if (updateTask(*task, title, description, dueDate, priority)) {
        std::cout << "修改成功。\n";
    } else {
        std::cout << "修改失败，请检查输入是否合法。\n";
    }
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
    if (task == nullptr) {
        std::cout << "未找到该待办事项。\n";
        return;
    }

    if (markTaskCompleted(*task)) {
        std::cout << "状态已更新为已完成。\n";
    } else {
        std::cout << "该待办事项已经是完成状态。\n";
    }
}

static void handleSearchTasks(std::vector<Task>& tasks) {
    std::cout << "\n--- 搜索待办事项 ---\n";
    std::cout << "1. 按标题关键字搜索\n";
    std::cout << "2. 按状态筛选（Pending）\n";
    std::cout << "3. 按状态筛选（Completed）\n";
    std::cout << "4. 按优先级筛选\n";
    int choice = readInt("请选择搜索方式：");

    std::vector<Task*> results;

    switch (choice) {
        case 1: {
            std::string keyword = readLine("请输入标题关键字：");
            results = findTasksByTitle(tasks, keyword);
            break;
        }
        case 2: {
            results = findTasksByStatus(tasks, TaskStatus::Pending);
            break;
        }
        case 3: {
            results = findTasksByStatus(tasks, TaskStatus::Completed);
            break;
        }
        case 4: {
            int priority = readInt("请输入要筛选的优先级（1-5）：");
            results = findTasksByPriority(tasks, priority);
            break;
        }
        default:
            std::cout << "无效选择。\n";
            return;
    }

    if (results.empty()) {
        std::cout << "未找到匹配的待办事项。\n";
        return;
    }

    std::cout << "\n找到 " << results.size() << " 条匹配结果：\n";
    std::cout << "ID | 标题 | 描述 | 截止日期 | 优先级 | 状态\n";
    std::cout << "--------------------------------------------------\n";
    for (const Task* task : results) {
        std::cout << task->id << " | "
                  << task->title << " | "
                  << task->description << " | "
                  << task->dueDate << " | "
                  << task->priority << " | "
                  << taskStatusToString(task->status) << "\n";
    }
}

static void handleShowStats(const std::vector<Task>& tasks) {
    std::cout << "\n--- 统计信息 ---\n";
    std::cout << "待办事项总数：" << countAllTasks(tasks) << "\n";
    std::cout << "待完成数量：" << countTasksByStatus(tasks, TaskStatus::Pending) << "\n";
    std::cout << "已完成数量：" << countTasksByStatus(tasks, TaskStatus::Completed) << "\n";
}

// ========== 主程序入口 ==========

void runApplication(const std::string& dataFilePath) {
    std::vector<Task> tasks;
    if (!loadTasksFromFile(dataFilePath, tasks)) {
        std::cout << "提示：未找到数据文件，将创建新的任务列表。\n";
    }

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
            case 6:
                handleSearchTasks(tasks);
                break;
            case 7:
                handleShowStats(tasks);
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
