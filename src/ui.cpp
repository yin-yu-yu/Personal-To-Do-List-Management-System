#include "ui.h"
#include "storage.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <limits>

// ========== 内部辅助函数 ==========

// 清空输入缓冲区的错误状态和残留数据
static void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// 读取一行字符串，自动去除首尾空白
static std::string readLine(const std::string& prompt) {
    std::string value;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, value);
        // 去除首尾空白
        size_t start = value.find_first_not_of(" \t");
        if (start == std::string::npos) {
            std::cout << "输入不能为空，请重新输入。\n";
            continue;
        }
        size_t end = value.find_last_not_of(" \t");
        value = value.substr(start, end - start + 1);
        return value;
    }
    return value;
}

// 读取一个整数，带有输入校验
static int readInt(const std::string& prompt) {
    int value = 0;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            clearInput();
            return value;
        }
        clearInput();
        std::cout << "输入无效，请输入一个整数。\n";
    }
}

// 读取指定范围内的整数（min <= value <= max）
static int readIntInRange(const std::string& prompt, int min, int max) {
    int value = 0;
    while (true) {
        value = readInt(prompt);
        if (value >= min && value <= max) {
            return value;
        }
        std::cout << "请输入 " << min << " 到 " << max << " 之间的数字。\n";
    }
}

// 读取菜单选项（0-7）
static int readMenuChoice() {
    return readIntInRange("请选择功能：", 0, 7);
}

// 确认操作（返回 true 表示用户确认）
static bool confirm(const std::string& message) {
    std::cout << message << " (y/n)：";
    std::string input;
    std::getline(std::cin, input);
    return input == "y" || input == "Y" || input == "yes" || input == "Yes";
}

// ========== 公开函数 ==========

void clearScreen() {
    std::cout << "\x1B[2J\x1B[H" << std::flush;
}

void pauseForUser() {
    std::cout << "\n按 Enter 键返回主菜单...";
    std::cin.get();
}

void showMainMenu() {
    std::cout << "\n";
    std::cout << "============================================\n";
    std::cout << "         个人待办事项管理系统\n";
    std::cout << "============================================\n";
    std::cout << "    1. 添加待办事项\n";
    std::cout << "    2. 查看全部待办事项\n";
    std::cout << "    3. 修改待办事项\n";
    std::cout << "    4. 删除待办事项\n";
    std::cout << "    5. 标记为已完成\n";
    std::cout << "    6. 搜索待办事项\n";
    std::cout << "    7. 查看统计信息\n";
    std::cout << "    0. 保存并退出\n";
    std::cout << "--------------------------------------------\n";
}

void showTasks(const std::vector<Task>& tasks) {
    if (tasks.empty()) {
        std::cout << "\n  暂无待办事项，请先添加。\n";
        return;
    }

    // 根据实际内容动态计算每列宽度
    size_t idWidth = 4;
    size_t titleWidth = 6;    // UTF-8 中文字符按字节计算，留足空间
    size_t descWidth = 6;
    size_t dateWidth = 10;
    size_t priorityWidth = 8;
    size_t statusWidth = 8;

    for (const auto& t : tasks) {
        idWidth       = std::max(idWidth,       std::to_string(t.id).size() + 2);
        titleWidth    = std::max(titleWidth,    t.title.size() + 2);
        descWidth     = std::max(descWidth,     t.description.size() + 2);
        dateWidth     = std::max(dateWidth,     t.dueDate.size() + 2);
        statusWidth   = std::max(statusWidth,   taskStatusToString(t.status).size() + 2);
    }

    // 限制最小宽度
    titleWidth  = std::max(titleWidth,  size_t(8));
    descWidth   = std::max(descWidth,   size_t(8));
    dateWidth   = std::max(dateWidth,   size_t(12));
    statusWidth = std::max(statusWidth, size_t(10));

    // 分隔线
    size_t totalWidth = idWidth + titleWidth + descWidth + dateWidth + priorityWidth + statusWidth + 5;
    std::string separator(totalWidth, '-');

    std::cout << "\n" << separator << "\n";
    // 表头
    std::cout << std::left
              << std::setw(static_cast<int>(idWidth))       << "ID"
              << std::setw(static_cast<int>(titleWidth))    << "标题"
              << std::setw(static_cast<int>(descWidth))     << "描述"
              << std::setw(static_cast<int>(dateWidth))     << "截止日期"
              << std::setw(static_cast<int>(priorityWidth)) << "优先级"
              << std::setw(static_cast<int>(statusWidth))   << "状态"
              << "\n";
    std::cout << separator << "\n";

    // 数据行
    for (const auto& t : tasks) {
        std::cout << std::left
                  << std::setw(static_cast<int>(idWidth))       << t.id
                  << std::setw(static_cast<int>(titleWidth))    << t.title
                  << std::setw(static_cast<int>(descWidth))     << t.description
                  << std::setw(static_cast<int>(dateWidth))     << t.dueDate
                  << std::setw(static_cast<int>(priorityWidth)) << t.priority
                  << std::setw(static_cast<int>(statusWidth))   << taskStatusToString(t.status)
                  << "\n";
    }
    std::cout << separator << "\n";
    std::cout << "  共 " << tasks.size() << " 条待办事项\n";
}

// 显示单条待办事项的详细信息
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
    std::cout << "\n========== 添加待办事项 ==========\n\n";

    Task task;
    task.id          = getNextTaskId(tasks);
    task.title       = readLine("请输入标题：");
    task.description = readLine("请输入描述：");
    task.dueDate     = readLine("请输入截止日期（如 2026-07-01）：");
    task.priority    = readIntInRange("请输入优先级（1=最高，5=最低）：", 1, 5);
    task.status      = TaskStatus::Pending;

    addTask(tasks, task);
    std::cout << "\n✓ 待办事项添加成功！（ID：" << task.id << "）\n";
}

static void handleEditTask(std::vector<Task>& tasks) {
    std::cout << "\n========== 修改待办事项 ==========\n\n";

    if (tasks.empty()) {
        std::cout << "当前没有待办事项可修改。\n";
        return;
    }

    showTasks(tasks);

    int id = readInt("\n请输入要修改的待办事项 ID：");
    Task* task = findTaskById(tasks, id);
    if (task == nullptr) {
        std::cout << "未找到 ID 为 " << id << " 的待办事项。\n";
        return;
    }

    showTaskDetail(*task);

    std::cout << "\n（直接回车保留原值）\n\n";

    std::string title       = readLine("请输入新标题：");
    std::string description = readLine("请输入新描述：");
    std::string dueDate     = readLine("请输入新截止日期：");
    int priority            = readInt("请输入新优先级（1-5，输入0保留原值）：");

    // 保留原值
    if (title.empty())       title       = task->title;
    if (description.empty()) description = task->description;
    if (dueDate.empty())     dueDate     = task->dueDate;
    if (priority == 0)       priority    = task->priority;

    if (updateTask(*task, title, description, dueDate, priority)) {
        std::cout << "\n✓ 待办事项修改成功！\n";
    } else {
        std::cout << "\n修改失败，请检查输入是否合法。\n";
    }
}

static void handleDeleteTask(std::vector<Task>& tasks) {
    std::cout << "\n========== 删除待办事项 ==========\n\n";

    if (tasks.empty()) {
        std::cout << "当前没有待办事项可删除。\n";
        return;
    }

    showTasks(tasks);

    int id = readInt("\n请输入要删除的待办事项 ID：");
    Task* task = findTaskById(tasks, id);
    if (task == nullptr) {
        std::cout << "未找到 ID 为 " << id << " 的待办事项。\n";
        return;
    }

    // 显示待删除的任务信息
    std::cout << "\n将删除以下待办事项：\n";
    std::cout << "  ID：" << task->id << "\n";
    std::cout << "  标题：" << task->title << "\n";
    std::cout << "  状态：" << taskStatusToString(task->status) << "\n";

    if (!confirm("\n确认删除？")) {
        std::cout << "已取消删除操作。\n";
        return;
    }

    deleteTaskById(tasks, id);
    std::cout << "✓ 待办事项删除成功！\n";
}

static void handleCompleteTask(std::vector<Task>& tasks) {
    std::cout << "\n========== 标记为已完成 ==========\n\n";

    if (tasks.empty()) {
        std::cout << "当前没有待办事项。\n";
        return;
    }

    showTasks(tasks);

    int id = readInt("\n请输入要标记完成的待办事项 ID：");
    Task* task = findTaskById(tasks, id);
    if (task == nullptr) {
        std::cout << "未找到 ID 为 " << id << " 的待办事项。\n";
        return;
    }

    if (markTaskCompleted(*task)) {
        std::cout << "✓「" << task->title << "」已标记为完成！\n";
    } else {
        std::cout << "该待办事项已经是「已完成」状态。\n";
    }
}

static void handleSearchTasks(std::vector<Task>& tasks) {
    std::cout << "\n========== 搜索待办事项 ==========\n\n";
    std::cout << "1. 按标题关键字搜索\n";
    std::cout << "2. 按状态筛选（待完成）\n";
    std::cout << "3. 按状态筛选（已完成）\n";
    std::cout << "4. 按优先级筛选\n";
    int choice = readIntInRange("请选择搜索方式：", 1, 4);

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
            int priority = readIntInRange("请输入要筛选的优先级（1-5）：", 1, 5);
            results = findTasksByPriority(tasks, priority);
            break;
        }
    }

    if (results.empty()) {
        std::cout << "\n未找到匹配的待办事项。\n";
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
    std::cout << "\n========== 统计信息 ==========\n\n";
    std::cout << "待办事项总数：" << countAllTasks(tasks) << "\n";
    std::cout << "待完成数量：" << countTasksByStatus(tasks, TaskStatus::Pending) << "\n";
    std::cout << "已完成数量：" << countTasksByStatus(tasks, TaskStatus::Completed) << "\n";
}

// ========== 主程序循环 ==========

void runApplication(const std::string& dataFilePath) {
    std::vector<Task> tasks;

    // 尝试加载已有数据
    if (!loadTasksFromFile(dataFilePath, tasks)) {
        std::cout << "提示：未找到数据文件 " << dataFilePath
                  << "，将创建新的待办事项列表。\n";
        pauseForUser();
    }

    int choice = -1;
    while (choice != 0) {
        clearScreen();
        showMainMenu();
        choice = readMenuChoice();

        clearScreen();
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
                std::cout << "\n数据已保存到 " << dataFilePath << "，程序退出。\n";
                break;
        }

        if (choice != 0) {
            pauseForUser();
        }
    }
}
