#include "ui.h"
#include "storage.h"

#include <algorithm>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <limits>

// ========== ANSI 颜色代码 ==========
#define CLR_RESET   "\033[0m"
#define CLR_BOLD    "\033[1m"
#define CLR_DIM     "\033[2m"
#define CLR_RED     "\033[31m"
#define CLR_GREEN   "\033[32m"
#define CLR_YELLOW  "\033[33m"
#define CLR_CYAN    "\033[36m"
#define CLR_MAGENTA "\033[35m"
#define CLR_WHITE_BOLD "\033[1;37m"

#ifdef _WIN32
#include <windows.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
static void enableVirtualTerminal() {
    // 设置控制台编码为 UTF-8
    system("chcp 65001 > nul");
    SetConsoleOutputCP(65001);
    // 启用 ANSI 转义序列支持
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}
#else
static void enableVirtualTerminal() {}
#endif

// ========== 颜色辅助函数 ==========
static std::string priorityColor(int priority) {
    switch (priority) {
        case 1: return CLR_RED CLR_BOLD;
        case 2: return CLR_MAGENTA;
        case 3: return CLR_YELLOW;
        case 4: return CLR_CYAN;
        case 5: return CLR_GREEN;
        default: return CLR_RESET;
    }
}

static std::string statusColor(TaskStatus status) {
    if (status == TaskStatus::Completed) {
        return CLR_GREEN;
    }
    return CLR_YELLOW;
}

// ========== 内部辅助函数 ==========

static void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static std::string readLine(const std::string& prompt) {
    std::string value;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, value);
        size_t start = value.find_first_not_of(" \t");
        if (start == std::string::npos) {
            std::cout << CLR_RED << "输入不能为空，请重新输入。" << CLR_RESET << "\n";
            continue;
        }
        size_t end = value.find_last_not_of(" \t");
        value = value.substr(start, end - start + 1);
        return value;
    }
}

static int readInt(const std::string& prompt) {
    int value = 0;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            clearInput();
            return value;
        }
        clearInput();
        std::cout << CLR_RED << "输入无效，请输入一个整数。" << CLR_RESET << "\n";
    }
}

static int readIntInRange(const std::string& prompt, int min, int max) {
    int value = 0;
    while (true) {
        value = readInt(prompt);
        if (value >= min && value <= max) {
            return value;
        }
        std::cout << CLR_RED << "请输入 " << min << " 到 " << max << " 之间的数字。" << CLR_RESET << "\n";
    }
}

static int readMenuChoice() {
    return readIntInRange("请选择功能：", 0, 9);
}

static bool confirm(const std::string& message) {
    std::cout << message << " (y/n)：";
    std::string input;
    std::getline(std::cin, input);
    return input == "y" || input == "Y" || input == "yes" || input == "Yes";
}

// ========== 分隔线工具 ==========
static std::string sepLine(int width, char c = '-') {
    return std::string(width, c);
}

// ========== 公共函数 ==========

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseForUser() {
    std::cout << "\n按 Enter 键返回菜单...";
    std::cin.get();
}

void showMainMenu() {
    std::cout << "\n";
    std::cout << CLR_WHITE_BOLD;
    std::cout << "============================================\n";
    std::cout << "         个人待办事项管理系统 v2.0\n";
    std::cout << "============================================\n";
    std::cout << CLR_RESET;
    std::cout << "    1. 添加待办事项\n";
    std::cout << "    2. 查看全部待办事项\n";
    std::cout << "    3. 修改待办事项\n";
    std::cout << "    4. 删除待办事项\n";
    std::cout << "    5. 标记为已完成\n";
    std::cout << "    6. 搜索待办事项\n";
    std::cout << "    7. 查看统计信息\n";
    std::cout << "    8. 排序待办事项\n";
    std::cout << "    9. 备份 / 恢复\n";
    std::cout << "    0. 保存并退出\n";
    std::cout << "--------------------------------------------\n";
}

// ========== 到期提醒 ==========

static void checkDueReminders(const std::vector<Task>& tasks) {
    std::time_t now = std::time(nullptr);
    std::tm* local = std::localtime(&now);
    char todayBuf[11];
    std::strftime(todayBuf, sizeof(todayBuf), "%Y-%m-%d", local);
    std::string today(todayBuf);

    std::vector<const Task*> dueSoon;
    std::vector<const Task*> overdue;

    for (const Task& task : tasks) {
        if (task.status == TaskStatus::Completed) continue;
        if (task.dueDate.empty()) continue;

        if (task.dueDate < today) {
            overdue.push_back(&task);
        } else if (task.dueDate == today) {
            dueSoon.push_back(&task);
        } else {
            // 检查是否在 3 天内
            if (task.dueDate.size() == 10) {
                int y1, m1, d1, y2, m2, d2;
                if (std::sscanf(task.dueDate.c_str(), "%d-%d-%d", &y1, &m1, &d1) == 3 &&
                    std::sscanf(today.c_str(), "%d-%d-%d", &y2, &m2, &d2) == 3) {
                    int days1 = y1 * 365 + m1 * 30 + d1;
                    int days2 = y2 * 365 + m2 * 30 + d2;
                    int diff = days1 - days2;
                    if (diff > 0 && diff <= 3) {
                        dueSoon.push_back(&task);
                    }
                }
            }
        }
    }

    if (overdue.empty() && dueSoon.empty()) return;

    std::cout << "\n" CLR_WHITE_BOLD;
    std::cout << "+" << sepLine(52, '=') << "+\n";
    std::cout << "|  [!!] 到期提醒" << std::string(36, ' ') << "|\n";
    std::cout << "+" << sepLine(52, '=') << "+\n" << CLR_RESET;

    if (!overdue.empty()) {
        std::cout << CLR_RED CLR_BOLD << "\n  [X] 已逾期 (" << overdue.size() << " 项任务):" << CLR_RESET << "\n";
        for (const Task* t : overdue) {
            std::cout << "      #" << t->id << " " << t->title << "  (截止: " << t->dueDate << ")\n";
        }
    }

    if (!dueSoon.empty()) {
        std::cout << CLR_YELLOW CLR_BOLD << "\n  [!] 即将到期 (" << dueSoon.size() << " 项任务):" << CLR_RESET << "\n";
        for (const Task* t : dueSoon) {
            std::cout << "      #" << t->id << " " << t->title << "  (截止: " << t->dueDate << ")\n";
        }
    }

    std::cout << "\n";
    pauseForUser();
}

// ========== 任务列表显示 ==========

void showTasks(const std::vector<Task>& tasks) {
    if (tasks.empty()) {
        std::cout << CLR_DIM << "\n  暂无待办事项，请先添加。\n" << CLR_RESET;
        return;
    }

    // 动态计算每列宽度
    size_t idWidth = 4;
    size_t titleWidth = 6;
    size_t descWidth = 6;
    size_t dateWidth = 10;
    size_t priorityWidth = 8;
    size_t statusWidth = 8;
    size_t tagWidth = 5;

    for (const auto& t : tasks) {
        idWidth       = std::max(idWidth,       std::to_string(t.id).size() + 2);
        titleWidth    = std::max(titleWidth,    t.title.size() + 2);
        descWidth     = std::max(descWidth,     t.description.size() + 2);
        dateWidth     = std::max(dateWidth,     t.dueDate.size() + 2);
        tagWidth      = std::max(tagWidth,      t.tag.size() + 2);
        statusWidth   = std::max(statusWidth,   taskStatusToString(t.status).size() + 2);
    }

    titleWidth  = std::max(titleWidth,  size_t(8));
    descWidth   = std::max(descWidth,   size_t(8));
    dateWidth   = std::max(dateWidth,   size_t(12));
    statusWidth = std::max(statusWidth, size_t(11));
    tagWidth    = std::max(tagWidth,    size_t(7));

    size_t totalWidth = idWidth + titleWidth + descWidth + dateWidth + priorityWidth + statusWidth + tagWidth + 6;

    std::string separator = "+" + sepLine(static_cast<int>(totalWidth), '-') + "+";

    std::cout << "\n" CLR_WHITE_BOLD;
    std::cout << separator << "\n";

    // 表头
    std::cout << "|" << std::left
              << std::setw(static_cast<int>(idWidth))       << "ID"
              << std::setw(static_cast<int>(titleWidth))    << "标题"
              << std::setw(static_cast<int>(descWidth))     << "描述"
              << std::setw(static_cast<int>(dateWidth))     << "截止日期"
              << std::setw(static_cast<int>(priorityWidth)) << "优先级"
              << std::setw(static_cast<int>(statusWidth))   << "状态"
              << std::setw(static_cast<int>(tagWidth))      << "标签"
              << "|\n";

    std::cout << separator << "\n" CLR_RESET;

    // 数据行
    for (const auto& t : tasks) {
        std::cout << "|" << std::left
                  << std::setw(static_cast<int>(idWidth))       << t.id
                  << CLR_RESET
                  << std::setw(static_cast<int>(titleWidth))    << t.title
                  << std::setw(static_cast<int>(descWidth))     << t.description
                  << std::setw(static_cast<int>(dateWidth))     << t.dueDate
                  << priorityColor(t.priority)
                  << std::setw(static_cast<int>(priorityWidth)) << t.priority
                  << CLR_RESET
                  << statusColor(t.status)
                  << std::setw(static_cast<int>(statusWidth))   << taskStatusToString(t.status)
                  << CLR_RESET
                  << CLR_DIM
                  << std::setw(static_cast<int>(tagWidth))      << t.tag
                  << CLR_RESET
                  << "|\n";
    }

    std::cout << CLR_WHITE_BOLD << separator << "\n" CLR_RESET;
    std::cout << "  共 " << tasks.size() << " 项任务\n";
}

// ========== 任务详情 ==========

static void showTaskDetail(const Task& task) {
    std::cout << "\n" CLR_WHITE_BOLD << "--- 任务详情 ---" CLR_RESET << "\n";
    std::cout << "ID：       " << task.id << "\n";
    std::cout << "标题：     " << task.title << "\n";
    std::cout << "描述：     " << task.description << "\n";
    std::cout << "截止日期： " << task.dueDate << "\n";
    std::cout << "优先级：   " << priorityColor(task.priority) << task.priority << CLR_RESET << "\n";
    std::cout << "状态：     " << statusColor(task.status) << taskStatusToString(task.status) << CLR_RESET << "\n";
    std::cout << "标签：     " << task.tag << "\n";
}

// ========== 功能处理函数 ==========

static void handleAddTask(std::vector<Task>& tasks) {
    std::cout << "\n========== 添加待办事项 ==========\n\n";

    Task task;
    task.id          = getNextTaskId(tasks);
    task.title       = readLine("标题：");
    task.description = readLine("描述：");
    task.dueDate     = readLine("截止日期（如 2026-07-01）：");
    task.priority    = readIntInRange("优先级（1=最高，5=最低）：", 1, 5);
    task.tag         = readLine("标签（如 工作、学习、个人）：");
    task.status      = TaskStatus::Pending;

    addTask(tasks, task);
    std::cout << "\n" CLR_GREEN << "[OK] 任务添加成功！ID：" << task.id << CLR_RESET << "\n";
}

static void handleEditTask(std::vector<Task>& tasks) {
    std::cout << "\n========== 修改待办事项 ==========\n\n";

    if (tasks.empty()) {
        std::cout << CLR_YELLOW << "当前没有待办事项可供修改。\n" << CLR_RESET;
        return;
    }

    showTasks(tasks);

    int id = readInt("\n请输入要修改的任务 ID：");
    Task* task = findTaskById(tasks, id);
    if (task == nullptr) {
        std::cout << CLR_RED << "未找到 ID 为 " << id << " 的任务。\n" << CLR_RESET;
        return;
    }

    showTaskDetail(*task);

    std::cout << "\n（直接回车保留原值，优先级输入 0 保留原值）\n";
    std::string title       = readLine("新标题：");
    std::string description = readLine("新描述：");
    std::string dueDate     = readLine("新截止日期：");
    int priority            = readIntInRange("新优先级（1-5，0=不修改）：", 0, 5);
    std::string tag         = readLine("新标签：");

    if (title.empty())       title       = task->title;
    if (description.empty()) description = task->description;
    if (dueDate.empty())     dueDate     = task->dueDate;
    if (priority == 0)       priority    = task->priority;
    if (tag.empty())         tag         = task->tag;

    if (updateTask(*task, title, description, dueDate, priority, tag)) {
        std::cout << "\n" CLR_GREEN << "[OK] 任务修改成功！\n" << CLR_RESET;
    } else {
        std::cout << "\n" CLR_RED << "修改失败，请检查输入是否合法。\n" << CLR_RESET;
    }
}

static void handleDeleteTask(std::vector<Task>& tasks) {
    std::cout << "\n========== 删除待办事项 ==========\n\n";

    if (tasks.empty()) {
        std::cout << CLR_YELLOW << "当前没有待办事项可供删除。\n" << CLR_RESET;
        return;
    }

    showTasks(tasks);

    int id = readInt("\n请输入要删除的任务 ID：");
    Task* task = findTaskById(tasks, id);
    if (task == nullptr) {
        std::cout << CLR_RED << "未找到 ID 为 " << id << " 的任务。\n" << CLR_RESET;
        return;
    }

    showTaskDetail(*task);

    if (!confirm("\n" CLR_RED "确认删除？" CLR_RESET)) {
        std::cout << "已取消删除操作。\n";
        return;
    }

    deleteTaskById(tasks, id);
    std::cout << CLR_GREEN << "[OK] 任务删除成功！\n" << CLR_RESET;
}

static void handleCompleteTask(std::vector<Task>& tasks) {
    std::cout << "\n========== 标记为已完成 ==========\n\n";

    if (tasks.empty()) {
        std::cout << CLR_YELLOW << "当前没有待办事项。\n" << CLR_RESET;
        return;
    }

    showTasks(tasks);

    int id = readInt("\n请输入要标记为已完成的任务 ID：");
    Task* task = findTaskById(tasks, id);
    if (task == nullptr) {
        std::cout << CLR_RED << "未找到 ID 为 " << id << " 的任务。\n" << CLR_RESET;
        return;
    }

    showTaskDetail(*task);

    if (markTaskCompleted(*task)) {
        std::cout << CLR_GREEN << "[OK] \"" << task->title << "\" 已标记为已完成！\n" << CLR_RESET;
    } else {
        std::cout << CLR_YELLOW << "该任务已经是「已完成」状态。\n" << CLR_RESET;
    }
}

static void handleSearchTasks(std::vector<Task>& tasks) {
    std::cout << "\n========== 搜索待办事项 ==========\n\n";
    std::cout << "  1. 按标题关键字搜索\n";
    std::cout << "  2. 按状态筛选（待完成）\n";
    std::cout << "  3. 按状态筛选（已完成）\n";
    std::cout << "  4. 按优先级筛选\n";
    std::cout << "  5. 按标签搜索\n";

    int choice = readIntInRange("请选择搜索方式：", 1, 5);

    std::vector<Task*> results;

    switch (choice) {
        case 1: {
            std::string keyword = readLine("请输入关键字：");
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
        case 5: {
            std::string tag = readLine("请输入标签关键字：");
            results = findTasksByTag(tasks, tag);
            break;
        }
    }

    if (results.empty()) {
        std::cout << CLR_YELLOW << "\n未找到匹配的待办事项。\n" << CLR_RESET;
        return;
    }

    std::cout << "\n" CLR_GREEN << "找到 " << results.size() << " 项匹配任务\n" << CLR_RESET;
    std::vector<Task> matched;
    for (const Task* t : results) {
        matched.push_back(*t);
    }
    showTasks(matched);
}

static void handleShowStats(const std::vector<Task>& tasks) {
    int total     = countAllTasks(tasks);
    int pending   = countTasksByStatus(tasks, TaskStatus::Pending);
    int completed = countTasksByStatus(tasks, TaskStatus::Completed);

    std::cout << "\n" CLR_WHITE_BOLD;
    std::cout << "+" << sepLine(32, '=') << "+\n";
    std::cout << "|        统计信息              |\n";
    std::cout << "+" << sepLine(32, '=') << "+\n" CLR_RESET;
    std::cout << "\n";
    std::cout << "  任务总数：     " CLR_WHITE_BOLD << total << CLR_RESET << "\n";
    std::cout << "  待完成：       " CLR_YELLOW << pending << CLR_RESET;
    if (total > 0) {
        std::cout << " (" << (pending * 100 / total) << "%)";
    }
    std::cout << "\n";
    std::cout << "  已完成：       " CLR_GREEN << completed << CLR_RESET;
    if (total > 0) {
        std::cout << " (" << (completed * 100 / total) << "%)";
    }
    std::cout << "\n";

    if (total > 0 && completed == total) {
        std::cout << "\n  " CLR_GREEN CLR_BOLD << "*** 所有任务已完成！太棒了！***" CLR_RESET << "\n";
    }
}

// ========== 2.0 新功能 ==========

static void handleSortTasks(std::vector<Task>& tasks) {
    if (tasks.empty()) {
        std::cout << CLR_YELLOW << "\n没有可排序的任务。\n" << CLR_RESET;
        return;
    }

    std::cout << "\n========== 排序待办事项 ==========\n\n";
    std::cout << "  1. 按优先级排序（高 -> 低）\n";
    std::cout << "  2. 按优先级排序（低 -> 高）\n";
    std::cout << "  3. 按截止日期排序（早 -> 晚）\n";
    std::cout << "  4. 按截止日期排序（晚 -> 早）\n";

    int choice = readIntInRange("请选择排序方式：", 1, 4);

    switch (choice) {
        case 1:
            sortTasksByPriority(tasks, true);
            std::cout << CLR_GREEN << "\n[OK] 已按优先级从高到低排序。\n" << CLR_RESET;
            break;
        case 2:
            sortTasksByPriority(tasks, false);
            std::cout << CLR_GREEN << "\n[OK] 已按优先级从低到高排序。\n" << CLR_RESET;
            break;
        case 3:
            sortTasksByDueDate(tasks, true);
            std::cout << CLR_GREEN << "\n[OK] 已按截止日期从早到晚排序。\n" << CLR_RESET;
            break;
        case 4:
            sortTasksByDueDate(tasks, false);
            std::cout << CLR_GREEN << "\n[OK] 已按截止日期从晚到早排序。\n" << CLR_RESET;
            break;
    }

    showTasks(tasks);
}

static void handleBackupRestore(std::vector<Task>& tasks) {
    std::cout << "\n========== 备份 / 恢复 ==========\n\n";
    std::cout << "  1. 备份当前数据\n";
    std::cout << "  2. 从备份恢复数据\n";

    int choice = readIntInRange("请选择操作：", 1, 2);

    switch (choice) {
        case 1: {
            if (backupTasksFile("data/tasks.csv")) {
                std::cout << CLR_GREEN << "\n[OK] 数据已备份到 data/tasks_backup.csv\n" << CLR_RESET;
            } else {
                std::cout << CLR_RED << "\n备份失败，请检查 data/tasks.csv 是否存在。\n" << CLR_RESET;
            }
            break;
        }
        case 2: {
            if (!confirm("\n" CLR_YELLOW "此操作将覆盖当前数据，确认继续？" CLR_RESET)) {
                std::cout << "已取消恢复操作。\n";
                return;
            }
            if (restoreTasksFromBackup("data/tasks_backup.csv", tasks)) {
                std::cout << CLR_GREEN << "\n[OK] 数据已从备份恢复！（共 " << tasks.size() << " 项任务）\n" << CLR_RESET;
            } else {
                std::cout << CLR_RED << "\n恢复失败，请检查 data/tasks_backup.csv 是否存在。\n" << CLR_RESET;
            }
            break;
        }
    }
}

// ========== 主程序循环 ==========

void runApplication(const std::string& dataFilePath) {
    enableVirtualTerminal();

    std::vector<Task> tasks;

    // 从文件加载任务
    if (!loadTasksFromFile(dataFilePath, tasks)) {
        std::cout << CLR_YELLOW << "提示：未找到数据文件 " << dataFilePath
                  << "，将以空列表启动。\n" << CLR_RESET;
        pauseForUser();
    }

    // 启动时显示到期提醒
    if (!tasks.empty()) {
        checkDueReminders(tasks);
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
            case 8:
                handleSortTasks(tasks);
                break;
            case 9:
                handleBackupRestore(tasks);
                break;
            case 0:
                saveTasksToFile(dataFilePath, tasks);
                std::cout << CLR_GREEN << "\n数据已保存到 " << dataFilePath
                          << "。再见！\n" << CLR_RESET;
                break;
        }

        if (choice != 0) {
            pauseForUser();
        }
    }
}
