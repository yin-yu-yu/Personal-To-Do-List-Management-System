# 个人待办事项管理系统

本仓库用于完成《8-1 管理系统小组作业 2.0 版》：以小组为单位实现一个基于 C++ 的个人待办事项管理系统。

## 项目目标

实现一个控制台版个人待办事项管理系统，支持用户对待办事项进行增删改查、状态管理、优先级管理、截止日期管理和本地数据保存。

## 小组成员

| 角色 | 成员 | 主要职责 |
| --- | --- | --- |
| 组长 / 项目统筹 | 刘阳 | 仓库创建、任务分配、进度协调、最终整合、会议记录 |
| 程序入口与菜单 | 张旭润 | 主菜单、功能选择、输入输出交互 (`main.cpp`, `ui.cpp`, `ui.h`) |
| 待办事项功能 | 梁倚云 | 添加、删除、修改、查询、完成状态等核心业务逻辑 (`task.cpp`, `task.h`) |
| 文件存储 | 张宇航 | CSV 数据文件读写、数据保存和加载 (`storage.cpp`, `storage.h`) |
| 文档与测试 | 贺平方 | 说明文档、用户手册、测试记录、截图整理 (`docs/`) |

注：由于github的原因，图片与视频保存于百度网盘内， 链接：https://pan.baidu.com/s/1wOEifho5EhpZTy_ZPdMA8g?pwd=phh2

## 功能列表

### 已实现功能

- ✅ 添加待办事项（自动生成 ID，支持标题、描述、截止日期、优先级）
- ✅ 查看全部待办事项（表格形式展示）
- ✅ 修改待办事项（支持保留原值）
- ✅ 删除待办事项（带确认提示）
- ✅ 标记为已完成 / 待完成
- ✅ 搜索待办事项（按标题关键字、按状态、按优先级筛选）
- ✅ 查看统计信息（总数、待完成数、已完成数）
- ✅ 优先级管理（1-5 级，1=最高优先级）
- ✅ 截止日期管理
- ✅ 本地 CSV 文件持久化存储
- ✅ CSV 特殊字符转义处理（逗号、引号、换行）
- ✅ 大小写不敏感标题搜索
- ✅ 按优先级排序（UI 集成）
- ✅ 按截止日期排序（UI 集成）
- ✅ 任务标签/分类功能
- ✅ 数据备份与恢复
- ✅ 到期提醒功能
- ✅ 控制台美化（颜色、图标、Unicode 边框）
- ✅ 标签搜索
- ✅ 按标签筛选

## 仓库结构

```text
.
├── PersonalToDoList.dev        # Dev-C++ 工程文件
├── README.md                   # 项目简介（本文件）
├── data/
│   └── tasks.csv               # 待办事项数据文件（CSV 格式）
├── docs/
│   ├── github协作指南.md       # 小组 GitHub 协作文档
│   ├── 会议记录模板.md         # 小组讨论记录模板
│   ├── 会议记录-第1次.md       # 第 1 次小组会议记录
│   ├── 分工安排.md             # 5 人小组任务分配
│   └── 项目说明文档.md         # 作业要求中的说明文档
├── include/
│   ├── storage.h               # 文件读写模块头文件
│   ├── task.h                  # 待办事项数据结构与业务逻辑头文件
│   └── ui.h                    # 界面交互模块头文件
├── src/
│   ├── main.cpp                # 程序入口
│   ├── storage.cpp             # 文件读写模块（CSV 格式）
│   ├── task.cpp                # 待办事项业务逻辑（CRUD + 查询 + 排序 + 统计）
│   └── ui.cpp                  # 菜单界面与用户交互
└── test/
    ├── test_task.cpp           # 核心业务逻辑单元测试
    └── test_storage.cpp        # 文件存储模块单元测试
```

## 技术架构

- **语言标准**：C++11
- **数据存储**：CSV 文件（RFC 4180 兼容）
- **模块划分**：UI 层 → 业务逻辑层 → 存储层
- **依赖关系**：`main` → `ui` → `storage` + `task`

```
main.cpp
  └── ui.h ──┬── task.h  (Task struct, TaskStatus enum, 所有业务函数)
              │
              └── ui.cpp ──┬── storage.h ── storage.cpp
                           │
                           └── task.h ────── task.cpp
```

## 编译运行

### Dev-C++

1. 使用 Dev-C++ 打开 `PersonalToDoList.dev`。
2. 检查工程中是否包含 `src` 和 `include` 下的所有 `.cpp`、`.h` 文件。
3. 点击"编译并运行"。

### 命令行（MinGW / GCC）

```powershell
# 编译主程序
g++ -std=c++11 -Iinclude src/main.cpp src/task.cpp src/storage.cpp src/ui.cpp -o PersonalToDoList.exe

# 运行
.\PersonalToDoList.exe
```

### 运行单元测试

```powershell
# 编译并运行核心逻辑测试
g++ -std=c++11 -Iinclude test/test_task.cpp src/task.cpp -o test/test_task.exe
.\test\test_task.exe

# 编译并运行存储模块测试
g++ -std=c++11 -Iinclude test/test_storage.cpp src/storage.cpp src/task.cpp -o test/test_storage.exe
.\test\test_storage.exe
```

## 使用说明

主菜单：

```
╔══════════════════════════════════════════════╗
║       📋 Personal To-Do List v2.0           ║
╚══════════════════════════════════════════════╝
    1. 📝 Add Task
    2. 📄 View All Tasks
    3. ✏  Edit Task
    4. 🗑  Delete Task
    5. ✅ Mark as Completed
    6. 🔍 Search Tasks
    7. 📊 View Statistics
    8. 🔃 Sort Tasks
    9. 💾 Backup / Restore
    0. 💾 Save & Exit
--------------------------------------------
```

| 选项 | 功能 | 说明 |
| --- | --- | --- |
| 1 | 添加待办事项 | 输入标题、描述、截止日期、优先级（1-5）、标签 |
| 2 | 查看全部 | 表格形式展示所有待办事项（带颜色高亮） |
| 3 | 修改待办事项 | 按 ID 查找，直接回车保留原值 |
| 4 | 删除待办事项 | 按 ID 删除，有确认提示 |
| 5 | 标记为已完成 | 按 ID 标记为 Completed |
| 6 | 搜索待办事项 | 子菜单：按标题/状态/优先级/标签筛选 |
| 7 | 查看统计信息 | 显示总数、待完成数、已完成数及百分比 |
| 8 | 排序任务 | 子菜单：按优先级/截止日期 升序/降序 |
| 9 | 备份恢复 | 子菜单：备份当前数据 / 从备份恢复 |
| 0 | 保存并退出 | 数据写入 `data/tasks.csv` |

## 数据格式

数据文件 `data/tasks.csv` 使用 CSV 格式：

```csv
id,title,description,dueDate,priority,status,tag
1,购买食材,去超市买蔬菜和水果,2026-07-15,2,Pending,shopping
2,完成报告,写小组项目总结报告,2026-07-20,1,Completed,work
```

## 开发流程

本项目使用 GitHub Flow 协作：

1. 组长创建仓库并初始化项目结构
2. 成员从 `main` 分支创建 `feature/姓名-功能` 分支
3. 在各自分支上开发并通过 Pull Request 合并到 `main`
4. 组长负责最终代码审查、合并和整合

详见 [GitHub 协作指南](docs/github协作指南.md)。

## 提交内容对应关系

- 代码程序：`src/`、`include/`、`data/`、`PersonalToDoList.dev`
- 说明文档：`docs/项目说明文档.md`
- 小组分工：`docs/分工安排.md`
- 会议记录：`docs/会议记录-*.md`
- GitHub 协作说明：`docs/github协作指南.md`
- 单元测试：`test/`

## 后续计划（2.0 版）✅ 已完成

- ✅ 增加按优先级和截止日期排序的 UI 交互
- ✅ 增加数据备份和恢复功能
- ✅ 增加任务分类/标签功能
- ✅ 增加到期提醒功能
- ✅ 美化控制台输出（颜色、图标、Unicode 边框）
- ✅ 补充测试截图和运行结果 → 见 `docs/测试报告.md`
- ✅ 补充更完整的项目总结文档 → 见 `docs/项目总结文档.md`

## v2.0 新增内容

### 底层代码
- `Task` 结构体新增 `tag` 字段
- `updateTask()` 支持标签参数
- 新增 `findTasksByTag()` 标签搜索函数
- `storage` 模块新增 `backupTasksFile()` / `restoreTasksFromBackup()`
- CSV 格式新增 `tag` 列（7 列）

### 控制台美化
- ANSI 颜色输出：优先级/状态/到期任务彩色高亮
- Unicode 图标和边框（emoji + box-drawing 字符）
- 到期提醒横幅
- Windows 虚拟终端支持
