/**
 * Core business logic unit tests
 * Covers all functions in task.cpp (v2.0)
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
    std::cout << "========== Core Logic Tests (v2.0) ==========\n\n";

    // ---- Test 1: Add Tasks ----
    std::cout << "--- Test 1: Add Tasks ---\n";
    {
        std::vector<Task> tasks;
        check("Empty list getNextTaskId = 1", getNextTaskId(tasks) == 1);

        Task t1;
        t1.id = 1;
        t1.title = "Buy groceries";
        t1.description = "Vegetables and fruit";
        t1.dueDate = "2026-07-05";
        t1.priority = 2;
        t1.status = TaskStatus::Pending;
        t1.tag = "shopping";

        addTask(tasks, t1);
        check("After add size=1", tasks.size() == 1);
        check("getNextTaskId = 2", getNextTaskId(tasks) == 2);

        Task t2;
        t2.id = 2;
        t2.title = "Finish report";
        t2.description = "Write team project summary";
        t2.dueDate = "2026-07-10";
        t2.priority = 1;
        t2.status = TaskStatus::Pending;
        t2.tag = "work";
        addTask(tasks, t2);
        check("Add second task size=2", tasks.size() == 2);
        check("getNextTaskId = 3", getNextTaskId(tasks) == 3);
    }

    // ---- Test 2: Query Tasks ----
    std::cout << "\n--- Test 2: Query Tasks ---\n";
    {
        std::vector<Task> tasks;
        Task t1 = {1, "Buy groceries", "Food", "2026-07-05", 2, TaskStatus::Pending, "shopping"};
        Task t2 = {2, "Finish report", "Report", "2026-07-10", 1, TaskStatus::Completed, "work"};
        Task t3 = {3, "Exercise", "Run", "2026-07-06", 3, TaskStatus::Pending, "health"};
        tasks.push_back(t1);
        tasks.push_back(t2);
        tasks.push_back(t3);

        check("findTaskById ID=1 found", findTaskById(tasks, 1) != nullptr);
        check("findTaskById matches title", findTaskById(tasks, 1)->title == "Buy groceries");
        check("findTaskById ID=99 returns null", findTaskById(tasks, 99) == nullptr);

        auto titleResults = findTasksByTitle(tasks, "o");
        check("Title search 'o' found 2", titleResults.size() == 2);

        auto titleResults2 = findTasksByTitle(tasks, "report");
        check("Title search 'report' found 1", titleResults2.size() == 1);

        auto titleResults3 = findTasksByTitle(tasks, "nonexistent");
        check("Title search 'nonexistent' found 0", titleResults3.empty());

        auto pendingResults = findTasksByStatus(tasks, TaskStatus::Pending);
        check("Status Pending found 2", pendingResults.size() == 2);

        auto completedResults = findTasksByStatus(tasks, TaskStatus::Completed);
        check("Status Completed found 1", completedResults.size() == 1);

        auto priorityResults = findTasksByPriority(tasks, 3);
        check("Priority 3 found 1", priorityResults.size() == 1);

        // Tag search tests (v2.0)
        auto tagResults = findTasksByTag(tasks, "work");
        check("Tag search 'work' found 1", tagResults.size() == 1);

        auto tagResults2 = findTasksByTag(tasks, "shopping");
        check("Tag search 'shopping' found 1", tagResults2.size() == 1);

        auto tagResults3 = findTasksByTag(tasks, "h");
        check("Tag search 'h' found 2 (shopping, health)", tagResults3.size() == 2);

        auto tagResults4 = findTasksByTag(tasks, "nonexistent");
        check("Tag search 'nonexistent' found 0", tagResults4.empty());

        auto tagResults5 = findTasksByTag(tasks, "");
        check("Tag search empty returns empty", tagResults5.empty());
    }

    // ---- Test 3: Update Task ----
    std::cout << "\n--- Test 3: Update Task ---\n";
    {
        std::vector<Task> tasks;
        Task t1 = {1, "Old title", "Old desc", "2026-01-01", 3, TaskStatus::Pending, "old"};
        tasks.push_back(t1);

        bool result = updateTask(tasks[0], "New title", "New desc", "2026-12-31", 1, "new");
        check("updateTask success returns true", result);
        check("Title updated", tasks[0].title == "New title");
        check("Description updated", tasks[0].description == "New desc");
        check("DueDate updated", tasks[0].dueDate == "2026-12-31");
        check("Priority updated", tasks[0].priority == 1);
        check("Tag updated", tasks[0].tag == "new");

        bool result2 = updateTask(tasks[0], "", "desc", "date", 3, "tag");
        check("updateTask empty title returns false", !result2);

        bool result3 = updateTask(tasks[0], "title", "desc", "date", 6, "tag");
        check("updateTask priority 6 (>5) returns false", !result3);

        bool result4 = updateTask(tasks[0], "title", "desc", "date", 0, "tag");
        check("updateTask priority 0 (<1) returns false", !result4);
    }

    // ---- Test 4: Mark Status ----
    std::cout << "\n--- Test 4: Status Mark ---\n";
    {
        Task pending = {1, "Task", "Desc", "2026-07-01", 1, TaskStatus::Pending, "test"};
        check("markTaskCompleted returns true", markTaskCompleted(pending));
        check("Status changed to Completed", pending.status == TaskStatus::Completed);
        check("markTaskCompleted again returns false", !markTaskCompleted(pending));

        Task completed = {2, "Done task", "Desc", "2026-07-01", 1, TaskStatus::Completed, "test"};
        check("markTaskPending returns true", markTaskPending(completed));
        check("Status changed to Pending", completed.status == TaskStatus::Pending);
        check("markTaskPending again returns false", !markTaskPending(completed));
    }

    // ---- Test 5: Delete Task ----
    std::cout << "\n--- Test 5: Delete Task ---\n";
    {
        std::vector<Task> tasks;
        Task t1 = {1, "Task1", "Desc1", "2026-01-01", 1, TaskStatus::Pending, "a"};
        Task t2 = {2, "Task2", "Desc2", "2026-01-02", 2, TaskStatus::Pending, "b"};
        Task t3 = {3, "Task3", "Desc3", "2026-01-03", 3, TaskStatus::Pending, "c"};
        tasks.push_back(t1);
        tasks.push_back(t2);
        tasks.push_back(t3);

        check("deleteTaskById ID=2 returns true", deleteTaskById(tasks, 2));
        check("After delete size=2", tasks.size() == 2);
        check("Deleted ID=2 returns null", findTaskById(tasks, 2) == nullptr);
        check("ID=1 and ID=3 still exist", findTaskById(tasks, 1) != nullptr && findTaskById(tasks, 3) != nullptr);
        check("deleteTaskById ID=99 returns false", !deleteTaskById(tasks, 99));
    }

    // ---- Test 6: Sorting ----
    std::cout << "\n--- Test 6: Sorting ---\n";
    {
        std::vector<Task> tasks;
        tasks.push_back({1, "Task C", "", "2026-03-01", 3, TaskStatus::Pending, ""});
        tasks.push_back({2, "Task A", "", "2026-01-01", 1, TaskStatus::Pending, ""});
        tasks.push_back({3, "Task B", "", "2026-02-01", 2, TaskStatus::Pending, ""});

        sortTasksByPriority(tasks, true);
        check("Sort by priority asc: [0].priority=1", tasks[0].priority == 1);
        check("Sort by priority asc: [2].priority=3", tasks[2].priority == 3);

        sortTasksByDueDate(tasks, true);
        check("Sort by date asc: [0].date=2026-01-01", tasks[0].dueDate == "2026-01-01");
        check("Sort by date asc: [2].date=2026-03-01", tasks[2].dueDate == "2026-03-01");

        sortTasksByDueDate(tasks, false);
        check("Sort by date desc: [0].date=2026-03-01", tasks[0].dueDate == "2026-03-01");
    }

    // ---- Test 7: Statistics ----
    std::cout << "\n--- Test 7: Statistics ---\n";
    {
        std::vector<Task> tasks;
        tasks.push_back({1, "Task1", "", "", 1, TaskStatus::Pending, ""});
        tasks.push_back({2, "Task2", "", "", 1, TaskStatus::Completed, ""});
        tasks.push_back({3, "Task3", "", "", 1, TaskStatus::Pending, ""});

        check("countAllTasks = 3", countAllTasks(tasks) == 3);
        check("countTasksByStatus Pending = 2", countTasksByStatus(tasks, TaskStatus::Pending) == 2);
        check("countTasksByStatus Completed = 1", countTasksByStatus(tasks, TaskStatus::Completed) == 1);
    }

    // ---- Test 8: Utility Functions ----
    std::cout << "\n--- Test 8: Utility Functions ---\n";
    {
        check("taskStatusToString(Pending)", taskStatusToString(TaskStatus::Pending) == "Pending");
        check("taskStatusToString(Completed)", taskStatusToString(TaskStatus::Completed) == "Completed");
        check("taskStatusFromString('Pending')", taskStatusFromString("Pending") == TaskStatus::Pending);
        check("taskStatusFromString('Completed')", taskStatusFromString("Completed") == TaskStatus::Completed);
        check("taskStatusFromString('unknown') -> Pending", taskStatusFromString("unknown") == TaskStatus::Pending);
    }

    // ---- Summary ----
    std::cout << "\n========================================\n";
    std::cout << "  Result: " << testsPassed << " passed, "
              << testsFailed << " failed\n";
    std::cout << "========================================\n";

    return testsFailed > 0 ? 1 : 0;
}
