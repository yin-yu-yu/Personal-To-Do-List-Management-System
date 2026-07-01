# 个人待办事项管理系统

本仓库用于完成《8-1 管理系统小组作业 1.0 版》：以小组为单位实现一个基于 C++ 的个人待办事项管理系统。

## 项目目标

实现一个控制台版个人待办事项管理系统，支持用户对待办事项进行增删改查、状态管理、优先级管理、截止日期管理和本地数据保存。

## 基础功能

- 添加待办事项
- 查看全部待办事项
- 按状态筛选待办事项
- 修改待办事项
- 删除待办事项
- 标记待办事项为已完成
- 将待办事项保存到本地数据文件
- 从本地数据文件读取待办事项

## 仓库结构

```text
.
├── PersonalToDoList.dev        # Dev-C++ 工程文件
├── README.md                   # 项目简介
├── data/
│   └── tasks.csv               # 待办事项数据文件
├── docs/
│   ├── github协作指南.md       # 小组 GitHub 协作文档
│   ├── 会议记录模板.md         # 小组讨论记录模板
│   ├── 分工安排.md             # 5 人小组任务分配
│   └── 项目说明文档.md         # 作业要求中的说明文档初稿
├── include/
│   ├── storage.h               # 文件读写模块头文件
│   ├── task.h                  # 待办事项数据结构头文件
│   └── ui.h                    # 界面交互模块头文件
└── src/
    ├── main.cpp                # 程序入口
    ├── storage.cpp             # 文件读写模块
    ├── task.cpp                # 待办事项业务逻辑
    └── ui.cpp                  # 菜单界面与用户交互
```

## 编译运行

### Dev-C++

1. 使用 Dev-C++ 打开 `PersonalToDoList.dev`。
2. 检查工程中是否包含 `src` 和 `include` 下的所有 `.cpp`、`.h` 文件。
3. 点击“编译并运行”。

### 命令行

```powershell
g++ -std=c++11 -Iinclude src/main.cpp src/task.cpp src/storage.cpp src/ui.cpp -o PersonalToDoList.exe
.\PersonalToDoList.exe
```

## 提交内容对应关系

- 代码程序：`src/`、`include/`、`data/`、`PersonalToDoList.dev`
- 说明文档：`docs/项目说明文档.md`
- 小组分工：`docs/分工安排.md`
- GitHub 协作说明：`docs/github协作指南.md`
- 会议记录模板：`docs/会议记录模板.md`

