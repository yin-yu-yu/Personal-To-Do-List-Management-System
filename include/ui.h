#ifndef UI_H
#define UI_H

#include "task.h"

#include <string>
#include <vector>

// 运行主程序，dataFilePath 为 CSV 数据文件路径
void runApplication(const std::string& dataFilePath);

// 显示主菜单界面
void showMainMenu();

// 以表格形式显示全部待办事项
void showTasks(const std::vector<Task>& tasks);

// 清空控制台屏幕
void clearScreen();

// 暂停等待用户按键后继续
void pauseForUser();

#endif
