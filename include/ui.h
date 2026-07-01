#ifndef UI_H
#define UI_H

#include "task.h"

#include <string>
#include <vector>

void runApplication(const std::string& dataFilePath);
void showMainMenu();
void showTasks(const std::vector<Task>& tasks);

#endif

