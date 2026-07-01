#ifndef STORAGE_H
#define STORAGE_H

#include "task.h"

#include <string>
#include <vector>

bool loadTasksFromFile(const std::string& filePath, std::vector<Task>& tasks);
bool saveTasksToFile(const std::string& filePath, const std::vector<Task>& tasks);

#endif

