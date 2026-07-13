#include "storage.h"

#include <fstream>
#include <sstream>

static std::string escapeCsvField(const std::string& value) {
    bool needsQuoting = false;
    for (std::size_t i = 0; i < value.size(); ++i) {
        if (value[i] == ',' || value[i] == '"' || value[i] == '\n') {
            needsQuoting = true;
            break;
        }
    }

    if (!needsQuoting) {
        return value;
    }

    std::string result = "\"";
    for (std::size_t i = 0; i < value.size(); ++i) {
        if (value[i] == '"') {
            result += "\"\"";
        } else {
            result += value[i];
        }
    }
    result += "\"";
    return result;
}

static void parseCsvLine(const std::string& line, std::string fields[], int fieldCount) {
    int fieldIndex = 0;
    bool inQuotes = false;
    std::string current;

    for (std::size_t i = 0; i < line.size() && fieldIndex < fieldCount; ++i) {
        char c = line[i];
        if (inQuotes) {
            if (c == '"') {
                if (i + 1 < line.size() && line[i + 1] == '"') {
                    current += '"';
                    ++i;
                } else {
                    inQuotes = false;
                }
            } else {
                current += c;
            }
        } else {
            if (c == '"') {
                inQuotes = true;
            } else if (c == ',') {
                fields[fieldIndex] = current;
                current.clear();
                ++fieldIndex;
            } else {
                current += c;
            }
        }
    }
    if (fieldIndex < fieldCount) {
        fields[fieldIndex] = current;
    }
}

bool loadTasksFromFile(const std::string& filePath, std::vector<Task>& tasks) {
    std::ifstream file(filePath.c_str());
    if (!file.is_open()) {
        return false;
    }

    tasks.clear();
    std::string line;

    if (!std::getline(file, line)) {
        return false;
    }

    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        std::string fields[7];
        parseCsvLine(line, fields, 7);

        Task task;
        task.id = std::atoi(fields[0].c_str());
        task.title = fields[1];
        task.description = fields[2];
        task.dueDate = fields[3];
        task.priority = std::atoi(fields[4].c_str());
        task.status = taskStatusFromString(fields[5]);
        task.tag = fields[6];
        tasks.push_back(task);
    }

    return true;
}

bool saveTasksToFile(const std::string& filePath, const std::vector<Task>& tasks) {
    std::ofstream file(filePath.c_str());
    if (!file.is_open()) {
        return false;
    }

    file << "id,title,description,dueDate,priority,status,tag\n";
    for (std::size_t i = 0; i < tasks.size(); ++i) {
        const Task& task = tasks[i];
        file << task.id << ","
             << escapeCsvField(task.title) << ","
             << escapeCsvField(task.description) << ","
             << escapeCsvField(task.dueDate) << ","
             << task.priority << ","
             << taskStatusToString(task.status) << ","
             << escapeCsvField(task.tag) << "\n";
    }

    return true;
}

bool backupTasksFile(const std::string& sourcePath) {
    std::ifstream src(sourcePath.c_str(), std::ios::binary);
    if (!src.is_open()) {
        return false;
    }

    std::string backupPath = sourcePath;
    size_t extPos = backupPath.rfind(".csv");
    if (extPos != std::string::npos) {
        backupPath = backupPath.substr(0, extPos) + "_backup.csv";
    } else {
        backupPath += "_backup";
    }

    std::ofstream dst(backupPath.c_str(), std::ios::binary);
    if (!dst.is_open()) {
        return false;
    }

    dst << src.rdbuf();
    return true;
}

bool restoreTasksFromBackup(const std::string& backupPath, std::vector<Task>& tasks) {
    return loadTasksFromFile(backupPath, tasks);
}

