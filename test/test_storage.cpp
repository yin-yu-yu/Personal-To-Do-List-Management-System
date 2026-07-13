/**
 * File storage module unit tests (v2.0)
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
    std::cout << "========== Storage Module Tests (v2.0) ==========\n\n";

    const std::string testFile = "test/test_data.csv";

    // ---- Test 1: Empty list ----
    std::cout << "--- Test 1: Save and load empty ---\n";
    {
        std::vector<Task> emptyTasks;
        check("Save empty list", saveTasksToFile(testFile, emptyTasks));

        std::vector<Task> loadedTasks;
        check("Load empty file", loadTasksFromFile(testFile, loadedTasks));
        check("Loaded list is empty", loadedTasks.empty());
    }

    // ---- Test 2: Save and load with tag field ----
    {
        std::vector<Task> tasks;
        tasks.push_back({1, "Buy items", "Shop desc", "2026-07-15", 3, TaskStatus::Pending, "shopping"});
        tasks.push_back({2, "Work task", "Work desc", "2026-08-01", 1, TaskStatus::Completed, "work"});

        check("Save 2 tasks", saveTasksToFile(testFile, tasks));

        std::vector<Task> loaded;
        check("Load 2 tasks", loadTasksFromFile(testFile, loaded));
        check("Loaded size=2", loaded.size() == 2);
        check("Task1 ID=1", loaded[0].id == 1);
        check("Task1 title correct", loaded[0].title == "Buy items");
        check("Task1 description correct", loaded[0].description == "Shop desc");
        check("Task1 dueDate correct", loaded[0].dueDate == "2026-07-15");
        check("Task1 priority=3", loaded[0].priority == 3);
        check("Task1 status=Pending", loaded[0].status == TaskStatus::Pending);
        check("Task1 tag correct", loaded[0].tag == "shopping");
        check("Task2 ID=2", loaded[1].id == 2);
        check("Task2 status=Completed", loaded[1].status == TaskStatus::Completed);
        check("Task2 tag correct", loaded[1].tag == "work");
    }

    // ---- Test 3: CSV special characters ----
    std::cout << "\n--- Test 2: CSV special characters ---\n";
    {
        std::vector<Task> tasks;
        // Fields with commas and quotes
        tasks.push_back({1, "Apples, Bananas, Oranges", "Go to \"market\" now", "2026-07-01", 2, TaskStatus::Pending, "food,snack"});

        check("Save special chars", saveTasksToFile(testFile, tasks));

        std::vector<Task> loaded;
        check("Load special chars", loadTasksFromFile(testFile, loaded));
        check("Loaded size=1", loaded.size() == 1);
        check("Title with commas restored", loaded[0].title == "Apples, Bananas, Oranges");
        check("Description with quotes restored", loaded[0].description == "Go to \"market\" now");
        check("Tag with comma restored", loaded[0].tag == "food,snack");
    }

    // ---- Test 4: Non-existent file ----
    std::cout << "\n--- Test 3: File error handling ---\n";
    {
        std::vector<Task> tasks;
        check("Load nonexistent file returns false", !loadTasksFromFile("test/nonexistent.csv", tasks));
    }

    // ---- Test 5: Backup and Restore (v2.0) ----
    std::cout << "\n--- Test 4: Backup and Restore ---\n";
    {
        std::vector<Task> tasks;
        tasks.push_back({1, "Backup test", "Test desc", "2026-12-31", 1, TaskStatus::Pending, "test"});
        check("Save original data", saveTasksToFile(testFile, tasks));

        check("Backup original file", backupTasksFile(testFile));

        std::vector<Task> restored;
        std::string backupFile = testFile;
        size_t extPos = backupFile.rfind(".csv");
        backupFile = backupFile.substr(0, extPos) + "_backup.csv";
        check("Restore from backup", restoreTasksFromBackup(backupFile, restored));
        check("Restored size=1", restored.size() == 1);
        check("Restored title matches", restored[0].title == "Backup test");
        check("Restored tag matches", restored[0].tag == "test");

        // Clean up backup file
        std::remove(backupFile.c_str());
    }

    // Clean up test file
    std::remove(testFile.c_str());

    std::cout << "\n========================================\n";
    std::cout << "  Result: " << testsPassed << " passed, "
              << testsFailed << " failed\n";
    std::cout << "========================================\n";

    return testsFailed > 0 ? 1 : 0;
}
