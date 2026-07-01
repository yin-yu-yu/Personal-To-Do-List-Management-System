/**
 * 核心业务逻辑单元测试
 * 测试 task.cpp 中的所有函数
 */
#include "task.h"
#include <iostream>
#include <cassert>
#include <string>

static int testsPassed = 0;
static int testsFailed = 0;

void check(const std::string& name, bool condition) {
    if (condition) {
        ++testsPassed;
        std::cout << "  [PASS] " << name << "\n";
    } else {
        ++testsFailed;
        std::cout << "  [FAIL] " << name << "\n";
    }
}

int main() {
    std::cout << "========== 待办事项核心逻辑测试 ==========\n\n";

    // ---- 测试 1: 添加任务 ----
    std::cout << "--- 测试组 1: 添加任务 ---\n";
    {
        std::vector<Task> tasks;
        check("空列表 getNextTaskId 返回 1", getNextTaskId(tasks) == 1);

        Task t1;
        t1.id = 1;
        t1.title = "购买食材";
        t1.description = "买蔬菜和水果";
        t1.dueDate = "2026-07-05";
        t1.priority = 2;
        t1.status = TaskStatus::Pending;

        addTask(tasks, t1);
        check("添加任务后列表大小=1", tasks.size() == 1);
        check("getNextTaskId 返回 2", getNextTaskId(tasks) == 2);

        Task t2;
        t2.id = 2;
        t2.title = "完成报告";
        t2.description = "写小组项目报告";
        t2.dueDate = "2026-07-10";
        t2.priority = 1;
        t2.status = TaskStatus::Pending;
        addTask(tasks, t2);
        check("添加第二个任务后列表大小=2", tasks.size() == 2);
        check("getNextTaskId 返回 3", getNextTaskId(tasks) == 3);
    }

    // ---- 测试 2: 查找任务 ----
    std::cout << "\n--- 测试组 2: 查找任务 ---\n";
    {
        std::vector<Task> tasks;
        Task t1 = {1, "购买食材", "买蔬菜", "2026-07-05", 2, TaskStatus::Pending};
        Task t2 = {2, "完成报告", "写报告", "2026-07-10", 1, TaskStatus::Completed};
        Task t3 = {3, "买菜做饭", "做饭", "2026-07-06", 3, TaskStatus::Pending};
        tasks.push_back(t1);
        tasks.push_back(t2);
        tasks.push_back(t3);

        check("findTaskById 找到ID=1", findTaskById(tasks, 1) != nullptr);
        check("findTaskById 找到的任务标题正确", findTaskById(tasks, 1)->title == "购买食材");
        check("findTaskById 返回nullptr for ID=99", findTaskById(tasks, 99) == nullptr);

        auto titleResults = findTasksByTitle(tasks, "买");
        check("按标题搜索'买'找到2个", titleResults.size() == 2);

        auto titleResults2 = findTasksByTitle(tasks, "报告");
        check("按标题搜索'报告'找到1个", titleResults2.size() == 1);

        auto titleResults3 = findTasksByTitle(tasks, "不存在");
        check("按标题搜索'不存在'找到0个", titleResults3.empty());

        auto pendingResults = findTasksByStatus(tasks, TaskStatus::Pending);
        check("按状态Pending找到2个", pendingResults.size() == 2);

        auto completedResults = findTasksByStatus(tasks, TaskStatus::Completed);
        check("按状态Completed找到1个", completedResults.size() == 1);

        auto priorityResults = findTasksByPriority(tasks, 3);
        check("按优先级3找到1个", priorityResults.size() == 1);
    }

    // ---- 测试 3: 修改任务 ----
    std::cout << "\n--- 测试组 3: 修改任务 ---\n";
    {
        std::vector<Task> tasks;
        Task t1 = {1, "旧标题", "旧描述", "2026-01-01", 3, TaskStatus::Pending};
        tasks.push_back(t1);

        bool result = updateTask(tasks[0], "新标题", "新描述", "2026-12-31", 1);
        check("updateTask 成功返回true", result);
        check("标题已更新", tasks[0].title == "新标题");
        check("描述已更新", tasks[0].description == "新描述");
        check("截止日期已更新", tasks[0].dueDate == "2026-12-31");
        check("优先级已更新", tasks[0].priority == 1);

        bool result2 = updateTask(tasks[0], "", "desc", "date", 3);
        check("updateTask 空标题返回false", !result2);

        bool result3 = updateTask(tasks[0], "title", "desc", "date", 6);
        check("updateTask 优先级6(>5)返回false", !result3);

        bool result4 = updateTask(tasks[0], "title", "desc", "date", 0);
        check("updateTask 优先级0(<1)返回false", !result4);
    }

    // ---- 测试 4: 完成/待完成状态 ----
    std::cout << "\n--- 测试组 4: 状态变更 ---\n";
    {
        Task pending = {1, "任务", "描述", "2026-07-01", 1, TaskStatus::Pending};
        check("markTaskCompleted 返回true", markTaskCompleted(pending));
        check("状态变为Completed", pending.status == TaskStatus::Completed);
        check("markTaskCompleted 再次调用返回false", !markTaskCompleted(pending));

        Task completed = {2, "已完成任务", "描述", "2026-07-01", 1, TaskStatus::Completed};
        check("markTaskPending 返回true", markTaskPending(completed));
        check("状态变为Pending", completed.status == TaskStatus::Pending);
        check("markTaskPending 再次调用返回false", !markTaskPending(completed));
    }

    // ---- 测试 5: 删除任务 ----
    std::cout << "\n--- 测试组 5: 删除任务 ---\n";
    {
        std::vector<Task> tasks;
        Task t1 = {1, "任务1", "描述1", "2026-01-01", 1, TaskStatus::Pending};
        Task t2 = {2, "任务2", "描述2", "2026-01-02", 2, TaskStatus::Pending};
        Task t3 = {3, "任务3", "描述3", "2026-01-03", 3, TaskStatus::Pending};
        tasks.push_back(t1);
        tasks.push_back(t2);
        tasks.push_back(t3);

        check("deleteTaskById ID=2 返回true", deleteTaskById(tasks, 2));
        check("删除后列表大小=2", tasks.size() == 2);
        check("删除后 findTaskById ID=2 返回nullptr", findTaskById(tasks, 2) == nullptr);
        check("ID=1 和 ID=3 仍然存在", findTaskById(tasks, 1) != nullptr && findTaskById(tasks, 3) != nullptr);
        check("deleteTaskById ID=99 返回false", !deleteTaskById(tasks, 99));
    }

    // ---- 测试 6: 排序 ----
    std::cout << "\n--- 测试组 6: 排序 ---\n";
    {
        std::vector<Task> tasks;
        tasks.push_back({1, "C任务", "", "2026-03-01", 3, TaskStatus::Pending});
        tasks.push_back({2, "A任务", "", "2026-01-01", 1, TaskStatus::Pending});
        tasks.push_back({3, "B任务", "", "2026-02-01", 2, TaskStatus::Pending});

        sortTasksByPriority(tasks, true);
        check("按优先级升序后第1个是priority=1", tasks[0].priority == 1);
        check("按优先级升序后第3个是priority=3", tasks[2].priority == 3);

        sortTasksByDueDate(tasks, true);
        check("按日期升序后第1个是2026-01-01", tasks[0].dueDate == "2026-01-01");
        check("按日期升序后第3个是2026-03-01", tasks[2].dueDate == "2026-03-01");

        sortTasksByDueDate(tasks, false);
        check("按日期降序后第1个是2026-03-01", tasks[0].dueDate == "2026-03-01");
    }

    // ---- 测试 7: 统计 ----
    std::cout << "\n--- 测试组 7: 统计 ---\n";
    {
        std::vector<Task> tasks;
        tasks.push_back({1, "任务1", "", "", 1, TaskStatus::Pending});
        tasks.push_back({2, "任务2", "", "", 1, TaskStatus::Completed});
        tasks.push_back({3, "任务3", "", "", 1, TaskStatus::Pending});

        check("countAllTasks = 3", countAllTasks(tasks) == 3);
        check("countTasksByStatus Pending = 2", countTasksByStatus(tasks, TaskStatus::Pending) == 2);
        check("countTasksByStatus Completed = 1", countTasksByStatus(tasks, TaskStatus::Completed) == 1);
    }

    // ---- 测试 8: 工具函数 ----
    std::cout << "\n--- 测试组 8: 工具函数 ---\n";
    {
        check("taskStatusToString(Pending)", taskStatusToString(TaskStatus::Pending) == "Pending");
        check("taskStatusToString(Completed)", taskStatusToString(TaskStatus::Completed) == "Completed");
        check("taskStatusFromString('Pending')", taskStatusFromString("Pending") == TaskStatus::Pending);
        check("taskStatusFromString('Completed')", taskStatusFromString("Completed") == TaskStatus::Completed);
        check("taskStatusFromString('unknown') -> Pending", taskStatusFromString("unknown") == TaskStatus::Pending);
    }

    // ---- 结果 ----
    std::cout << "\n========================================\n";
    std::cout << "  测试结果: " << testsPassed << " 通过, "
              << testsFailed << " 失败\n";
    std::cout << "========================================\n";

    return testsFailed > 0 ? 1 : 0;
}
