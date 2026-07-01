#include "storage.h"

#include <fstream>
#include <sstream>

static std::string escapeCsv(const std::string& value) {
    std::string result = value;
    for (std::size_t i = 0; i < result.size(); ++i) {
        if (result[i] == ',') {
            result[i] = ' ';
        }
    }
    return result;
}

bool loadTasksFromFile(const std::string& filePath, std::vector<Task>& tasks) {
    std::ifstream file(filePath.c_str());
    if (!file.is_open()) {
        return false;
    }

    tasks.clear();
    std::string line;
    std::getline(file, line);

    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        std::stringstream ss(line);
        std::string id;
        std::string title;
        std::string description;
        std::string dueDate;
        std::string priority;
        std::string status;

        std::getline(ss, id, ',');
        std::getline(ss, title, ',');
        std::getline(ss, description, ',');
        std::getline(ss, dueDate, ',');
        std::getline(ss, priority, ',');
        std::getline(ss, status, ',');

        Task task;
        task.id = std::atoi(id.c_str());
        task.title = title;
        task.description = description;
        task.dueDate = dueDate;
        task.priority = std::atoi(priority.c_str());
        task.status = taskStatusFromString(status);
        tasks.push_back(task);
    }

    return true;
}

bool saveTasksToFile(const std::string& filePath, const std::vector<Task>& tasks) {
    std::ofstream file(filePath.c_str());
    if (!file.is_open()) {
        return false;
    }

    file << "id,title,description,dueDate,priority,status\n";
    for (const Task& task : tasks) {
        file << task.id << ","
             << escapeCsv(task.title) << ","
             << escapeCsv(task.description) << ","
             << escapeCsv(task.dueDate) << ","
             << task.priority << ","
             << taskStatusToString(task.status) << "\n";
    }

    return true;
}

