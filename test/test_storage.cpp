/**
 * 文件存储模块单元测试
 */
#include "storage.h"
#include <iostream>
#include <cstdio>  // for remove

static int testsPassed = 0;
static int testsFailed = 0;

void check(const std::string& name, bool condition) {
    if (condition) { ++testsPassed; std::cout << "  [PASS] " << name << "\n"; }
    else { ++testsFailed; std::cout << "  [FAIL] " << name << "\n"; }
}

int main() {
    std::cout << "========== 文件存储模块测试 ==========\n\n";

    const std::string testFile = "test/test_data.csv";

    // ---- 测试 1: 保存空列表 ----
    std::cout << "--- 测试组 1: 保存和加载 ---\n";
    {
        std::vector<Task> emptyTasks;
        check("保存空列表", saveTasksToFile(testFile, emptyTasks));

        std::vector<Task> loadedTasks;
        check("加载空文件", loadTasksFromFile(testFile, loadedTasks));
        check("加载后列表为空", loadedTasks.empty());
    }

    // ---- 测试 2: 保存并加载任务 ----
    {
        std::vector<Task> tasks;
        tasks.push_back({1, "测试任务", "测试描述", "2026-07-15", 3, TaskStatus::Pending});
        tasks.push_back({2, "已完成任务", "描述2", "2026-08-01", 1, TaskStatus::Completed});

        check("保存2个任务", saveTasksToFile(testFile, tasks));

        std::vector<Task> loaded;
        check("加载2个任务", loadTasksFromFile(testFile, loaded));
        check("加载后大小=2", loaded.size() == 2);
        check("任务1 ID=1", loaded[0].id == 1);
        check("任务1 标题正确", loaded[0].title == "测试任务");
        check("任务1 描述正确", loaded[0].description == "测试描述");
        check("任务1 日期正确", loaded[0].dueDate == "2026-07-15");
        check("任务1 优先级=3", loaded[0].priority == 3);
        check("任务1 状态=Pending", loaded[0].status == TaskStatus::Pending);
        check("任务2 ID=2", loaded[1].id == 2);
        check("任务2 状态=Completed", loaded[1].status == TaskStatus::Completed);
    }

    // ---- 测试 3: CSV 特殊字符 ----
    std::cout << "\n--- 测试组 2: CSV 特殊字符处理 ---\n";
    {
        std::vector<Task> tasks;
        // 包含逗号、引号的字段
        tasks.push_back({1, "买苹果,香蕉,橘子", "去\"水果店\"购买", "2026-07-01", 2, TaskStatus::Pending});

        check("保存含特殊字符的任务", saveTasksToFile(testFile, tasks));

        std::vector<Task> loaded;
        check("加载含特殊字符的任务", loadTasksFromFile(testFile, loaded));
        check("加载后大小=1", loaded.size() == 1);
        check("标题含逗号正确恢复", loaded[0].title == "买苹果,香蕉,橘子");
        check("描述含引号正确恢复", loaded[0].description == "去\"水果店\"购买");
    }

    // ---- 测试 4: 不存在的文件 ----
    std::cout << "\n--- 测试组 3: 文件不存在 ---\n";
    {
        std::vector<Task> tasks;
        check("加载不存在的文件返回false", !loadTasksFromFile("test/nonexistent.csv", tasks));
    }

    // 清理测试文件
    std::remove(testFile.c_str());

    std::cout << "\n========================================\n";
    std::cout << "  测试结果: " << testsPassed << " 通过, "
              << testsFailed << " 失败\n";
    std::cout << "========================================\n";

    return testsFailed > 0 ? 1 : 0;
}
